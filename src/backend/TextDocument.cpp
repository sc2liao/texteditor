#include "TextDocument.hpp"

#include "PathUtils.hpp"

#include <QFile>
#include <QSaveFile>
#include <QStringConverter>
#include <QTextStream>

namespace backend {

TextDocument::TextDocument(QObject *parent)
    : QObject(parent)
{
}

QString TextDocument::filePath() const { return m_filePath; }
QString TextDocument::fileName() const { return m_fileName; }
QString TextDocument::content() const { return m_content; }
bool TextDocument::isModified() const { return m_content != m_savedContent; }
bool TextDocument::hasOpenFile() const { return !m_filePath.isEmpty(); }

QString TextDocument::fileSize() const { return m_fileSize; }
qint64 TextDocument::fileSizeBytes() const { return m_fileSizeBytes; }
QString TextDocument::encoding() const { return m_encoding; }
bool TextDocument::permissionRead() const { return m_permissionRead; }
bool TextDocument::permissionWrite() const { return m_permissionWrite; }
bool TextDocument::permissionExecute() const { return m_permissionExecute; }
QString TextDocument::permissionsText() const { return m_permissionsText; }
QString TextDocument::lastModified() const { return m_lastModified; }
bool TextDocument::exists() const { return m_exists; }
bool TextDocument::isRegularFile() const { return m_isRegularFile; }
QString TextDocument::lastError() const { return m_lastError; }

void TextDocument::setContent(const QString &content)
{
    if (m_content == content) {
        return;
    }
    m_content = content;
    emit contentChanged();
    emit modifiedChanged();
}

void TextDocument::applyMetadata(const FileMetadata &meta)
{
    m_fileName = meta.fileName;
    m_fileSize = meta.sizeText;
    m_fileSizeBytes = meta.sizeBytes;
    m_encoding = meta.encoding;
    m_permissionRead = meta.permissionRead;
    m_permissionWrite = meta.permissionWrite;
    m_permissionExecute = meta.permissionExecute;
    m_permissionsText = meta.permissionsText;
    m_lastModified = meta.lastModified;
    m_exists = meta.exists;
    m_isRegularFile = meta.isRegularFile;
    emit metadataChanged();
}

void TextDocument::clearMetadata()
{
    m_fileName.clear();
    m_fileSize.clear();
    m_fileSizeBytes = 0;
    m_encoding = QStringLiteral("UTF-8");
    m_permissionRead = false;
    m_permissionWrite = false;
    m_permissionExecute = false;
    m_permissionsText.clear();
    m_lastModified.clear();
    m_exists = false;
    m_isRegularFile = false;
    emit metadataChanged();
}

void TextDocument::setLastError(const QString &message)
{
    if (m_lastError == message) {
        return;
    }
    m_lastError = message;
    emit lastErrorChanged();
}

bool TextDocument::loadFromDisk(const std::filesystem::path &path)
{
    QFile file(QString::fromStdString(path.string()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setLastError(QStringLiteral("Cannot open file for reading: %1").arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    const QString text = in.readAll();
    file.close();

    m_content = text;
    m_savedContent = text;
    emit contentChanged();
    emit modifiedChanged();
    return true;
}

bool TextDocument::writeToDisk(const std::filesystem::path &path)
{
    QSaveFile file(QString::fromStdString(path.string()));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setLastError(QStringLiteral("Cannot open file for writing: %1").arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << m_content;
    if (!file.commit()) {
        setLastError(QStringLiteral("Failed to save file: %1").arg(file.errorString()));
        return false;
    }

    m_savedContent = m_content;
    emit modifiedChanged();
    return true;
}

bool TextDocument::openFile(const QString &path)
{
    const auto fsPathOpt = toFilesystemPath(path);
    if (!fsPathOpt) {
        const auto msg = QStringLiteral("Invalid file path: %1").arg(path);
        setLastError(msg);
        emit openFailed(msg);
        return false;
    }
    const std::filesystem::path &fsPath = *fsPathOpt;
    std::error_code ec;
    if (!std::filesystem::exists(fsPath, ec) || ec) {
        const auto msg = QStringLiteral("File does not exist: %1").arg(normalizeLocalPath(path));
        setLastError(msg);
        emit openFailed(msg);
        return false;
    }

    if (!std::filesystem::is_regular_file(fsPath, ec) || ec) {
        const auto msg = QStringLiteral("Not a regular file: %1").arg(path);
        setLastError(msg);
        emit openFailed(msg);
        return false;
    }

    if (!loadFromDisk(fsPath)) {
        emit openFailed(m_lastError);
        return false;
    }

    const auto meta = FileMetadata::fromPath(fsPath);
    if (!meta) {
        const auto msg = QStringLiteral("Failed to read file metadata: %1").arg(path);
        setLastError(msg);
        emit openFailed(msg);
        return false;
    }

    m_filePath = meta->path;
    applyMetadata(*meta);
    setLastError({});
    emit filePathChanged();
    emit openSucceeded(m_filePath);
    return true;
}

bool TextDocument::save()
{
    if (m_filePath.isEmpty()) {
        const auto msg = QStringLiteral("No file path set. Use saveAs(path) first.");
        setLastError(msg);
        emit saveFailed(msg);
        return false;
    }
    return saveAs(m_filePath);
}

bool TextDocument::saveAs(const QString &path)
{
    const auto fsPathOpt = toFilesystemPath(path);
    if (!fsPathOpt) {
        const auto msg = QStringLiteral("Invalid file path: %1").arg(path);
        setLastError(msg);
        emit saveFailed(msg);
        return false;
    }
    const std::filesystem::path &fsPath = *fsPathOpt;

    if (!writeToDisk(fsPath)) {
        emit saveFailed(m_lastError);
        return false;
    }

    const auto meta = FileMetadata::fromPath(fsPath);
    if (meta) {
        m_filePath = meta->path;
        applyMetadata(*meta);
    } else {
        m_filePath = normalizeLocalPath(path);
    }

    setLastError({});
    emit filePathChanged();
    emit saveSucceeded(m_filePath);
    return true;
}

void TextDocument::resetDocument()
{
    m_filePath.clear();
    m_content.clear();
    m_savedContent.clear();
    clearMetadata();
    setLastError({});
    emit filePathChanged();
    emit contentChanged();
    emit modifiedChanged();
}

void TextDocument::newFile()
{
    resetDocument();
}

void TextDocument::closeFile()
{
    resetDocument();
}

void TextDocument::refreshMetadata()
{
    if (m_filePath.isEmpty()) {
        return;
    }

    const auto fsPathOpt = toFilesystemPath(m_filePath);
    if (!fsPathOpt) {
        return;
    }
    const auto meta = FileMetadata::fromPath(*fsPathOpt);
    if (meta) {
        applyMetadata(*meta);
    }
}

} // namespace backend

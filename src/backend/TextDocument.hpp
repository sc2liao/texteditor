#pragma once

#include "FileMetadata.hpp"

#include <QObject>
#include <QString>

namespace backend {

class TextDocument : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY metadataChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(bool modified READ isModified NOTIFY modifiedChanged)
    Q_PROPERTY(bool hasOpenFile READ hasOpenFile NOTIFY filePathChanged)

    Q_PROPERTY(QString fileSize READ fileSize NOTIFY metadataChanged)
    Q_PROPERTY(qint64 fileSizeBytes READ fileSizeBytes NOTIFY metadataChanged)
    Q_PROPERTY(QString encoding READ encoding NOTIFY metadataChanged)
    Q_PROPERTY(bool permissionRead READ permissionRead NOTIFY metadataChanged)
    Q_PROPERTY(bool permissionWrite READ permissionWrite NOTIFY metadataChanged)
    Q_PROPERTY(bool permissionExecute READ permissionExecute NOTIFY metadataChanged)
    Q_PROPERTY(QString permissionsText READ permissionsText NOTIFY metadataChanged)
    Q_PROPERTY(QString lastModified READ lastModified NOTIFY metadataChanged)
    Q_PROPERTY(bool exists READ exists NOTIFY metadataChanged)
    Q_PROPERTY(bool isRegularFile READ isRegularFile NOTIFY metadataChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)

public:
    explicit TextDocument(QObject *parent = nullptr);

    QString filePath() const;
    QString fileName() const;
    QString content() const;
    bool isModified() const;
    bool hasOpenFile() const;

    QString fileSize() const;
    qint64 fileSizeBytes() const;
    QString encoding() const;
    bool permissionRead() const;
    bool permissionWrite() const;
    bool permissionExecute() const;
    QString permissionsText() const;
    QString lastModified() const;
    bool exists() const;
    bool isRegularFile() const;
    QString lastError() const;

    void setContent(const QString &content);

    Q_INVOKABLE bool openFile(const QString &path);
    Q_INVOKABLE bool save();
    Q_INVOKABLE bool saveAs(const QString &path);
    Q_INVOKABLE void newFile();
    Q_INVOKABLE void closeFile();
    Q_INVOKABLE void refreshMetadata();

signals:
    void filePathChanged();
    void contentChanged();
    void modifiedChanged();
    void metadataChanged();
    void lastErrorChanged();
    void openFailed(const QString &message);
    void saveFailed(const QString &message);
    void openSucceeded(const QString &path);
    void saveSucceeded(const QString &path);

private:
    bool loadFromDisk(const std::filesystem::path &path);
    bool writeToDisk(const std::filesystem::path &path);
    void applyMetadata(const FileMetadata &meta);
    void clearMetadata();
    void resetDocument();
    void setLastError(const QString &message);

    QString m_filePath;
    QString m_content;
    QString m_savedContent;
    QString m_lastError;

    QString m_fileName;
    QString m_fileSize;
    qint64 m_fileSizeBytes = 0;
    QString m_encoding = QStringLiteral("UTF-8");
    bool m_permissionRead = false;
    bool m_permissionWrite = false;
    bool m_permissionExecute = false;
    QString m_permissionsText;
    QString m_lastModified;
    bool m_exists = false;
    bool m_isRegularFile = false;
};

} // namespace backend

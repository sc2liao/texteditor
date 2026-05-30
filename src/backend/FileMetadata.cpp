#include "FileMetadata.hpp"

#include <QDateTime>
#include <QFile>
#include <chrono>
#include <system_error>

namespace backend {

namespace {

QString formatSize(std::int64_t bytes)
{
    if (bytes < 0) {
        return QStringLiteral("—");
    }
    if (bytes < 1024) {
        return QStringLiteral("%1 B").arg(bytes);
    }
    if (bytes < 1024 * 1024) {
        return QStringLiteral("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    }
    if (bytes < 1024 * 1024 * 1024) {
        return QStringLiteral("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    }
    return QStringLiteral("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 1);
}

bool hasPerm(std::filesystem::perms permissions, std::filesystem::perms bit)
{
    using std::filesystem::perms;
    const auto masked = permissions & bit;
    return masked != perms::none && masked != perms::unknown;
}

QString formatPermissions(std::filesystem::perms permissions)
{
    using std::filesystem::perms;

    auto triplet = [&](perms read, perms write, perms exec) {
        QString out;
        out += hasPerm(permissions, read) ? QLatin1Char('r') : QLatin1Char('-');
        out += hasPerm(permissions, write) ? QLatin1Char('w') : QLatin1Char('-');
        out += hasPerm(permissions, exec) ? QLatin1Char('x') : QLatin1Char('-');
        return out;
    };

    return triplet(perms::owner_read, perms::owner_write, perms::owner_exec)
         + triplet(perms::group_read, perms::group_write, perms::group_exec)
         + triplet(perms::others_read, perms::others_write, perms::others_exec);
}

QString detectEncodingFromBom(const QByteArray &head)
{
    if (head.startsWith("\xEF\xBB\xBF")) {
        return QStringLiteral("UTF-8 (BOM)");
    }
    if (head.startsWith("\xFF\xFE")) {
        return QStringLiteral("UTF-16 LE");
    }
    if (head.startsWith("\xFE\xFF")) {
        return QStringLiteral("UTF-16 BE");
    }
    if (head.startsWith("\xFF\xFE\x00\x00")) {
        return QStringLiteral("UTF-32 LE");
    }
    if (head.startsWith("\x00\x00\xFE\xFF")) {
        return QStringLiteral("UTF-32 BE");
    }
    return QStringLiteral("UTF-8");
}

} // namespace

std::optional<FileMetadata> FileMetadata::fromPath(const std::filesystem::path &path)
{
    std::error_code ec;
    if (!std::filesystem::exists(path, ec) || ec) {
        return std::nullopt;
    }

    FileMetadata meta;
    const auto absolute = std::filesystem::weakly_canonical(path, ec);
    meta.path = QString::fromStdString((ec ? path : absolute).string());
    meta.fileName = QString::fromStdString(path.filename().string());
    meta.exists = true;
    meta.isRegularFile = std::filesystem::is_regular_file(path, ec);

    if (meta.isRegularFile) {
        meta.sizeBytes = static_cast<std::int64_t>(std::filesystem::file_size(path, ec));
        if (ec) {
            meta.sizeBytes = -1;
        }
    } else {
        meta.sizeBytes = 0;
    }
    meta.sizeText = formatSize(meta.sizeBytes);

    const auto filePerms = std::filesystem::status(path, ec).permissions();
    if (!ec) {
        using std::filesystem::perms;
        meta.permissionRead = hasPerm(filePerms, perms::owner_read)
                           || hasPerm(filePerms, perms::group_read)
                           || hasPerm(filePerms, perms::others_read);
        meta.permissionWrite = hasPerm(filePerms, perms::owner_write)
                            || hasPerm(filePerms, perms::group_write)
                            || hasPerm(filePerms, perms::others_write);
        meta.permissionExecute = hasPerm(filePerms, perms::owner_exec)
                              || hasPerm(filePerms, perms::group_exec)
                              || hasPerm(filePerms, perms::others_exec);
        meta.permissionsText = formatPermissions(filePerms);
    }

    const auto ftime = std::filesystem::last_write_time(path, ec);
    if (!ec) {
        const auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - std::filesystem::file_time_type::clock::now()
            + std::chrono::system_clock::now());
        const auto time = std::chrono::system_clock::to_time_t(sctp);
        meta.lastModified = QDateTime::fromSecsSinceEpoch(time).toString(Qt::ISODate);
    }

    if (meta.isRegularFile) {
        QFile file(meta.path);
        if (file.open(QIODevice::ReadOnly)) {
            meta.encoding = detectEncodingFromBom(file.read(4));
        } else {
            meta.encoding = QStringLiteral("UTF-8");
        }
    } else {
        meta.encoding = QStringLiteral("UTF-8");
    }

    return meta;
}

} // namespace backend

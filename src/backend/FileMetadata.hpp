#pragma once

#include <QString>
#include <cstdint>
#include <filesystem>
#include <optional>

namespace backend {

struct FileMetadata {
    QString path;
    QString fileName;
    std::int64_t sizeBytes = 0;
    QString sizeText;
    QString encoding;
    bool permissionRead = false;
    bool permissionWrite = false;
    bool permissionExecute = false;
    QString permissionsText;
    QString lastModified;
    bool exists = false;
    bool isRegularFile = false;

    static std::optional<FileMetadata> fromPath(const std::filesystem::path &path);
};

} // namespace backend

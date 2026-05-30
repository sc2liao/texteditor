#pragma once

#include <QString>
#include <filesystem>
#include <optional>

namespace backend {

// Converts QML/FileDialog paths (file://, ///home/..., UNC) to a native local path.
QString normalizeLocalPath(const QString &input);

std::optional<std::filesystem::path> toFilesystemPath(const QString &input);

} // namespace backend

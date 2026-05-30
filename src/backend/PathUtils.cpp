#include "PathUtils.hpp"

#include <QDir>
#include <QRegularExpression>
#include <QUrl>

namespace backend {

namespace {

QString fromFileUrl(const QUrl &url)
{
    if (!url.isValid() || url.scheme() != QLatin1String("file")) {
        return {};
    }

    const QString local = url.toLocalFile();
    return local.isEmpty() ? QString() : QDir::cleanPath(local);
}

QString fromWslUncPath(const QString &input)
{
    QString path = input;
    path.replace(QLatin1Char('\\'), QLatin1Char('/'));

    static const QRegularExpression pattern(
        QStringLiteral(R"(^//wsl(?:\.localhost)?(?:\$|/)([^/]+)(/.*)?$)"),
        QRegularExpression::CaseInsensitiveOption);

    const auto match = pattern.match(path);
    if (!match.hasMatch()) {
        return {};
    }

    const QString distro = match.captured(1);
    QString linuxPath = match.captured(2);
    if (linuxPath.isEmpty()) {
        linuxPath = QStringLiteral("/");
    }

    const QByteArray currentDistro = qgetenv("WSL_DISTRO_NAME");
    if (!currentDistro.isEmpty()
        && distro.compare(QString::fromUtf8(currentDistro), Qt::CaseInsensitive) != 0) {
        return {};
    }

    return QDir::cleanPath(linuxPath);
}

} // namespace

QString normalizeLocalPath(const QString &input)
{
    const QString trimmed = input.trimmed();
    if (trimmed.isEmpty()) {
        return {};
    }

    if (trimmed.startsWith(QLatin1String("file:"), Qt::CaseInsensitive)) {
        const QString fromUrl = fromFileUrl(QUrl(trimmed));
        if (!fromUrl.isEmpty()) {
            return fromUrl;
        }
    }

    // Malformed URL from QML: ///home/user/... (file scheme lost, extra slash)
    if (trimmed.startsWith(QLatin1String("///"))) {
        const QString fromUrl = fromFileUrl(QUrl(QStringLiteral("file:") + trimmed));
        if (!fromUrl.isEmpty()) {
            return fromUrl;
        }
        return QDir::cleanPath(trimmed.mid(2));
    }

    if (trimmed.startsWith(QLatin1String("//wsl"), Qt::CaseInsensitive)
        || trimmed.startsWith(QLatin1String("\\\\wsl"), Qt::CaseInsensitive)) {
        const QString wslPath = fromWslUncPath(trimmed);
        if (!wslPath.isEmpty()) {
            return wslPath;
        }
    }

    const QUrl userInput = QUrl::fromUserInput(trimmed);
    if (userInput.isValid() && userInput.scheme() == QLatin1String("file")) {
        const QString fromUrl = fromFileUrl(userInput);
        if (!fromUrl.isEmpty()) {
            return fromUrl;
        }
    }

    return QDir::cleanPath(trimmed);
}

std::optional<std::filesystem::path> toFilesystemPath(const QString &input)
{
    const QString local = normalizeLocalPath(input);
    if (local.isEmpty()) {
        return std::nullopt;
    }
    return std::filesystem::path(local.toStdString());
}

} // namespace backend

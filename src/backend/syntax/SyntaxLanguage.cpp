#include "SyntaxLanguage.hpp"

#include <QStringList>

namespace backend {

SyntaxLanguage detectSyntaxLanguage(const QString &filePath)
{
    const QString lower = filePath.toLower();

    static const QStringList cppExtensions = {
        QStringLiteral(".cpp"),
        QStringLiteral(".cxx"),
        QStringLiteral(".cc"),
        QStringLiteral(".hpp"),
        QStringLiteral(".hxx"),
        QStringLiteral(".hh"),
    };

    static const QStringList cExtensions = {
        QStringLiteral(".c"),
        QStringLiteral(".h"),
    };

    for (const QString &extension : cppExtensions) {
        if (lower.endsWith(extension)) {
            return SyntaxLanguage::Cpp;
        }
    }

    for (const QString &extension : cExtensions) {
        if (lower.endsWith(extension)) {
            return SyntaxLanguage::C;
        }
    }

    return SyntaxLanguage::None;
}

QString syntaxLanguageName(SyntaxLanguage language)
{
    switch (language) {
    case SyntaxLanguage::C:
        return QStringLiteral("C");
    case SyntaxLanguage::Cpp:
        return QStringLiteral("C++");
    case SyntaxLanguage::None:
        break;
    }
    return QStringLiteral("Plain text");
}

} // namespace backend

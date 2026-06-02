#pragma once

#include <QString>

namespace backend {

enum class SyntaxLanguage {
    None,
    C,
    Cpp,
};

SyntaxLanguage detectSyntaxLanguage(const QString &filePath);

QString syntaxLanguageName(SyntaxLanguage language);

} // namespace backend

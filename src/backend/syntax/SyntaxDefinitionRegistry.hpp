#pragma once

#include "SyntaxLanguage.hpp"

#include <QStringList>

namespace backend {

struct SyntaxDefinition {
    SyntaxLanguage language = SyntaxLanguage::None;
    QString id;
    QStringList extensions;
    QStringList controlKeywords;
    QStringList typeKeywords;
    bool highlightFunctions = true;
    bool highlightPreprocessor = true;
};

class SyntaxDefinitionRegistry
{
public:
    static const SyntaxDefinition &definitionFor(SyntaxLanguage language);
    static SyntaxLanguage detectLanguage(const QString &filePath);

private:
    static const SyntaxDefinition &cDefinition();
    static const SyntaxDefinition &cppDefinition();
};

} // namespace backend

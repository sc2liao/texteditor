#pragma once

#include "SyntaxDefinitionRegistry.hpp"

#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>

namespace backend {

class GenericSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    struct Rule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    explicit GenericSyntaxHighlighter(const SyntaxDefinition &definition, QTextDocument *document = nullptr);

    SyntaxLanguage language() const { return m_language; }

protected:
    void highlightBlock(const QString &text) override;

private:
    void buildRules(const SyntaxDefinition &definition);

    SyntaxLanguage m_language = SyntaxLanguage::None;
    QVector<Rule> m_rules;
    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
    QTextCharFormat m_multiLineCommentFormat;
    QTextCharFormat m_defaultFormat;
};

} // namespace backend

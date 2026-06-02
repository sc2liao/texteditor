#include "GenericSyntaxHighlighter.hpp"

#include <QColor>
#include <QFont>

namespace backend {

namespace {

QTextCharFormat makeFormat(const QColor &color, bool bold = false, bool italic = false)
{
    QTextCharFormat format;
    format.setForeground(color);
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    format.setFontItalic(italic);
    return format;
}

void addKeywordRules(QVector<GenericSyntaxHighlighter::Rule> *rules,
                     const QStringList &keywords,
                     const QTextCharFormat &format)
{
    for (const QString &keyword : keywords) {
        GenericSyntaxHighlighter::Rule rule;
        rule.pattern = QRegularExpression(
            QStringLiteral("\\b%1\\b").arg(QRegularExpression::escape(keyword)));
        rule.format = format;
        rules->append(rule);
    }
}

} // namespace

GenericSyntaxHighlighter::GenericSyntaxHighlighter(const SyntaxDefinition &definition,
                                                     QTextDocument *document)
    : QSyntaxHighlighter(document)
    , m_language(definition.language)
{
    buildRules(definition);
}

void GenericSyntaxHighlighter::buildRules(const SyntaxDefinition &definition)
{
    m_defaultFormat = makeFormat(QColor(QStringLiteral("#f0f0f0")));

    const QTextCharFormat keywordFormat = makeFormat(QColor(QStringLiteral("#569cd6")));
    const QTextCharFormat typeFormat = makeFormat(QColor(QStringLiteral("#4ec9b0")));
    const QTextCharFormat preprocessorFormat = makeFormat(QColor(QStringLiteral("#c586c0")));
    const QTextCharFormat stringFormat = makeFormat(QColor(QStringLiteral("#ce9178")));
    const QTextCharFormat numberFormat = makeFormat(QColor(QStringLiteral("#b5cea8")));
    const QTextCharFormat singleLineCommentFormat = makeFormat(QColor(QStringLiteral("#6a9955")), false, true);
    m_multiLineCommentFormat = makeFormat(QColor(QStringLiteral("#6a9955")), false, true);
    const QTextCharFormat functionFormat = makeFormat(QColor(QStringLiteral("#dcdcaa")));

    addKeywordRules(&m_rules, definition.controlKeywords, keywordFormat);
    addKeywordRules(&m_rules, definition.typeKeywords, typeFormat);

    if (definition.highlightPreprocessor) {
        Rule preprocessorRule;
        preprocessorRule.pattern =
            QRegularExpression(QStringLiteral("^[ \\t]*#[ \\t]*[a-zA-Z_][a-zA-Z0-9_]*"));
        preprocessorRule.format = preprocessorFormat;
        m_rules.append(preprocessorRule);
    }

    if (definition.highlightFunctions) {
        Rule functionRule;
        functionRule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\s*\\()"));
        functionRule.format = functionFormat;
        m_rules.append(functionRule);
    }

    Rule stringRule;
    stringRule.pattern = QRegularExpression(QStringLiteral("\".*?\""));
    stringRule.format = stringFormat;
    m_rules.append(stringRule);

    Rule charRule;
    charRule.pattern = QRegularExpression(QStringLiteral("'.*?'"));
    charRule.format = stringFormat;
    m_rules.append(charRule);

    Rule numberRule;
    numberRule.pattern = QRegularExpression(
        QStringLiteral("\\b(?:0[xX][0-9a-fA-F]+|[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?)[lLuUfF]*\\b"));
    numberRule.format = numberFormat;
    m_rules.append(numberRule);

    Rule singleLineCommentRule;
    singleLineCommentRule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    singleLineCommentRule.format = singleLineCommentFormat;
    m_rules.append(singleLineCommentRule);

    m_commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    m_commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void GenericSyntaxHighlighter::highlightBlock(const QString &text)
{
    setFormat(0, text.length(), m_defaultFormat);

    for (const Rule &rule : std::as_const(m_rules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            const QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        const QRegularExpressionMatch startMatch = m_commentStartExpression.match(text);
        startIndex = startMatch.capturedStart();
    }

    while (startIndex >= 0) {
        const QRegularExpressionMatch endMatch = m_commentEndExpression.match(text, startIndex);
        int endIndex = endMatch.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + endMatch.capturedLength();
        }
        setFormat(startIndex, commentLength, m_multiLineCommentFormat);
        const QRegularExpressionMatch startMatch = m_commentStartExpression.match(text, startIndex + commentLength);
        startIndex = startMatch.capturedStart();
    }
}

} // namespace backend

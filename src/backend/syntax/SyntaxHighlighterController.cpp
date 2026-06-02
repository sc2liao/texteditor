#include "SyntaxHighlighterController.hpp"

#include "GenericSyntaxHighlighter.hpp"
#include "SyntaxDefinitionRegistry.hpp"

#include <QColor>
#include <QQuickTextDocument>
#include <QTextCursor>
#include <QTextDocument>

namespace backend {

SyntaxHighlighterController::SyntaxHighlighterController(QObject *parent)
    : QObject(parent)
{
}

SyntaxHighlighterController::~SyntaxHighlighterController()
{
    qDeleteAll(m_highlighters);
    m_highlighters.clear();
    m_languages.clear();
}

QString SyntaxHighlighterController::languageNameForFile(const QString &filePath) const
{
    return syntaxLanguageName(SyntaxDefinitionRegistry::detectLanguage(filePath));
}

QTextDocument *SyntaxHighlighterController::resolveTextDocument(QObject *textDocumentObject) const
{
    auto *quickDocument = qobject_cast<QQuickTextDocument *>(textDocumentObject);
    if (!quickDocument) {
        return nullptr;
    }
    return quickDocument->textDocument();
}

QSyntaxHighlighter *SyntaxHighlighterController::createHighlighter(SyntaxLanguage language,
                                                                     QTextDocument *document) const
{
    if (language == SyntaxLanguage::None) {
        return nullptr;
    }

    const SyntaxDefinition &definition = SyntaxDefinitionRegistry::definitionFor(language);
    return new GenericSyntaxHighlighter(definition, document);
}

void SyntaxHighlighterController::setHighlighter(QTextDocument *document,
                                                   QSyntaxHighlighter *highlighter,
                                                   SyntaxLanguage language)
{
    if (!document) {
        return;
    }

    if (QSyntaxHighlighter *existing = m_highlighters.value(document, nullptr)) {
        delete existing;
        m_highlighters.remove(document);
        m_languages.remove(document);
    }

    if (highlighter) {
        m_highlighters.insert(document, highlighter);
        m_languages.insert(document, language);
    }
}

void SyntaxHighlighterController::applyForFileName(QObject *textDocumentObject, const QString &filePath)
{
    QTextDocument *document = resolveTextDocument(textDocumentObject);
    if (!document) {
        return;
    }

    const SyntaxLanguage language = SyntaxDefinitionRegistry::detectLanguage(filePath);
    if (language == SyntaxLanguage::None) {
        clear(textDocumentObject);
        return;
    }

    if (QSyntaxHighlighter *existing = m_highlighters.value(document, nullptr)) {
        if (m_languages.value(document) == language) {
            existing->rehighlight();
            return;
        }
    }

    setHighlighter(document, createHighlighter(language, document), language);
}

void SyntaxHighlighterController::clear(QObject *textDocumentObject)
{
    QTextDocument *document = resolveTextDocument(textDocumentObject);
    if (!document) {
        return;
    }

    setHighlighter(document, nullptr, SyntaxLanguage::None);

    QTextCursor cursor(document);
    cursor.select(QTextCursor::Document);
    QTextCharFormat plain;
    plain.setForeground(QColor(QStringLiteral("#f0f0f0")));
    cursor.mergeCharFormat(plain);
    cursor.clearSelection();
}

} // namespace backend

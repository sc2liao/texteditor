#pragma once

#include "SyntaxLanguage.hpp"

#include <QHash>
#include <QObject>
#include <QString>

class QQuickTextDocument;
class QSyntaxHighlighter;
class QTextDocument;

namespace backend {

class SyntaxHighlighterController : public QObject
{
    Q_OBJECT

public:
    explicit SyntaxHighlighterController(QObject *parent = nullptr);
    ~SyntaxHighlighterController() override;

    Q_INVOKABLE void applyForFileName(QObject *textDocumentObject, const QString &filePath);
    Q_INVOKABLE void clear(QObject *textDocumentObject);
    Q_INVOKABLE QString languageNameForFile(const QString &filePath) const;

private:
    QTextDocument *resolveTextDocument(QObject *textDocumentObject) const;
    void setHighlighter(QTextDocument *document, QSyntaxHighlighter *highlighter, SyntaxLanguage language);
    QSyntaxHighlighter *createHighlighter(SyntaxLanguage language, QTextDocument *document) const;

    QHash<QTextDocument *, QSyntaxHighlighter *> m_highlighters;
    QHash<QTextDocument *, SyntaxLanguage> m_languages;
};

} // namespace backend

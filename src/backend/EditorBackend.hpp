#pragma once

#include "TextDocument.hpp"

#include <QObject>
#include <QQmlEngine>

namespace backend {

// QML-facing facade: owns the active document and exposes it to the UI.
class EditorBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TextDocument *document READ document CONSTANT)

public:
    explicit EditorBackend(QObject *parent = nullptr);

    TextDocument *document();

    static void registerQmlTypes();

private:
    TextDocument m_document;
};

} // namespace backend

#include "EditorBackend.hpp"

#include <QQmlEngine>
#include <qqml.h>

namespace backend {

EditorBackend::EditorBackend(QObject *parent)
    : QObject(parent)
{
}

TextDocument *EditorBackend::document()
{
    return &m_document;
}

void EditorBackend::registerQmlTypes()
{
    qmlRegisterType<TextDocument>("TextEditor.Backend", 1, 0, "TextDocument");
    qmlRegisterSingletonType<EditorBackend>(
        "TextEditor.Backend",
        1,
        0,
        "Editor",
        [](QQmlEngine *engine, QJSEngine *) -> QObject * {
            Q_UNUSED(engine);
            static EditorBackend instance;
            QQmlEngine::setObjectOwnership(&instance, QQmlEngine::CppOwnership);
            return &instance;
        });
}

} // namespace backend

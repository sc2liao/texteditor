#include "backend/EditorBackend.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    backend::EditorBackend::registerQmlTypes();

    QQmlApplicationEngine engine;
    engine.loadFromModule("TextEditor", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}

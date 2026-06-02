cmake_minimum_required(VERSION 3.21)

# 1. SET THE PATH FIRST
# We set CMAKE_PREFIX_PATH at the very top so EVERY find_package call knows where to look.
set(CMAKE_PREFIX_PATH "/home/denny/Qt/6.8.0/gcc_64/")

project(texteditor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. FIND QT6
# Because we set CMAKE_PREFIX_PATH above, we don't need PATHS or NO_DEFAULT_PATH here.
# CMake will check our Denny folder first automatically.
find_package(Qt6 6.8 REQUIRED COMPONENTS Core Gui Qml Quick)

# This is a helper macro from Qt to set up standard paths
qt_standard_project_setup(REQUIRES 6.8)

# 3. EXECUTABLE (C++ entry only; UI lives under qml/)
qt_add_executable(texteditor
    src/main.cpp
    src/backend/FileMetadata.cpp
    src/backend/PathUtils.cpp
    src/backend/TextDocument.cpp
    src/backend/EditorBackend.cpp
    src/backend/syntax/SyntaxLanguage.cpp
    src/backend/syntax/SyntaxDefinitionRegistry.cpp
    src/backend/syntax/GenericSyntaxHighlighter.cpp
    src/backend/syntax/SyntaxHighlighterController.cpp
)

target_include_directories(texteditor PRIVATE src)

qt_add_qml_module(texteditor
    URI TextEditor
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/EditorPane.qml
        qml/components/UnsavedChangesDialog.qml
)

# 4. LINKING
target_link_libraries(texteditor
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)
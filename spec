ramp-up exercise: 多功能 GUI 筆記本 (Qt6 & C++20/23)
這是一個小練習, 包含部門會用到的QT/QML, C++ 以及github, cmake工具的使用, 僅作為個人學習的目標加強學習效率, 並不會有考核也不會有檢查

Objective
熟悉github push/pull/PR 流程
瞭解QML 開發以及除厝流程
熟悉cmake 建置流程
prerequesties
sign up github and create a repo named "texteditor"
install QT6.8 LTS, and remember this reference: https://doc.qt.io/qt-6.8/qmltypes.html
specification
1. 核心功能
多頁面管理：支援同時開啟多個檔案（Tabs）。
檔案操作：實作檔案的讀取與儲存功能。
右鍵選單 (Context Menu)：
在當前編輯分頁點擊右鍵，需跳出「檔案屬性」視窗或選單。
屬性內容包含：檔案路徑、檔案大小、編碼格式（預設為 UTF-8/Text）、檔案權限（Read/Write/Execute）。
2. 分支任務與 Git 工作流
在 GitHub Repo 中建置以下branches:

main Branch：基礎筆記本功能（純文字讀寫、Tab 管理、右鍵屬性）。
markdown Branch：延伸開發，支援 Markdown Syntax Highlight。
cpp Branch：延伸開發，支援 C++ Syntax Highlight。
整合作業：請分別從 markdown 與 cpp 提交 Pull Request (PR) 至 main 分支，模擬程式碼審核流程。
💡 開發重點提示 (給新同仁的建議)
C++ 整合 QML：建議將檔案 I/O 邏輯實作在 C++ Backend Class 中，並透過 Q_PROPERTY 或 Q_INVOKABLE 暴露給 QML 調用。
檔案屬性獲取：可以善用 C++17 引入的 <filesystem> 庫，這在處理檔案權限與大小時非常高效。
CMake 規範：請確保 CMakeLists.txt 寫法標準，能正確連結 Qt6 各模組（Core, Gui, Qml, Quick）。
Syntax Highlighting：
研究 QSyntaxHighlighter 類別的使用。
考慮如何設計架構，讓不同的 Branch 能靈活切換或擴充不同的 Highlighter 邏輯。
📥 交付方式
透過PR提交至githubu repo 即可
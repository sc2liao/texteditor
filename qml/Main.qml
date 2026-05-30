import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import TextEditor.Backend 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    title: windowTitle

    property TextDocument document: Editor.document
    property string windowTitle: {
        if (!document.hasOpenFile) {
            return qsTr("Text Editor")
        }
        const star = document.modified ? " *" : ""
        return document.fileName + star + qsTr(" — Text Editor")
    }

    // Pending action after unsaved-changes dialog: "open" | "close" | null
    property string pendingAction: ""

    function toLocalPath(urlOrPath) {
        if (urlOrPath === undefined || urlOrPath === null) {
            return ""
        }
        if (typeof urlOrPath === "string") {
            return urlOrPath
        }
        return urlOrPath.toLocalFile !== undefined ? urlOrPath.toLocalFile() : urlOrPath.toString()
    }

    function performOpen(path) {
        document.openFile(toLocalPath(path))
    }

    function performClose() {
        document.closeFile()
    }

    function requestOpen(path) {
        if (document.hasOpenFile && document.modified) {
            pendingAction = "open"
            unsavedDialog.fileName = document.fileName
            unsavedDialog.open()
            unsavedDialog.pendingPath = toLocalPath(path)
        } else {
            performOpen(path)
        }
    }

    function requestClose() {
        if (!document.hasOpenFile) {
            return
        }
        if (document.modified) {
            pendingAction = "close"
            unsavedDialog.fileName = document.fileName
            unsavedDialog.open()
        } else {
            performClose()
        }
    }

    FileDialog {
        id: openDialog
        title: qsTr("Open file")
        fileMode: FileDialog.OpenFile
        onAccepted: requestOpen(toLocalPath(selectedFile))
    }

    FileDialog {
        id: saveDialog
        title: qsTr("Save file")
        fileMode: FileDialog.SaveFile
        currentFile: document.hasOpenFile ? document.filePath : ""
        onAccepted: document.saveAs(toLocalPath(selectedFile))
    }

    UnsavedChangesDialog {
        id: unsavedDialog
        fileName: ""
        property string pendingPath: ""

        onSaveAndContinue: {
            if (document.save()) {
                if (pendingAction === "open") {
                    performOpen(pendingPath)
                } else if (pendingAction === "close") {
                    performClose()
                }
            }
            pendingAction = ""
        }
        onDiscardAndContinue: {
            if (pendingAction === "open") {
                performOpen(pendingPath)
            } else if (pendingAction === "close") {
                performClose()
            }
            pendingAction = ""
        }
        onCancelled: pendingAction = ""
    }

    Dialog {
        id: propertiesDialog
        title: qsTr("File properties")
        modal: true
        standardButtons: Dialog.Close
        width: 480

        ColumnLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16
            spacing: 8

            Label {
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                text: qsTr("Path: %1").arg(document.filePath)
            }
            Label { text: qsTr("Size: %1").arg(document.fileSize) }
            Label { text: qsTr("Encoding: %1").arg(document.encoding) }
            Label {
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                text: qsTr("Permissions: %1 (Read:%2 Write:%3 Execute:%4)")
                    .arg(document.permissionsText)
                    .arg(document.permissionRead ? qsTr("yes") : qsTr("no"))
                    .arg(document.permissionWrite ? qsTr("yes") : qsTr("no"))
                    .arg(document.permissionExecute ? qsTr("yes") : qsTr("no"))
            }
            Label { text: qsTr("Last modified: %1").arg(document.lastModified) }
        }
    }

    Dialog {
        id: errorDialog
        property string message: ""
        title: qsTr("Error")
        modal: true
        standardButtons: Dialog.Ok
        width: 400
        height: 250

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 16
            wrapMode: Text.WordWrap
            text: errorDialog.message
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            Action {
                text: qsTr("Open…")
                shortcut: StandardKey.Open
                onTriggered: openDialog.open()
            }
            Action {
                text: qsTr("Save")
                shortcut: StandardKey.Save
                enabled: document.hasOpenFile
                onTriggered: {
                    if (document.modified) {
                        if (!document.save()) {
                            saveDialog.open()
                        }
                    }
                }
            }
            Action {
                text: qsTr("Save As…")
                shortcut: StandardKey.SaveAs
                enabled: document.hasOpenFile
                onTriggered: saveDialog.open()
            }
            MenuSeparator {}
            Action {
                text: qsTr("Close")
                shortcut: "Ctrl+W"
                enabled: document.hasOpenFile
                onTriggered: requestClose()
            }

            
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            Layout.fillWidth: true
            Label {
                leftPadding: 16
                text: document.hasOpenFile
                    ? (document.filePath + (document.modified ? "  •  " + qsTr("modified") : ""))
                    : qsTr("No file open")
                elide: Text.ElideMiddle
                Layout.fillWidth: true
            }
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            visible: document.hasOpenFile

            TabButton {
                text: document.fileName + (document.modified ? " *" : "")
                width: implicitWidth + 24
            }
        }

        EditorPane {
            Layout.fillWidth: true
            Layout.fillHeight: true
            document: root.document
            onPropertiesRequested: propertiesDialog.open()
        }

        Rectangle {
            Layout.fillWidth: true
            height: 28
            color: "#2d2d2d"

            Label {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                color: "#cccccc"
                text: document.hasOpenFile
                    ? qsTr("Editing: %1  |  Encoding: %2  |  Size: %3")
                        .arg(document.fileName)
                        .arg(document.encoding)
                        .arg(document.fileSize)
                    : qsTr("Ready")
            }
        }
    }

    Connections {
        target: document
        function onOpenFailed(message) {
            errorDialog.message = message
            errorDialog.open()
        }
        function onSaveFailed(message) {
            errorDialog.message = message
            errorDialog.open()
        }
    }
}

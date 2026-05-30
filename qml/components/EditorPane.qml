import QtQuick
import QtQuick.Controls
import TextEditor.Backend 1.0

Rectangle {
    id: root
    required property TextDocument document
    signal propertiesRequested()

    color: "#1e1e1e"

    ScrollView {
        anchors.fill: parent
        anchors.margins: 8
        clip: true

        TextArea {
            id: editor
            width: parent.width
            placeholderText: document.hasOpenFile
                ? ""
                : qsTr("No file open. Use File → Open to load a file.")
            color: "#f0f0f0"
            placeholderTextColor: "#888888"
            selectionColor: "#264f78"
            selectedTextColor: "#ffffff"
            wrapMode: TextArea.Wrap
            selectByMouse: true
            readOnly: !document.hasOpenFile
            font.family: "monospace"
            font.pixelSize: 13
            text: document.content

            onTextChanged: {
                if (document.hasOpenFile && document.content !== text) {
                    document.content = text
                }
            }

            TapHandler {
                acceptedButtons: Qt.RightButton
                onTapped: {
                    if (document.hasOpenFile) {
                        document.refreshMetadata()
                        root.propertiesRequested()
                    }
                }
            }
        }
    }

    Connections {
        target: document
        function onContentChanged() {
            if (editor.text !== document.content) {
                editor.text = document.content
            }
        }
        function onFilePathChanged() {
            if (!document.hasOpenFile) {
                editor.text = ""
            }
        }
    }
}

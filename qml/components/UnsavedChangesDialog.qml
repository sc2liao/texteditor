import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root

    property string fileName: ""
    modal: true
    title: qsTr("Unsaved changes")
    standardButtons: Dialog.NoButton
    width: 400

    signal saveAndContinue()
    signal discardAndContinue()
    signal cancelled()

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
        spacing: 12

        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("Save changes to \"%1\" before continuing?").arg(root.fileName)
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 8

            Button {
                text: qsTr("Save")
                onClicked: {
                    root.close()
                    root.saveAndContinue()
                }
            }
            Button {
                text: qsTr("Don't Save")
                onClicked: {
                    root.close()
                    root.discardAndContinue()
                }
            }
            Button {
                text: qsTr("Cancel")
                onClicked: {
                    root.close()
                    root.cancelled()
                }
            }
        }
    }
}

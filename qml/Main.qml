import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    title: qsTr("Text Editor")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton { text: qsTr("Untitled") }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            Rectangle {
                color: "#1e1e1e"

                TextArea {
                    anchors.fill: parent
                    anchors.margins: 12
                    placeholderText: qsTr("Bootstrap editor ready. Next: file I/O and context menu.")
                    color: "#ffffff"
                    wrapMode: TextArea.Wrap
                    selectByMouse: true
                }
            }
        }
    }
}

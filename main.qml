import QtQuick 2.10
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import AI 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TabBar {
        visible: false
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }
    TabBar {
        visible: false
        rotation: 90
        width: parent.height
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: (-width + height) / 2
        }

        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }
    property int nn: 0
    Timer {
        running: true
        repeat: true
        onTriggered: nn++
    }

    CppTest {
        anchors{
//            topMargin: 20
//            leftMargin: 20
//            rightMargin: 20
//            bottomMargin: 20
            fill: parent
        }
    }
}

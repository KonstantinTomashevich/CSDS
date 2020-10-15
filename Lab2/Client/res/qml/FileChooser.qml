import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Rectangle {
    anchors.fill: parent
    color: "azure"
    signal back()
    signal enter(string filename)

    Label {
        id: lblRequestFilename
        anchors {
            centerIn: parent
            topMargin: 5
        }
        text: qsTr("Enter filename:")
    }
    TextField {
        id: txtfldFilename
        anchors {
            top: lblRequestFilename.bottom;
            horizontalCenter: parent.horizontalCenter
            topMargin: 5
        }
    }
    Button {
        id: btnBack
        anchors {
            bottom: parent.bottom
            left: parent.left
            topMargin: 5
        }
        text: qsTr("Back")
        onClicked: back()
    }
    Button {
        id: btnEnter
        anchors {
            bottom: parent.bottom
            right: parent.right
            topMargin: 5
        }
        text: qsTr("Enter")
        onClicked: enter(txtfldFilename.text)
    }

} // Rectangle

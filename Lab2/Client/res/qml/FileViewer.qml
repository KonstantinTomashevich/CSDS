import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Rectangle {
    anchors.fill: parent
    color: "azure"

    signal back()

    function setFile(filename, text) {
        labelFilename.text = filename
        textAreaFileContents.text = text
    }

    Label {
        id: labelFilename
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 5
            bottomMargin: 5
        }
        text: qsTr("<FILENAME>")
    }

    ScrollView {
        id: scrollViewFileContents
        width: parent.width
        anchors {
            top: labelFilename.bottom;
            bottom: buttonBack.top
            margins: 5
        }
        Text{
            id: textAreaFileContents
            anchors.fill: parent
            text: "Lorem ipsum"
        }
    }
    Button {
        id: buttonBack
        width: parent.width
        anchors {
            bottom: parent.bottom
        }
        text: qsTr("Back")
        onClicked: back()
    }
} // Rectangle

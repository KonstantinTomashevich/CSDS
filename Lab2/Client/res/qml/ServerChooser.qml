import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Rectangle {
    anchors.fill: parent
    color: "azure"

    signal enter(string address, string port)

    Label {
        id: lblEnterServerAddress
        anchors {
            centerIn: parent
            margins: 5
        }
        text: qsTr("Enter server address:")
    }
    TextField {
        id: txtfldServerAddress
        anchors {
            top: lblEnterServerAddress.bottom;
            horizontalCenter: parent.horizontalCenter
            margins: 5
        }
        text: "127.0.0.1"
    }
    Label {
        id: lblEnterServerPort
        anchors {
            top: txtfldServerAddress.bottom;
            horizontalCenter: parent.horizontalCenter
            margins: 5
        }
        text: qsTr("Enter server port:")
    }
    TextField {
        id: txtfldEnterServerPort
        anchors {
            top: lblEnterServerPort.bottom;
            horizontalCenter: parent.horizontalCenter
            margins: 5
        }
        text: "1234"
    }
    Button {
        id: btnEnter
        width: parent.width
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        text: qsTr("Enter")
        onClicked: enter(txtfldServerAddress.text, txtfldEnterServerPort.text)
    }
} // Rectangle

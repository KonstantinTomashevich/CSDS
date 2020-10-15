import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Rectangle {
    anchors.fill: parent
    color: "azure"

    signal back()
    signal enter(string login, string password)

    Label {
        id: lblLogin
        anchors {
            centerIn: parent
            topMargin: 5
        }
        text: qsTr("Login:")
    }
    TextField {
        id: txtfldLogin
        anchors {
            top: lblLogin.bottom;
            horizontalCenter: parent.horizontalCenter
            topMargin: 5
        }
    }
    Label {
        id: lblPassword
        anchors {
            top: txtfldLogin.bottom;
            horizontalCenter: parent.horizontalCenter
            topMargin: 5
        }
        text: qsTr("Password:")
    }
    TextField {
        id: txtfldPassword
        echoMode: TextField.Password
        anchors {
            top: lblPassword.bottom;
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
        onClicked: enter(txtfldLogin.text, txtfldPassword.text)
    }

} // Rectangle

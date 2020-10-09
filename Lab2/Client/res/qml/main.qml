import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Client")
    id: root

    property string fileName
    property string text

    StackView {
        id: stack
        initialItem: serverChooser
        anchors.fill: parent
    }

    Component {
        id: serverChooser
        ServerChooser {
            onEnter: {
                var connectionResult = controller.connectToServer(address, port)
                if (connectionResult.isConnected())
                    stack.push(loginForm)
                else {
                    // TODO: PRINT ERROR MESSAGE
                    console.log(connectionResult.reason())
                }
            }
        }
    }
    Component {
        id: loginForm
        LoginForm {
            onBack: {
                stack.pop()
                controller.reset();
            }
            onEnter: {
                var authenticationResult = controller.authenticate(login, password)
                if (authenticationResult.isAuthenticated())
                    stack.push(fileChooser)
                else {
                    // TODO: PRINT ERROR MESSAGE
                    console.log(authenticationResult.reason())
                }
            }
        }
    }
    Component {
        id: fileChooser
        FileChooser {
            onBack: {
                stack.pop()
                stack.pop()
                controller.reset();
            }
            onEnter: {
                var loadedFile = controller.loadFile(filename)
                if (loadedFile.isLoaded()) {
                    root.fileName = loadedFile.filename()
                    root.text = loadedFile.text()
                    stack.push(fileViewer)
                }
                else {
                    console.log(loadedFile.reason())
                    // TODO: PRINT ERROR MESSAGE
                }
            }
        }
    }
    Component {
        id: fileViewer
        FileViewer {
            id: fileViewerObj
            Component.onCompleted: {
                fileViewerObj.setFile(fileName, text)
            }
            onBack: {
                stack.pop()
            }
        }
    }
} // Window

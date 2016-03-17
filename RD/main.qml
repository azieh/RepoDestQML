import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    visible: true
    maximumHeight: 1000
    maximumWidth: 1000
    minimumHeight: 400
    minimumWidth: 500
    title: qsTr("Raportowanie przestojów")
    color: "#444444"
    Connections{
        target: mainWindow
        onTextUpdate:{
            if ( logModel.count == 13 ){
                logModel.remove(0,1)
            }
            logInput.text = stName + ": " +text
            logModel.append({score: logInput.text})
        }
        onPcsUpdate:{
            pcsName.text = "PCS database path: " + text
        }
        onApuUpdate:{
            apuName.text = "APU database path: " + text
        }
    }
    GridLayout {
        id: gridLayoutFirst
        width: 250
        height: 250
        objectName: "gridLayoutFirst"
    }

    Flow {
        id: gridLayoutRest
        x: 250
        width: 250
        height: 250

        spacing: 1.5
        objectName: "gridLayoutRest"

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }
        move: Transition {
            SequentialAnimation {
                PauseAnimation {
                    duration: (gridLayoutRest.ViewTransition.index -
                               gridLayoutRest.ViewTransition.targetIndexes[0]) * 100
                }
                ParallelAnimation {
                    NumberAnimation {
                        property: "x"; to: gridLayoutRest.ViewTransition.item.x + 20
                        easing.type: Easing.OutQuad
                    }
                    NumberAnimation {
                        property: "y"; to: gridLayoutRest.ViewTransition.item.y + 50
                        easing.type: Easing.OutQuad
                    }
                }
                NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.OutBounce }
            }
        }

    }

    Rectangle{
        id: textArea
        y: 250
        color: "#292929"
        width: 500
        height: 150
        ColumnLayout{
            anchors.fill: parent
            Layout.alignment: Qt.AlignCenter
            spacing: 0
            Text{
                id: areaTextInfo
                color: "#747474"
                font.pointSize: 7
                text: "Log/message window:"
            }
            Component {
                id: logDelegate
                Item {
                    width: 500; height: 10
                    Text {
                        anchors.fill: parent
                        text: score
                        color: "white"
                    }
                }
            }
            Text {
                id: logInput
                visible: false
                text: "Log/message"
            }
            ListView {
                id: logWindow
                model: logModel
                delegate: logDelegate
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ListModel {
                id: logModel
                ListElement { score: " " }
            }
            states: [
                State { when: textAreaStateVisible;
                    PropertyChanges {   target: textArea; opacity: 1.0; visible: true   }
                },
                State { when: !textAreaStateVisible;
                    PropertyChanges {   target: textArea; opacity: 0.0; visible: false  }
                }
            ]
            transitions: Transition {
                NumberAnimation { property: "opacity"; duration: 300 }
            }

        }
        Text{
            id: apuName
            anchors.right: textArea.right
            anchors.bottom: textArea.bottom
            Layout.column: 1
            color: "#747474"
            font.pointSize: 7
            text: "APU database path: is not currently available"
        }
        Text{
            id: pcsName
            anchors.right: textArea.right
            anchors.bottom: apuName.top
            Layout.column: 1
            color: "#747474"
            font.pointSize: 7
            text: "PCS database path: is not currently available"
        }
    }



}


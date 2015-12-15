import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Item {
    width: 380
    height: 240

    Connections{
        id: cppConnection
        target:clientGuiConnection


        onLoopTime: {
            timeText.text = text
        }
    }

    Rectangle {
        id: mainBackground
        color: "#000000"
        border.color: "white"
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.fill: parent

        Rectangle {
            id: topBar
            x: 0
            y: 0
            width: 380
            height: 55
            gradient: Gradient {
                GradientStop {
                    position: 0.058
                    color: "#3b3b3b"
                }

                GradientStop {
                    position: 0.517
                    color: "#000000"
                }

                GradientStop {
                    position: 0.966
                    color: "#3b3b3b"
                }
            }

            Rectangle {
                id: timeBar
                border.color: "darkgrey"
                anchors.left: topBar.left
                y: 1
                width: topBar.width /4
                height: topBar.height
                color: "#00000000"
                opacity: 1
                Column {
                    id:timeColumn
                    anchors.top: timeBar.top
                    anchors.left: timeBar.left
                    spacing: 7
                    anchors.leftMargin: 10
                    anchors.topMargin: 3
                    anchors.horizontalCenter: timeBar.horizontalCenter
                    anchors.verticalCenter: timeBar.verticalCenter
                    Text {
                        id: timeStaticText
                        anchors.horizontalCenter: timeColumn.horizontalCenter
                        font.pointSize: 10
                        color: "darkorange"
                        text: "Cycle time:"
                    }
                    Text {
                        id: timeText
                        anchors.horizontalCenter: timeColumn.horizontalCenter
                        font.pointSize: 10
                        color: "white"
                        text: asdasd
                    }
                }
            }

            Rectangle {
                id: okBar
                border.color: "darkgrey"
                anchors.left: timeBar.right

                y: 1
                width: topBar.width /4
                height: topBar.height
                color: "#00000000"
                opacity: 1

                Column {
                    id:okColumn
                    anchors.top: okBar.top
                    anchors.left: okBar.left
                    spacing: 10
                    anchors.leftMargin: 10
                    anchors.topMargin: 3
                    anchors.horizontalCenter: okBar.horizontalCenter
                    anchors.verticalCenter: okBar.verticalCenter
                    Text {
                        id: okStaticText
                        anchors.horizontalCenter: okColumn.horizontalCenter
                        font.pointSize: 8
                        color: "darkorange"
                        text: "Operations OK"
                    }
                    Text {
                        id: okText
                        anchors.horizontalCenter: okColumn.horizontalCenter
                        font.pointSize: 10
                        color: "white"
                        text:  qsTr("9999")
                    }
                }

                Rectangle {
                    id: nokBar
                    border.color: "darkgrey"
                    anchors.left: okBar.right
                    y: 0
                    width: topBar.width /4
                    height: topBar.height
                    color: "#00000000"
                    anchors.leftMargin: 0
                    opacity: 1

                    Column {
                        id:nokColumn
                        anchors.top: nokBar.top
                        anchors.left: nokBar.left
                        spacing: 10
                        anchors.leftMargin: 10
                        anchors.topMargin: 3
                        anchors.horizontalCenter: nokBar.horizontalCenter
                        anchors.verticalCenter: nokBar.verticalCenter
                        Text {
                            id: nokStaticText
                            anchors.horizontalCenter: nokColumn.horizontalCenter
                            font.pointSize: 8
                            color: "darkorange"
                            text: "Operations NOK"
                        }
                        Text {
                            id: nokText
                            anchors.horizontalCenter: nokColumn.horizontalCenter
                            font.pointSize: 10
                            color: "white"
                            text:  qsTr("9999")
                        }
                    }
                    Rectangle {
                        id: statusBar
                        border.color: "darkgrey"
                        anchors.left: nokBar.right
                        y: 0
                        width: topBar.width /4
                        height: topBar.height
                        color: "#00000000"
                        anchors.leftMargin: 0
                        opacity: 1


                        Text {
                            id: statusText
                            anchors.top: statusBar.top
                            anchors.left: statusBar.left
                            anchors.topMargin: 5
                            anchors.leftMargin: 5
                            color: "darkorange"
                            text:  qsTr("Disconnected")
                        }

                    }

                    Image {
                        id: statusImg
                        anchors.rightMargin: 2
                        anchors.bottomMargin: 2
                        anchors.bottom: statusBar.bottom
                        anchors.right: statusBar.right
                        opacity: 1
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/img/disconnected.png"
                    }
                }
            }
        }
    }
}

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

Item {
    id: main
    width: 250
    height: 250

    Connections{
        id: cppConnection
        target:clientGuiConnection


        onLoopTime: {

        }
    }


    Rectangle {
        id: rectangle1
        color: "#000000"
        anchors.fill: parent
    }

    GridLayout {
        id: gridLayout1
        anchors.fill: parent
        rowSpacing: 0


        GridLayout {
            id: leftBarGrid1

            anchors.fill: parent
            Layout.fillHeight: false
            Layout.fillWidth: false
            rowSpacing: 0


            Rectangle {
                id: leftBarNameRect
                Layout.alignment: Qt.AlignTop
                Layout.column: 1
                Layout.row: 0
                width: main.width /5
                height: main.height /5
                color: "#b43535"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        leftBarGrid2.visible = !leftBarGrid2.visible
                        textArea.visible = !textArea.visible
                    }
                }
            }
            Grid {
                id: leftBarGrid2
                columns: 1
                Layout.column: 1
                Layout.row: 1
                populate: Transition {
                    NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
                }
                Rectangle {
                    id: leftBarStatusRect
                    width: main.width /5
                    height: main.height /5
                    color: "#dcb1b1"
                }

                Rectangle {
                    id: leftBarTimeRect
                    width: main.width /5
                    height: main.height /5
                    color: "#330303"
                }

                Rectangle {
                    id: leftBarOkRect
                    width: main.width /5
                    height: main.height /5
                    color: "#6677a2"
                }

                Rectangle {
                    id: leftBarNokRect
                    width: main.width /5
                    height: main.height /5
                    color: "#8bbc23"
                }
            }

        }

        TextArea {
            id: textArea
            text: "elo Panie/Panowie...\n\nCzy mógłby ktoś odesłać mnie do jakiejś lektury bądź przedstawić w jaki sposób wprowadzić adres IP i go wyświetlić? Mianowicie chodzi mi o coś takiego\n\nwprowadzam ip w formacie 192.168.1.1 i w takim samym mi wyświetla zapisując go pod zmienną którą dajmy na to nazwiemy IP by móc później operować tym adresem w celu np zmiany na adres binarny"
            textColor: "#ffffff"
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            backgroundVisible: false
        }
    }


}

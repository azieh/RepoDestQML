import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1



GridLayout {
    id: mainGridLayout
    columnSpacing: 0
    rowSpacing: 0
    Layout.maximumWidth: 250
    Layout.maximumHeight: 250
    Layout.minimumWidth: 50
    Layout.minimumHeight: 50

    transitions: Transition {
        NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
    }

    property bool textAreaStateVisible: false
    property bool leftBarGridStateVisible: false

    GridLayout {
        id: leftMainBarGrid
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillHeight: true

        Layout.maximumWidth: 50
        rowSpacing: 0


        Rectangle {
            id: leftBarNameRect
            Layout.alignment: Qt.AlignTop
            Layout.column: 1
            Layout.row: 0
            width: 50
            height: 50
            color: "#b43535"

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: {
                    textAreaStateVisible = !textAreaStateVisible
                    leftBarGridStateVisible = !leftBarGridStateVisible
                }
            }
        }
        Grid {
            id: leftBarGrid
            columns: 1
            Layout.column: 1
            Layout.row: 1
            states: [
                State { when: leftBarGridStateVisible;
                    PropertyChanges {   target: leftBarGrid; opacity: 1.0; visible: true   }
                },
                State { when: !leftBarGridStateVisible;
                    PropertyChanges {   target: leftBarGrid; opacity: 0.0; visible: false  }
                }
            ]
            transitions: Transition {
                NumberAnimation { property: "opacity"; duration: 200 }

            }
            Rectangle {
                id: leftBarStatusRect
                width: 50
                height: 50
                color: "#dcb1b1"
            }

            Rectangle {
                id: leftBarTimeRect
                width: 50
                height: 50
                color: "#330303"
            }

            Rectangle {
                id: leftBarOkRect
                width: 50
                height: 50
                color: "#6677a2"
            }

            Rectangle {
                id: leftBarNokRect
                width: 50
                height: 50
                color: "#8bbc23"
            }
        }

    }
    Rectangle{
        id: textAreaRect
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.maximumWidth: 200
        color: "black"
        height: 250
        width: 200
        Layout.column: 2
        Layout.row: 0



        states: [
            State { when: textAreaStateVisible;
                PropertyChanges {   target: textArea; opacity: 1.0; visible: true   }
                PropertyChanges {   target: textAreaRect; opacity: 1.0; visible: true   }
            },
            State { when: !textAreaStateVisible;
                PropertyChanges {   target: textArea; opacity: 0.0; visible: false  }
                PropertyChanges {   target: textAreaRect; opacity: 1.0; visible: false   }
            }
        ]
        transitions: Transition {
            NumberAnimation { property: "opacity"; duration: 300 }
        }

        TextArea {
            id: textArea

            anchors.fill: parent
            textColor: "#ffffff"
            font { pointSize: 6; family: "Tahoma" }


            backgroundVisible: false



            text: "elo Panie/Panowie...\n\nCzy mógłby ktoś odesłać mnie do jakiejś lektury bądź przedstawić w jaki sposób wprowadzić adres IP i go wyświetlić? Mianowicie chodzi mi o coś takiego\n\nwprowadzam ip w formacie 192.168.1.1 i w takim samym mi wyświetla zapisując go pod zmienną którą dajmy na to nazwiemy IP by móc później operować tym adresem w celu np zmiany na adres binarny"

        }

    }
}



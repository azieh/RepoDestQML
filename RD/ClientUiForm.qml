import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0


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

    property bool textAreaStateVisible: true
    property bool leftBarGridStateVisible: true

    GridLayout {
        id: leftMainBarGrid
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillHeight: true

        Layout.maximumWidth: 50
        rowSpacing: 0

        Item{
            id: leftBarName
            width: 50
            height: 50
            Layout.alignment: Qt.AlignTop
            Layout.column: 1
            Layout.row: 0
            Rectangle {
                id: leftBarNameRect
                anchors.fill: parent
                color: "#828282"
                border.width: 0


            }
            InnerShadow {
                color: "#505050"
                anchors.fill: parent
                cached: true
                horizontalOffset: 0
                verticalOffset: 0
                radius: 32
                fast: true
                spread: 0.5
                samples: 32
                smooth: true
                source: parent
            }
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
                color: "#505050"
                border.color: "#ffffff"
                border.width: 1
            }

            Rectangle {
                id: leftBarTimeRect
                width: 50
                height: 50
                color: "#505050"
                border.color: "#ffffff"
                border.width: 1
            }

            Rectangle {
                id: leftBarOkRect
                width: 50
                height: 50
                color: "#505050"
                radius: 1
                border.color: "#ffffff"
                border.width: 1
            }

            Rectangle {
                id: leftBarNokRect
                width: 50
                height: 50
                color: "#505050"
                radius: 0
                border.color: "#ffffff"
                rotation: 0
                border.width: 1
            }
        }

    }






    TextArea {
        id: textArea
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.column: 2
        Layout.row: 0


        font { pointSize: 6; family: "Tahoma" }

        style: TextAreaStyle {
            backgroundColor : "#505050"

            textColor: "#ffffff"
            renderType: Text.NativeRendering

        }

        text: "elo Panie/Panowie...\n\nCzy mógłby ktoś odesłać mnie do jakiejś lektury bądź przedstawić w jaki sposób wprowadzić adres IP i go wyświetlić? Mianowicie chodzi mi o coś takiego\n\nwprowadzam ip w formacie 192.168.1.1 i w takim samym mi wyświetla zapisując go pod zmienną którą dajmy na to nazwiemy IP by móc później operować tym adresem w celu np zmiany na adres binarny"
        readOnly: true

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

}




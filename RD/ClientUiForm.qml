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
    Connections{
        target: clientWindow
        onLoopTimeUpdate:{
            timeText.text = text
            gc()
        }
        onOkUpdate:{
            okText.text = text
        }
        onNokUpdate:{
            nokText.text = text
        }
        onTextUpdate:{
            if (p1model.count == 22){
               p1model.remove(0,1)
            }

            p1input.text = text
            p1model.append({score: p1input.text})
        }
        onStationNameUpdate:{
            nameText.text = text
        }
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
                color: "#747474"
                border.color: "#292929"
                border.width: 1
            }
            InnerShadow {
                color: "#505050"
                anchors.fill: leftBarNameRect
                horizontalOffset: 4.3
                verticalOffset: 4.1
                radius: 9.9
                cached: true
                spread: 0.7
                fast: true
                samples: 8
                source: leftBarNameRect
            }
            Text{
                id:nameText
                rotation: 45
                font.family: "Arial"
                font.bold: true
                font.pointSize: 14
                color: "#ffffff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                id: mouseArea
                hoverEnabled: false
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
                color: "#747474"
                border.color: "#292929"
                border.width: 1
                ColumnLayout{
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    Text{
                        color: "#ffffff"
                        text: "Status"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Text{
                        id:statusText
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Rectangle {
                id: leftBarTimeRect
                width: 50
                height: 50
                color: "#747474"
                border.color: "#292929"
                border.width: 1
                ColumnLayout{
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    Text{
                        color: "#ffffff"
                        text: "1L time"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Text{
                        id:timeText
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Rectangle {
                id: leftBarOkRect
                width: 50
                height: 50
                color: "#747474"
                border.color: "#292929"
                border.width: 1
                ColumnLayout{
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    Text{
                        color: "#ffffff"
                        text: "OK:"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Text{
                        id:okText
                        text: "0"
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Rectangle {
                id: leftBarNokRect
                width: 50
                height: 50
                color: "#747474"
                border.color: "#292929"
                border.width: 1
                ColumnLayout{
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    Text{
                        color: "#ffffff"
                        text: "NOK"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Text{
                        id:nokText
                        color: "#ffffff"
                        text: "0"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
    Rectangle{
        id: textArea
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.column: 1
        Layout.row: 0
        color: "#292929"
        width: 200
        height: 250



        Component {
            id: delegate
            Item {
                width: 200; height: 10
                Label {
                    text: score
                }
            }
        }

        Text {
            id: p1input
            visible: false
            text: "Log/message"


        }

        ListView {
            id: p1scores
            model: p1model
            delegate: delegate
            anchors.fill: parent
        }

        ListModel {
            id: p1model
            ListElement { score: "" }
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
}



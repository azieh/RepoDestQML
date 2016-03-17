import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

import QtGraphicalEffects 1.0

GridLayout {
    id: mainGridLayout
    columnSpacing: 0
    rowSpacing: 0
    Layout.maximumWidth: 500
    Layout.maximumHeight: 500
    Layout.minimumWidth: 50
    Layout.minimumHeight: 50

    transitions: Transition {
        NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad }
    }
    Connections{
        target: clientWindow

        onHideClientWindow:{
            textAreaStateVisible = false
            leftBarGridStateVisible = false
            leftBarNameInnerShadow.visible = false
        }
        onStationNameUpdate:{
            nameText.text = text
        }
        onConnectionStatusUpdate:{
            connectedImage.visible      = b
            disconnectedImage.visible   = !b
        }
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
            if ( logModel.count == 23 ){
                logModel.remove(0,1)
            }
            logInput.text = text
            logModel.append({score: logInput.text})
        }
        onIpUpdate:{
            ip.text = "Ip: " + text
        }
        onDbUpdate:{
            db.text = "DB" + text
        }
    }

    property bool textAreaStateVisible: false
    property bool leftBarGridStateVisible: false
    signal clientWindowWasClicked(bool x)

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
                id: leftBarNameInnerShadow
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
                visible: textAreaStateVisible
            }
            InnerShadow {
                id: leftBarNameMouseInnerShadow
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
                visible: false
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
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    textAreaStateVisible = !textAreaStateVisible
                    leftBarGridStateVisible = !leftBarGridStateVisible
                    leftBarNameInnerShadow.visible = textAreaStateVisible
                    clientWindowWasClicked( true )
                }
                onEntered: {
                    leftBarNameMouseInnerShadow.visible = true
                }
                onExited: {
                    leftBarNameMouseInnerShadow.visible = false
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
                    spacing: 0
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignCenter
                    Text{
                        color: "#ffffff"
                        text: "Status"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Image {
                        id: disconnectedImage
                        fillMode: Image.Stretch
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "ico/disconnected.png"
                        visible: true
                    }
                    Image {
                        id: connectedImage
                        fillMode: Image.Stretch
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "ico/connected.png"
                        visible: false
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
                        text: "Time"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.topMargin: 1
                    }
                    Text{
                        id:timeText
                        text: "9999ms"
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
                width: 200; height: 10
                Label {
                    text: score
                    color: "white"
                }
            }
        }
        TextArea {
            id: logInput
            visible: false
            text: "Log/message"
            width: 200
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
        Text{
            id: db
            anchors.right: textArea.right
            anchors.bottom: textArea.bottom
            Layout.column: 1
            Layout.row: 0
            color: "#747474"
            font.pointSize: 7
            text: "DB501"
        }
        Text{
            id: ip
            anchors.right: textArea.right
            anchors.bottom: db.top
            Layout.column: 1
            Layout.row: 0
            color: "#747474"
            font.pointSize: 7
            text: "Ip:192.168.1.1"
        }
    }
}



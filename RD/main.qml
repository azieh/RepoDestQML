import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    title: qsTr("Raportowanie przestojów")
    color: "#292929"

    GridLayout {
        id: gridLayout
        rows: 10
        columns: 5
        columnSpacing: 1
        rowSpacing: 1
        objectName: "gridLayout"
        anchors.fill: parent
    }
}


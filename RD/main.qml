import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 400
    title: qsTr("Raportowanie przestojów")
    color: "#444444"

    Flow {
        id: gridLayout
        spacing: 1.5
        objectName: "gridLayout"
        anchors.fill: parent
        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }

        move: Transition {
            SequentialAnimation {
                PauseAnimation {
                    duration: (gridLayout.ViewTransition.index -
                               gridLayout.ViewTransition.targetIndexes[0]) * 100
                }
                ParallelAnimation {
                    NumberAnimation {
                        property: "x"; to: gridLayout.ViewTransition.item.x + 20
                        easing.type: Easing.OutQuad
                    }
                    NumberAnimation {
                        property: "y"; to: gridLayout.ViewTransition.item.y + 50
                        easing.type: Easing.OutQuad
                    }
                }
                NumberAnimation { properties: "x,y"; duration: 500; easing.type: Easing.OutBounce }
            }
        }
    }
}


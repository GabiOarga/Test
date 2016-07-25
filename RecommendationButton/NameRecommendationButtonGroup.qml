import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.2

Item {
    id: buttonGroupContainer

    property real groupWidth
    property real groupHeight /* Not implemented in forumlas */
    property real groupPaddingLeft
    property real groupPaddingRight
    property real groupPaddingTop
    property real groupPaddingBottom
    property real groupSpacing
    property real buttonSizeActual: 0 /* Do not define */

    /* 10 char limit */
    property string buttonOneName
    property string buttonTwoName
    property string buttonThreeName

    Component.onCompleted: {
        worker.sendMessage({"GW": groupWidth,
                               "GH": groupHeight,
                               "PL": groupPaddingLeft,
                               "PR": groupPaddingRight,
                               "GS": groupSpacing})
    }

    Behavior on buttonSizeActual {
        NumberAnimation {
            duration: 600
            easing.type: Easing.OutBounce
        }
    }

    ButtonGroup {
        id: buttonGroupHandler
        buttons: buttonRow.children

        onCheckedButtonChanged: {
            if (button1.checked) {
                button2.checked = false
                button3.checked = false
            } else if (button2.checked) {
                button1.checked = false
                button3.checked = false
            } else if (button3.checked){
                button1.checked = false
                button2.checked = false
            }
        }
    }

    Row {
        id: buttonRow

        spacing: groupSpacing
        leftPadding: groupPaddingLeft
        rightPadding: groupPaddingRight
        topPadding: groupPaddingTop
        bottomPadding: groupPaddingBottom

        NameRecommendationButton {
            id: button1
            size: buttonSizeActual
            buttonName: buttonOneName
        }
        NameRecommendationButton {
            id: button2
            size: buttonSizeActual
            buttonName: buttonTwoName
        }
        NameRecommendationButton {
            id: button3
            size: buttonSizeActual
            buttonName: buttonThreeName
        }
    }

    WorkerScript {
        id: worker
        source: "./TestWorkerScript.js"

        onMessage: {
            buttonSizeActual = messageObject.getSize
        }
    }
}

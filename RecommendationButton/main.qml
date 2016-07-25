import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2

Window {
    id: window
    visible: true
    width: 600
    height: 880
    title: qsTr("Hello World")
    color: "#eee"

    NameRecommendationButtonGroup {
        groupHeight: parent.height * 0.1
        groupWidth: parent.width
        groupSpacing: 20
        buttonOneName: "pavel.anthone"
        buttonTwoName: "pavel25"
        buttonThreeName: "anthonepav"
    }
}

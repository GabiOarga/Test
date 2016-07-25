import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.2

//TODO Animation
//#1: scale up from x to y when loaded
//2#: bounce on clicked
//#3: fill with color

Button {
    id: buttonRoot

    property real size
    property real scale: 1.35
    property real scaleCorrection: 1.21
    property real horizontalOffset
    property real verticalOffset
    property string fillColor: "#ffffff"
    property string fontUrl
    property string buttonName

    checkable: true
    width: size + horizontalOffset
    height: size / buttonRoot.scale / buttonRoot.scaleCorrection + verticalOffset

    background: Canvas {
        id: buttonCanvas

        width: size
        height: size / buttonRoot.scale / buttonRoot.scaleCorrection
        x: horizontalOffset
        y: verticalOffset

        onPaint: {
            var cSize = size * 0.825
            var BEH = cSize
            var BEV = cSize / buttonRoot.scale
            var SEH = BEH * 0.48
            var SEV = SEH / buttonRoot.scale

            var ctx = getContext("2d")
            ctx.clearRect(horizontalOffset, verticalOffset, size, size / buttonRoot.scale)
            ctx.fillStyle = fillColor

            ctx.beginPath()
            ctx.moveTo(0, 0)
            ctx.ellipse(0, 0, BEH, BEV)
            ctx.ellipse((BEH * 0.5 - SEH * 0.5) * 2.81,
                        (BEV * 0.5 - SEV * 0.5) * 0.7,
                        SEH, SEV)
            ctx.closePath()
            ctx.fill()
        }

        Text {
            id: buttonText

            text: buttonName
            anchors.centerIn: parent
            bottomPadding: parent.height * 0.12
            font.pixelSize: parent.height * 0.24 * ((parent.height - 4) / parent.height)
            font.family: font.name
            color: "#b2b2b2"

            FontLoader { id: font; source: fontUrl }
        }
    }

    onClicked: { buttonRoot.checked = true }

    onCheckedChanged: {
        if(fillColor == "#ffffff") {
            fillColor = "#1bbc9b"
            buttonText.color = "#ffffff"
        } else {
            fillColor = "#ffffff"
            buttonText.color = "#b2b2b2"
        }
        buttonCanvas.requestPaint()
    }
}






















import QtQuick 2.15
import QtQuick.Window
import QtQuick.Controls

Window
{
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button{
        id: button1
       text: "Click me"
       anchors.centerIn:parent
       onCanceled: {
           win.color = Qt.rgba(Math.random(), Math.random(), Math.random());
       }
    }
}

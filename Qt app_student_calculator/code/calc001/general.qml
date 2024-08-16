import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Fusion 2.15


Rectangle {
id: win
color: "#f5efff"

Label {
       id: label1
       text: "Hello"
       font.family: "Ink Free"
       font.pixelSize: 24
       font.italic: true
       anchors.top: parent.top
       anchors.topMargin: 80
       anchors.horizontalCenter: parent.horizontalCenter
   }

   Label {
       id: label2
       text: "<--------choose a category"
       font.family: "Ink Free"
       font.pixelSize: 18
       anchors.top: label1.bottom
       anchors.horizontalCenter: parent.horizontalCenter
   }
}


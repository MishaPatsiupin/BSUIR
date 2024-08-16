import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls.Fusion 2.15

Rectangle {
id: win
color: "#f5efff"


Label {
       id: label1
       text: "Курсовая работа"
//       font.family: "Times new roman"
       font.pixelSize: 24
       font.italic: true
       anchors.top: parent.top
       anchors.topMargin: 80
       anchors.horizontalCenter: parent.horizontalCenter
   }

Label {
    id: label2
    text: "Патюпин М.С."
//       font.family: "Times new roman"
    font.pixelSize: 18
    anchors.top: label1.bottom
    anchors.horizontalCenter: parent.horizontalCenter
}

   Label {
       id: label3
       text: "Богдан Е.В."
 //      font.family: "Times new roman"
       font.pixelSize: 18
       anchors.top: label2.bottom
       anchors.horizontalCenter: parent.horizontalCenter
   }

   Label {
       id: label4
       text: "Минск, 2024"
//       font.family: "Times new roman"
       font.pixelSize: 12
       anchors.top: label3.bottom
       anchors.horizontalCenter: parent.horizontalCenter
   }

}

import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import QtQuick.Controls.Fusion 2.15

Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent

    property int size1: 0
    property var buf1: []
    property int size2: 0
    property var buf2: []
    property int len
    property var fmin: []
    property int chek_input: 0

    Connections {
       target: rot_main
        function onSend_f(data){
            fmin = data
        }
   }

    function isBinaryString(str) {
        for (let i = 0; i < str.length; i++) {
            if (str.charAt(i) !== '0' && str.charAt(i) !== '1' && str.charAt(i) !== 'x') {
                return false;
            }
        }
        return true;
    }

    Column {
        anchors.centerIn: parent
        width: parent.width * 0.8
        spacing: 10

        Label {
            text: "Введите исходные данные"
        }

        TextField {
            id: sizeField1
            width: parent.width
            placeholderText: "Введите количество наборов L:"
            onTextChanged: size1 = parseInt(text)
        }

        Repeater {
            model: size1
            TextField {
                id: textField1
                width: parent.width
                placeholderText: "Введите " + (index + 1) + " набор: "
                onTextChanged: {
                    buf1[index] = text
                    len = buf1[0].length
                    if ((index >= 0 && text.length !== len) || !isBinaryString(text)) {
                        textField1.color = "red"
                    } else {
                        textField1.color = "black"
                    }
                }
            }
        }

        TextField {
            id: sizeField2
            width: parent.width
            placeholderText: "Введите количество наборов N:"
            onTextChanged: size2 = parseInt(text)
        }

        Repeater {
            model: size2
            TextField {
                id: textField2
                width: parent.width
                placeholderText: "Введите " + (index + 1) + " набор: "
                onTextChanged: {
                    buf2[index] = text
                    if (text.length !== len || !isBinaryString(text)) {
                        textField2.color = "red"
                    } else {
                        textField2.color = "black"
                    }
                }
            }
        }

        Button {
            width: parent.width
            text: "Отправить"
            onClicked: {
                console.log(buf1)
                console.log(buf2)
                rot_main.input_L_N(buf1, buf2)
                rot_main.action_rot()
                rot_main.prepare_f()
               console.log(fmin)
                chek_input = 1

            }
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
            text: "Fмднф = " + fmin
            visible: chek_input === 1
        }

    }
}

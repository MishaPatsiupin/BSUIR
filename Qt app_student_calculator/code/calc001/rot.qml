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
    property int check_correct_input: 0

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
    ScrollView {
        anchors.fill: parent

        width: parent.width * 0.8
        contentWidth: Math.max(width, childrenRect.width)

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
//            onTextChanged: size1 = parseInt(text)
            onTextChanged: {
                var value = parseInt(text);
                if (value > 20) {
                    value = 20;
                    text = "20";
                }
                if (value < 0){
                    value = 0;
                    text = "0";
                }

                size1 = value;
            }
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
                        check_correct_input = 1
                    } else {
                        textField1.color = "black"
                        check_correct_input = 0
                    }
                }
            }
        }

        TextField {
            id: sizeField2
            width: parent.width
            placeholderText: "Введите количество наборов N:"
            onTextChanged: {
                var value = parseInt(text);
                if (value > 20) {
                    value = 20;
                    text = "20";
                }
                if (value < 0){
                    value = 0;
                    text = "0";
                }

                size2 = value;
            }
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
                        check_correct_input = 1
                    } else {
                        textField2.color = "black"
                        check_correct_input = 0
                    }
                }
            }
        }

        Button {

            width: parent.width
            text: "Отправить"
            onClicked: {
                console.log(buf1.length)
                console.log(buf2.length)

                if (check_correct_input === 0 && (buf1.length > 0 || buf2.length > 0)) {
                console.log(buf1)
                console.log(buf2)
                rot_main.input_L_N(buf1, buf2)
                rot_main.action_rot()
                rot_main.prepare_f()
               console.log(fmin)
                chek_input = 1

                lebel_result.text = "Fмднф = " + fmin
                }else
                    console.log("Неправильно введены исходные данные для алгоритма рота")
                        }
                }

        Label {
            id: lebel_result
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
            text: "Fмднф = " + fmin
            visible: chek_input === 1
        }

    }
}
}

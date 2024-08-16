import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Fusion 2.15
//внедрил очиску, отачил получение данных до идеала
Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent
property int cols1: 3
    property int rows1: 0
    property var matrix1: []

    property var matrixData: []

    property var labels: ["i", "j", "k"]

    property int chek_input_size: 0
 property int chek_add_size: 0

    Connections {
        target: duo_vec
        function onSendVec(data){
            matrixData = data
        }
    }

    ScrollView {
        anchors.fill: parent

        width: parent.width * 0.8
        contentWidth: Math.max(width, childrenRect.width)

        Column {
            width: parent.width
            spacing: 10

            Label {
                font.pixelSize: 13
                text: "Проверка на коллинеарность/ортагональность"
            }


            TextField {
                id: equationsField
                width: parent.width * 0.8
    anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Количество векторов: "
                onTextChanged: {
                    var value = parseInt(text);
                    if (value > 10) {
                        value = 10;
                        text = "10";
                    }
                    if (value < 2){
                        value = 2;
                        text = "2";
                    }
                    rows1 = value
                    matrix1.splice(0, matrix1.length);
                    for (var i = 0; i < (3*rows1); i++) {
                            matrix1.push(0)
                    }
                    matrixData.push(0)

                }
            }
            Repeater {
                model: rows1
                delegate: Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    property int rowIndex: index
                    Repeater {
                        model: cols1
                        TextField {
                            id: cellField1
                            width: 45
                            height: 45
                            placeholderText: labels[index] + (rowIndex + 1)
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            font.pixelSize: 20
                            color: "black"

                            onTextChanged: {

                                if (text === "") {
                                    matrix1[rowIndex * cols1 + index] = 0;
                                } else {
                                    matrix1[rowIndex * cols1 + index] = text;
                                }
                            }
                        }
                    }
                }
            }

            Button {
                id: submitButton
                text: "Проверить"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.4
                onClicked: {
                    duo_vec.input_vec_max(matrix1)
                    console.log("Вектора: " + matrix1)


                    duo_vec.action_kol()
                    chek_add_size = 1
                    console.log("Коллинеарны/ортаганальны: " + matrixData)
                }
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                text: if(matrixData[0] ===1){"Вектора коллинеарны"}else{"Вектора не коллинеарны"}
                visible: chek_add_size === 1
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                text: if(matrixData[1] ===1){"Вектора ортаганальны"}else{"Вектора не ортаганальны"}
                visible: chek_add_size === 1
            }
        }
    }
}


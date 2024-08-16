import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Fusion 2.15
//внедрил очиску, отачил получение данных до идеала
Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent
property int cols1: 3
    property int rows1: 2
    property var matrix1: [0, 0, 0]
    property var matrix2: [0, 0, 0]

    property var matrixData: [0, 0, 0]

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
                text: "Введите вектора:"
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
                                    if(rowIndex === 0){
                                    matrix1[index] = 0;
                                    }
                                    else{
                                        matrix2[index] = 0;
                                    }
                                } else {
                                    if(rowIndex === 0){
                                    matrix1[index] = text;
                                    }
                                    else{
                                        matrix2[index] = text;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Button {
                id: submitButton
                text: "Вычислить"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.4
                onClicked: {
                    duo_vec.input_vec1(matrix1)
                    duo_vec.input_vec2(matrix2)
                    console.log("Вектор1: " + matrix1)
console.log("Вектор2: " + matrix2)

                    duo_vec.action_vec()
                    chek_add_size = 1
                    console.log("Векторное произведение векторов: " + matrixData)
                }
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                text: "Векторное произвидение = {" + matrixData.join("; ") + "}"
                visible: chek_add_size === 1
            }
        }
    }
}



import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Fusion 2.15

Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent

    property int cols1: 0
    property int rows1: 0
    property var matrix1: []

    property var matrixData: []
    property int matrixCols
    property int matrixRows

    property int chek_input_size: 0
 property int chek_add_size: 0

    Connections {
        target: duo_matrix
        function onSendMatrix(data, cols, rows){
            matrixData = data
            matrixCols = cols
            matrixRows = rows
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
                text: "Размерность матрицы:"
            }

            TextField {
                id: sizeField1
                width: parent.width * 0.8
anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Введите количество строк/столбцов:"
                onTextChanged: {
                    var value = parseInt(text);
                    if (value > 5) {
                        value = 5;
                        text = "5";
                    }
                    if (value < 2){
                        value = 2;
                        text = "2";
                    }

                    rows1 = value;
                    cols1 = value;

                    if (rows1 + cols1 >= 1){
                    chek_input_size = 1
                    }
                                        for (var i = 0; i < (rows1*cols1); i++) {
                                                matrix1.push(0)
                                            matrixData.push(0)
                                        }
                }
            }


            Label {
                text: " "
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
                            placeholderText: "0"
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
                text: "Найти детерминант"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.4
                onClicked: {
                    duo_matrix.input_matrix1(matrix1, cols1, rows1)

                    console.log("Количество строк 1й: " + rows1)
                    console.log("Количество столбцов 1й: " + cols1)
                    console.log("Матрица 1: " + matrix1)

                    duo_matrix.action_det()
                    chek_add_size = 1
                    console.log("дата" + matrixData)
                    console.log("Cols" +matrixCols)
                    console.log("Rows" + matrixRows)
                }
            }
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                text: "Детерминант = " + matrixData[0]
                visible: chek_add_size === 1 && chek_input_size === 1
            }

        }
    }
}


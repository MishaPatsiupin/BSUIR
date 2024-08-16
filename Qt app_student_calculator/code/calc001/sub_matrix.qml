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
    property int cols2: 0
    property int rows2: 0
    property var matrix2: []

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
                text: "Размерность матриц:"
            }

            TextField {
                id: sizeField1
                width: parent.width * 0.8
anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Введите количество строк:"
                onTextChanged: {
                    var value = parseInt(text);
                    if (value > 5) {
                        value = 5;
                        text = "5";
                    }
                    if (value < 1){
                        value = 1;
                        text = "1";
                    }

                    rows1 = value;
                    rows2 = value;
                }
            }

            TextField {
                id: sizeField2
                width: parent.width * 0.8
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: "Введите количество столбцов:"
                onTextChanged: {
                    var value = parseInt(text);
                    if (value > 5) {
                        value = 5;
                        text = "5";
                    }
                    if (value < 1){
                        value = 1;
                        text = "1";
                    }
                    cols1 = value;
                    cols2 = value;

if (rows1 + cols1 >= 1){
chek_input_size = 1
}
                    for (var i = 0; i < (rows1*cols1); i++) {
                            matrix1.push(0)
                        matrix2.push(0)
                        matrixData.push(0)
                    }
                }
            }

            Label {
                text: " "
            }
            Label {
                text: "Вторая матрица:"
                visible: chek_input_size === 1
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


            Label {
                text: "Вторая матрица:"
                visible: chek_input_size === 1
            }

            Repeater {
                model: rows2
                delegate: Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    property int rowIndex: index
                    Repeater {
                        model: cols2
                        TextField {
                            id: cellField2
                            width: 45
                            height: 45
                            placeholderText: "0"
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            font.pixelSize: 20
                            color: "black"

                            onTextChanged: {
                                if (text === "") {
                                    matrix2[rowIndex * cols2 + index] = 0;
                                } else {
                                    matrix2[rowIndex * cols2 + index] = text;
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
                width: parent.width * 0.3
                onClicked: {
                    duo_matrix.input_matrix1(matrix1, cols1, rows1)
                    duo_matrix.input_matrix2(matrix2, cols2, rows2)

                    console.log("Количество строк 1й: " + rows1)
                    console.log("Количество столбцов 1й: " + cols1)
                    console.log("Матрица 1: " + matrix1)
                    console.log("Количество строк 2й: " + rows2)
                    console.log("Количество столбцов 2й: " + cols2)
                    console.log("Матрица 2: " + matrix2)

                    duo_matrix.action_sub()
                    chek_add_size = 1
                    console.log("дата" + matrixData)
                    console.log("Cols" +matrixCols)
                    console.log("Rows" + matrixRows)
                }
            }
            Label {
                text: "Результат вычитания матриц:"
                visible: chek_add_size === 1 && chek_input_size === 1
            }
            Repeater {
                model: matrixRows
                delegate: Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    property int rowIndex: index
                    Repeater {
                        model: matrixCols
                        TextField {
                            id: cellField
                            width: 45
                            height: 45
                            text: matrixData[rowIndex * matrixCols + index]
                            readOnly: true
                            horizontalAlignment: TextInput.AlignHCenter
                            verticalAlignment: TextInput.AlignVCenter
                            font.pixelSize: 20
                            color: "black"
                        }
                    }
                }
            }
        }
    }
}

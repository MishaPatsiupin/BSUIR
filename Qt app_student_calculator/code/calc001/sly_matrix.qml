import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Fusion 2.15

Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent

    property int equationsCount: 0
    property int variablesCount: 0
    property var matrix_x: []
    property var matrix_b: []

    property int result_cols: 0
    property int result_rows: 0
    property var result_data: []

        property int chek_output: 0

    Connections {
        target: duo_matrix
        function onSendMatrix(data, cols, rows){
            result_data = data
            result_cols = cols
            result_rows = rows
        }
    }



    Column {
        width: parent.width
        spacing: 10

        Label {
            text: "Решение системы уравнений"
        }


        TextField {
            id: equationsField
            width: parent.width * 0.8
anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Количество уравнений/неизвестных: "
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
                equationsCount = value
                variablesCount = value
                if (value > 1){
                   chek_output = 1
                }
                createForm()

            }
        }


//        TextField {
//            id: variablesField
//            width: parent.width * 0.8
//            anchors.horizontalCenter: parent.horizontalCenter
//            placeholderText: "Количество неизвестных: "
//            onTextChanged: {
//                var value = parseInt(text);
//                if (value > 5) {
//                    value = 5;
//                    text = "5";
//                }
//                if (value < 1){
//                    value = 1;
//                    text = "1";
//                }
//                variablesCount = value
//                createForm()
//            }
//        }

        ColumnLayout {
            id: equationsLayout
            visible: false
            Repeater {
                id: equationsRepeater
                model: equationsCount

                RowLayout {
                    property int rowIndex: index
                    spacing: 10
                    Layout.alignment: Qt.AlignHCenter  // Добавьте это

                    Repeater {
                        id: variablesRepeater
                        model: variablesCount

                        TextField {
                            id: coefficientField
                            implicitWidth: 30
                            clip: true
                            wrapMode: TextInput.NoWrap
                            placeholderText: "a" + (index + 1)
                            Layout.fillWidth: false

                            onTextChanged: {
                                if (text === "") {
                                    matrix_x[rowIndex * variablesCount + index] = 0;
                                    chek_output = 0
                                } else {
                                    matrix_x[rowIndex * variablesCount + index] = parseInt(text);
                                    chek_output = 0
                                }
                            }
                        }
                    }

                    Label {
                        text: "="
                    }

                    TextField {
                        id: constantField
                        implicitWidth: 30
                        clip: true
                        wrapMode: TextInput.NoWrap
                        placeholderText: "b" + (index + 1)
                        Layout.fillWidth: false

                        onTextChanged: {
                            if (text === "") {
                                matrix_b[rowIndex] = 0;
                            } else {
                                matrix_b[rowIndex] = parseFloat(text);
                            }
                        }
                    }
                }
            }
        }


            Button {
                enabled: check_output === 1
                text: "Вычислить"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    console.log(matrix_x)
                    console.log(matrix_b)
                    duo_matrix.input_matrix1(matrix_x, variablesCount, equationsCount)
                    duo_matrix.input_matrix2(matrix_b, variablesCount, equationsCount)

                    duo_matrix.action_sly()

                    console.log("дата" + result_data)
                    console.log("Cols" + result_cols)
                    console.log("Rows" + result_rows)
   chek_output = 2
                }
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                visible:chek_output === 2
                       font.pixelSize: 13
                                    text: "Решением являются значения a: " + result_data.join("; ")

                                }
//            Label {
//                anchors.horizontalCenter: parent.horizontalCenter
//                visible:chek_output === 1
//                                    text:"" + result_data
//                                }
        }


    function createForm() {
        if (equationsCount > 0 && variablesCount > 0 && equationsCount >= variablesCount) {
            matrix_x = [];
            matrix_b = [];

            for( var i = 0; i < (equationsCount * variablesCount); i++){
            matrix_x.push(0)
            }
            for( var j = 0; j < equationsCount; j++){
            matrix_b.push(0)
            }
           equationsLayout.visible = true
        } else {
            equationsLayout.visible = false
        }
    }
}

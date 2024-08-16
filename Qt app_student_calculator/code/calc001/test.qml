import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: win
    color: "#f5efff"
    anchors.fill: parent

    property int equationsCount: 0
    property int variablesCount: 0
    property var matrix_x: []
    property var matrix_b: []

    Column {
        spacing: 10
        anchors.centerIn: parent

        Label {
            text: "Количество уравнений:"
        }

        TextField {
            id: equationsField
            width: 50
            placeholderText: "0"
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
                createForm()
            }
        }

        Label {
            text: "Количество неизвестных:"
        }

        TextField {
            id: variablesField
            width: 50
            placeholderText: "0"
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
                variablesCount = value
                createForm()
            }
        }

        ColumnLayout {
            id: equationsLayout
            visible: false
            Repeater {
                id: equationsRepeater
                model: equationsCount

                RowLayout {
                    property int rowIndex: index
                    spacing: 10

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
                                    } else {
                                        matrix_x[rowIndex * variablesCount + index] = parseInt(text);
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

            Button {
                text: "Отправить данные"
                onClicked: {
                    console.log(matrix_x)
                    console.log(matrix_b)
                }
            }
        }
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

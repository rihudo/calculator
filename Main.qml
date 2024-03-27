import QtQuick
import QtQuick.Controls
import com.tool 1.0
import "calculator.js" as JStool

Window {
    id: win
    height: 640
    width: 480
    visible: true
    title: qsTr("Calculator")

    Solve{
        id: num_solve
        isResult: false
    }

    Rectangle {
        id: display
        height: 0.3645 * parent.height
        width: parent.width
        color: "white"
        border.color: "black"

        Text {
            id: display_text
            text: "0"
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignBottom
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 0.0002 * win.width * win.height
            wrapMode: Text.WrapAnywhere
        }
    }

    Rectangle {
        id: keys
        height: win.height - display.height
        width: parent.width
        color: "black"
        anchors.top: display.bottom
        Grid {
            rows: 5
            columns: 4
            Repeater {
                id: numkeys_grid
                model: ["(", ")", "AC", "DEL",
                        "7", "8", "9", "+",
                        "4", "5", "6", "-",
                        "1", "2", "3", "x",
                        ".", "0", "=", "/"]
                Button {
                    font.pointSize: 13
                    height: keys.height / 5
                    width: keys.width / 4
                    required property string modelData
                    text: modelData
                    onClicked: {
                        var current_txt = num_solve.isResult ? "0" : display_text.text;
                        num_solve.isResult = false;
                        if (text !== "=") {
                            display_text.text = JStool.text_input(current_txt, text);
                        }
                        else {
                            display_text.text = num_solve.num_calculate(display_text.text);
                            num_solve.isResult = true;
                        }
                    }
                }
            }
        }
    }
}

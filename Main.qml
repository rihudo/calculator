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

    function handle_input(text) {
        if ("" === text)
            return;
        var current_txt = num_solve.isResult ? "0" : display_text.text;
        if (text !== "=") {
            num_solve.isResult = false;
            display_text.text = JStool.text_input(current_txt, text);
        }
        else if (!num_solve.isResult) {
            display_text.text = num_solve.num_calculate(display_text.text);
            num_solve.isResult = true;
        }
    }

    Rectangle {
        focus: true
        Keys.onPressed: function(event) {
            let mapping_key = "";
            if (event.key >= Qt.Key_0 && event.key <= Qt.Key_9) {
                mapping_key = (event.key - Qt.Key_0).toString();
            }
            else
            {
                mapping_key = num_solve.get_mapping_str(event.key);
            }

            handle_input(mapping_key);
        }

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
                    text: modelData
                    onClicked: {
                        handle_input(text)
                    }
                }
            }
        }
    }
}

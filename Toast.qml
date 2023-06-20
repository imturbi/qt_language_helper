import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Rectangle {
    property bool isShow: false

    // 显示toast函数
    function showToast(text) {
        t.text = text;
        isShow = true;
        timer.restart();
    }

    visible: isShow

    width: t.width+20
    height: t.height+10
    radius: width/2

    color: "#666666"

    Text {
        id: t
        anchors.centerIn: parent

        padding: 10
        text: ""
        color: "white"
        font.pixelSize: 20
    }

    Timer {
        id: timer

        interval: 2000
        onTriggered: {
            isShow = false
        }
    }
}

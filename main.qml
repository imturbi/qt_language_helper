import QtQuick 2.9
import QtQuick.Window 2.2

import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import KbmLang 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("钥匙柜多语言工具")

    MessageDialog {
        id: dialogMsg
        title: qsTr("提示")
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Close
    }

    KbmLang {
        id: trans
    }

    Column {
        width: parent.width
        spacing: 20

        Column {
            spacing: 20

            Label {
                font.pixelSize: 20
                text: qsTr("抽取要翻译的文字")
            }

            Column {
                spacing: 10
                leftPadding: 10

                Row {
                    spacing: 5

                    FileDialog {
                        id: dialogFile
                        title: qsTr("选择文件")
                        folder: shortcuts.desktop
                        selectExisting: true
                        selectFolder: false
                        selectMultiple: false
                        nameFilters: ["*.ts"]
                        onAccepted: {
                            labelLang.text = dialogFile.fileUrl.toString().substring(8,dialogFile.fileUrl.length)
                        }
                    }

                    Button {
                        text: qsTr("选择语言文件...")
                        font.pixelSize: 18

                        onClicked: {
                            dialogFile.open()
                            labelTrans.text = ""
                        }
                    }

                    Label {
                        id: labelLang
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18

                    }

                }

                Row {
                    spacing: 5

                    Button {
                        text: qsTr("抽取文字")
                        font.pixelSize: 18

                        onClicked: {
                            if(labelLang.text.length == 0) {
                                dialogMsg.text = qsTr("请选择语言文件")
                                dialogMsg.open()
                                return
                            }

                            if(trans.parse(labelLang.text)) {
                                labelTrans.text = trans.fileWord
                            }
                        }
                    }

                    Label {
                        id: labelTrans
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                    }
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "gray"
        }

        Column {
            spacing: 20

            Label {
                font.pixelSize: 20
                text: qsTr("合并翻译的文字")
            }


            Column {
                spacing: 10
                leftPadding: 10

                Row {
                    spacing: 5

                    FileDialog {
                        id: dialogFile1
                        title: qsTr("选择文件")
                        folder: shortcuts.desktop
                        selectExisting: true
                        selectFolder: false
                        selectMultiple: false
                        nameFilters: ["*.ts"]
                        onAccepted: {
                            sfile.text = dialogFile1.fileUrl.toString().substring(8,dialogFile1.fileUrl.length)
                        }
                    }

                    Button {
                        text: qsTr("选择源文件...")
                        font.pixelSize: 18

                        onClicked: {
                            dialogFile1.open()
                        }
                    }

                    Label {
                        id: sfile
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                    }

                }

                Row {
                    spacing: 5

                    FileDialog {
                        id: dialogFile2
                        title: qsTr("选择文件")
                        folder: shortcuts.desktop
                        selectExisting: true
                        selectFolder: false
                        selectMultiple: false
                        nameFilters: ["*.xlsx"]
                        onAccepted: {
                            tfile.text = dialogFile2.fileUrl.toString().substring(8,dialogFile2.fileUrl.length)
                            trans.getLanguageList(tfile.text)
                        }
                    }

                    Button {
                        text: qsTr("选择翻译文件...")
                        font.pixelSize: 18

                        onClicked: {
                            dialogFile2.open()
                        }
                    }

                    Label {
                        id: tfile
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                    }

                }

                Row {
                    spacing: 5

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                        text: qsTr("请选择源语言")
                    }

                    ComboBox {
                        id: lang
                        model: trans.languageList
                    }
                }

                Row {
                    spacing: 5

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                        text: qsTr("请选择目标语言")
                    }

                    ComboBox {
                        id: lang1
                        model: trans.languageList

                    }
                }

                Row {
                    spacing: 5

                    Button {
                        text: qsTr("合并翻译")
                        font.pixelSize: 18

                        onClicked: {
                            if(sfile.text.length == 0) {
                                dialogMsg.text = qsTr("请选择源文件")
                                dialogMsg.open()
                                return
                            }

                            if(tfile.text.length == 0) {
                                dialogMsg.text = qsTr("请选择目标文件")
                                dialogMsg.open()
                                return
                            }

                            if(trans.replace(sfile.text, tfile.text, lang.currentText, lang1.currentText)) {
                                labelTrans.text = trans.fileWord
                            }
                        }
                    }

                    Label {
                        id: labelResult
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 18
                    }
                }
            }
        }

    }

    Toast {
        id: toast

        anchors.centerIn: parent

    }

    Connections {
        target: trans
        onMessageChanged: {
            toast.showToast(trans.message)
        }
    }
}

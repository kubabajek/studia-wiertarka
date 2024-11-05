import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    width: 400
    height: 450
    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height
    title: "Wiertarka"
    visible: true

    property var connected: false
    property var connection_message: "Select COM"

    property var mee: 0
  

    Rectangle{
        width: 400
        height: 60
        color: connected ? "green" : "red"
        Text {
            anchors.centerIn: parent
            visible: !connected
            text: "Please connect to a device"
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
        }
    }


    Image{
        id: img_wiertarka
        opacity: connected ? 0.1 : 1
        source: "images/wiertarka.jpg"
        width: !connected ? 450 : 450 + mee
        height: !connected ? 450 : 450 + mee
        fillMode: Image.PreserveAspectFit
    }

    Column {
        spacing: 20
        anchors.centerIn: parent
        visible: connected

        // Slider with a range of 0 to 100
        Slider {
            id: slider
            from: 0
            to: 100
            stepSize: 1
            value: 0 
            width: 300
            onValueChanged: {
                valueInput.text = Math.round(slider.value).toString()
                backend.sliderPercentageSlot(Math.floor(slider.value))
                mee = slider.value
            }
        }
        Text {
            text: "Current Value: " + Math.round(slider.value) + "%"
            font.pointSize: 16
            horizontalAlignment: Text.AlignHCenter
        }
        Row {
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Set Value:"
                font.pointSize: 14
            }

            TextField {
                id: valueInput
                text: Math.round(slider.value).toString()
                width: 50
                inputMethodHints: Qt.ImhDigitsOnly
                onEditingFinished: {
                    let num = parseInt(text);
                    if (!isNaN(num) && num >= 0 && num <= 100) {
                        slider.value = num;
                    } else {
                        // Reset to current slider value if invalid input
                        text = Math.round(slider.value).toString();
                    }
                }
            }
        }
    }
    
    
    Button {
        id: button_connect
        text: connected ? "Disconnect" : "Connect"
        enabled: true
        onClicked: backend.connectionButtonSlot(connected)
        anchors.bottom: parent.bottom
    }

    footer: Text {
        text: (connected ? "Connected! " : "Not connected. ") + connection_message
        font.family: "Helvetica"
        font.pointSize: 9
        color: "black"
    }

    
    Connections{
        target: backend
        function onConnectionStatusSignal(state, message){
            connected = state
            connection_message = message
        }
    }
}



# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import os
import sys
from pathlib import Path
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtCore import QObject, Signal, Slot, QThread, QUrl
from PySide6.QtWidgets import QApplication
from modules.communicator import Communicator

POWER_ON = 200
POWER_OFF = 201
DIRECTION_RIGHT = 202
DIRECTION_LEFT = 203

class MainClass(QObject):

    #Signals to connect with Communicator class
    sendDataSignal = Signal(int, int) 

    #Signals to connect with qml file
    connectionStatusSignal = Signal(bool, str)

    def __init__(self):
        QObject.__init__(self)
        self.uartThread = QThread()
        self.uart = Communicator()
        
        #Connect with Communicator class
        self.uart.receivedDataSignal.connect(self.uartReceivedDataHandler)
        self.uart.connectionStatusSignal.connect(self.connectionStatusSlot)
        self.sendDataSignal.connect(self.uart.send)
        self.uartThread.started.connect(self.uart.communication_thread)

        #Run communicator thread
        self.uart.moveToThread(self.uartThread)
        self.uartThread.start()


    #SLOTS FOR COMMUNICATOR
    @Slot(bool, str)
    def connectionStatusSlot(self, state : bool, message : str):
        self.connectionStatusSignal.emit(state, message)


    @Slot(int)
    def uartReceivedDataHandler(self, data : int):
        pass


    #SLOTS FOR WINDOW.QML
    @Slot(bool)
    def connectionButtonSlot(self, connected: bool):
        if(connected):
            self.uart.disconnect_com("Disconnected by user.")
        else:
           self.uart.select_com()
        
    @Slot (int)
    def sliderPercentageSlot(self, percentage: int):
        if(self.uart.connected):
            print(f"Update button % value: {percentage}")
            self.uart.send(percentage, 1)

    @Slot (bool)
    def powerButtonSlot(self, state: bool):
        if(self.uart.connected):
            print("Turn power ON" if state else "Turn power OFF")
            self.uart.send(POWER_ON if state else POWER_OFF, 1)
        else:
            print("DEBUG: uart not connected error")

    @Slot (bool)
    def directionButtonSlot(self, direction: bool):
        if(self.uart.connected):
            print("Turn direction RIGHT" if direction else "Turn direction LEFT")
            self.uart.send(DIRECTION_RIGHT if direction else DIRECTION_LEFT, 1)
        else:
            print("DEBUG: uart not connected error")        



if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_class = MainClass()

    engine = QQmlApplicationEngine()
    engine.rootContext().setContextProperty("backend", main_class)
    qml_file = os.fspath(Path(__file__).resolve().parent / 'window.qml')
    
    engine.load(QUrl.fromLocalFile(qml_file))
    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec())
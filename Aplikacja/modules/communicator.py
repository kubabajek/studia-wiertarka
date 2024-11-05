import serial
import time
from serial.tools import list_ports
from PySide6.QtCore import QObject, Signal
from PySide6.QtWidgets import QPushButton, QVBoxLayout, QDialog, QLabel, QComboBox
from PySide6.QtSerialPort import QSerialPortInfo

BAUDRATE = 9600 
TIMEOUT = None

class Communicator(QObject):
    receivedDataSignal = Signal(int)
    connectionStatusSignal = Signal(bool, str)
    connected = False
    uart = None
    def __init__(self):
        QObject.__init__(self)
        self.devices = set([item.description for item in list_ports.comports()])

    def communication_thread(self): 
        while True:
            if(self.connected):
                try:
                    recv = self.uart.read(1)
                    if recv:
                        data = int.from_bytes(recv, byteorder='big')
                        self.receivedDataSignal.emit(data)                        
                except Exception as e:
                    self.disconnect_com(str(e))
                    time.sleep(0.1)
                
            else:
                time.sleep(0.1)
            
                
    def send(self, data : int, len : int):
        if(not self.connected):
            print("Error device is not connected. (communicator send function)") 
            return
        data = data.to_bytes(len, 'big')
        try:
            self.uart.write(data)
        except Exception as e:
            self.disconnect_com(str(e))
            print("Transmit error: " + str(e))



    def disconnect_com(self, message):
        self.connected = False
        if(self.uart != None):
            self.uart.close()
            message += f" ({self.uart.portstr})"
        self.uart = None
        self.connectionStatusSignal.emit(False, message)
        print("Disconnected from com, reason: ", message)

    def connect_com(self, port):
        try:
            self.uart = serial.Serial(port, baudrate=BAUDRATE, timeout = TIMEOUT)
            self.connected = True
            self.connectionStatusSignal.emit(True, "Port: " + port)
            print("Connect to com: " + port)
            return True
        except Exception as e:
            self.disconnect_com(str(e))
            print("Connect to com error: " + str(e))
            return False

    def select_com(self):
        dialog = PortSelectionDialog()
        if dialog.exec():
            port = dialog.selected_port()
            self.connect_com(port)
            print(f"Selected port: {port}")



class PortSelectionDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Select COM Port")
        self.layout = QVBoxLayout()

        self.port_label = QLabel("Select COM Port:")
        self.layout.addWidget(self.port_label)

        self.port_combo = QComboBox()
        self.layout.addWidget(self.port_combo)

        self.refresh_ports_button = QPushButton("Refresh Ports")
        self.refresh_ports_button.clicked.connect(self.refresh_ports)
        self.layout.addWidget(self.refresh_ports_button)

        self.ok_button = QPushButton("OK")
        self.ok_button.clicked.connect(self.accept)
        self.layout.addWidget(self.ok_button)

        self.setLayout(self.layout)

        self.refresh_ports()

    def refresh_ports(self):
        self.port_combo.clear()
        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            port_name = port.portName()
            port_desc = port.description()
            self.port_combo.addItem(f"{port_name} ({port_desc})")

    def selected_port(self):
        return self.port_combo.currentText().split(" ")[0]
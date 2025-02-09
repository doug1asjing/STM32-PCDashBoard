import sys
import serial
import serial.tools.list_ports
import psutil
import time
import threading
from datetime import datetime
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QPushButton, QComboBox, QSystemTrayIcon, QMenu, QAction
from PyQt5.QtCore import QTimer, Qt
from PyQt5.QtGui import QIcon

class SerialMonitor(QMainWindow):
    def __init__(self):
        super().__init__()

        # 设定窗口标题 & 禁止调整窗口大小
        self.setWindowTitle("STM32桌面信息窗口")
        self.setGeometry(100, 100, 300, 200)
        self.setFixedSize(300, 200)  # 锁定窗口大小

        # 串口选择框
        self.port_label = QLabel("串口:", self)
        self.port_label.move(20, 20)

        self.port_combo = QComboBox(self)
        self.port_combo.move(70, 18)
        self.refresh_ports()

        # 温湿度显示
        self.temp_label = QLabel("温度: --°C", self)
        self.temp_label.move(20, 60)

        self.humid_label = QLabel("湿度: --%", self)
        self.humid_label.move(150, 60)

        # 开始 & 停止按钮
        self.start_button = QPushButton("连接", self)
        self.start_button.move(50, 120)
        self.start_button.clicked.connect(self.start_monitoring)

        self.stop_button = QPushButton("停止", self)
        self.stop_button.move(150, 120)
        self.stop_button.clicked.connect(self.stop_monitoring)
        self.stop_button.setEnabled(False)

        # 定时器
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_info)

        # **系统托盘图标**
        icon_path = "icon.png"  # 确保此路径下有图标文件（.ico）
        self.tray_icon = QSystemTrayIcon(QIcon(icon_path), self)
        self.tray_icon.setToolTip("STM32桌面信息窗口")  # 默认托盘提示

        # **托盘菜单**
        self.tray_menu = QMenu(self)
        restore_action = QAction("显示窗口", self)
        restore_action.triggered.connect(self.showNormal)

        exit_action = QAction("退出", self)
        exit_action.triggered.connect(self.exit_app)

        self.tray_menu.addAction(restore_action)
        self.tray_menu.addAction(exit_action)
        self.tray_icon.setContextMenu(self.tray_menu)

        # **托盘鼠标悬停更新**
        self.tray_icon.activated.connect(self.restore_from_tray)

        # 显示托盘
        self.tray_icon.show()

        # 串口线程
        self.serial_port = None
        self.is_running = False
        self.thread = None
        self.temp = "--"
        self.humid = "--"

    def refresh_ports(self):
        """刷新串口列表"""
        self.port_combo.clear()
        ports = serial.tools.list_ports.comports()
        for port in ports:
            self.port_combo.addItem(port.device)

    def start_monitoring(self):
        """启动串口通信"""
        selected_port = self.port_combo.currentText()
        if not selected_port:
            return

        try:
            self.serial_port = serial.Serial(selected_port, 9600, timeout=1)
            self.is_running = True
            self.start_button.setEnabled(False)
            self.stop_button.setEnabled(True)
            self.port_combo.setEnabled(False)  # 禁止修改串口选择框
            self.thread = threading.Thread(target=self.read_serial_data, daemon=True)
            self.thread.start()
            self.timer.start(1000)  # 每秒更新 CPU & 内存
        except Exception as e:
            print(f"串口打开失败: {e}")

    def stop_monitoring(self):
        """停止串口通信"""
        self.is_running = False
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.port_combo.setEnabled(True)  # 恢复串口选择框
        if self.serial_port:
            self.serial_port.close()
        self.timer.stop()

    def read_serial_data(self):
        """读取 STM32 发送的数据"""
        while self.is_running:
            try:
                data = self.serial_port.read(2)  # 读取 2 个字节
                if len(data) == 2:
                    self.temp = data[0]
                    self.humid = data[1]
                    self.temp_label.setText(f"温度: {self.temp}°C")
                    self.humid_label.setText(f"湿度: {self.humid}%")
                    self.update_tray_tooltip()  # 更新托盘鼠标悬浮提示
            except Exception as e:
                print(f"串口读取错误: {e}")
            time.sleep(1)

    def update_info(self):
        """发送 CPU 和内存使用情况到 STM32"""
        if self.serial_port and self.serial_port.is_open:
            cpu_usage = int(psutil.cpu_percent(interval=1))
            memory_usage = int(psutil.virtual_memory().percent)
            now = datetime.now()

            data = [
                cpu_usage,
                memory_usage,
                now.year - 1900,
                now.month,
                now.day,
                now.hour,
                now.minute,
                now.second
            ]

            try:
                self.serial_port.write(bytes(data))
            except Exception as e:
                print(f"数据发送失败: {e}")

    def update_tray_tooltip(self):
        """更新系统托盘悬浮提示信息"""
        tooltip = f"温度: {self.temp}°C\n湿度: {self.humid}%"
        self.tray_icon.setToolTip(tooltip)

    def closeEvent(self, event):
        """拦截窗口关闭事件，最小化到系统托盘"""
        event.ignore()
        self.hide()
        self.tray_icon.showMessage("STM32桌面信息窗口", "程序已最小化到托盘", QSystemTrayIcon.Information, 3000)

    def restore_from_tray(self, reason):
        """从托盘恢复窗口"""
        if reason == QSystemTrayIcon.ActivationReason.Trigger:
            self.showNormal()

    def exit_app(self):
        """退出应用程序"""
        self.tray_icon.hide()
        sys.exit()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SerialMonitor()
    window.show()
    sys.exit(app.exec_())

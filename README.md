# STM32-PCDashBoard
基于STM32的PC信息显示窗口，带有番茄钟，温湿度检测功能

# 硬件部分
STM32F103C8T6, DHT11, 0.96 inch OLED Display (SSD1306), Speaker


# 软件部分
基于STM32 HAL库，波特律动（BaudDance）RTC驱动、波特律动（BaudDance）OLED驱动
基于psutil、Pyserial获取上位机CPU与内存使用率，发送至STM32并显示

##主界面显示（当前时间、CPU与内存使用率）
##番茄钟
##环境信息（温湿度）
##设置页面

# TO DO
##PCB设计
##外壳设计
##代码易读性优化


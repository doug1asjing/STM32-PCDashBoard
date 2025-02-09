/**
 * @file kk_knob.c
 * @brief 旋钮编码器驱动实现文件
 */

#include <btn.h>
#include <gpio.h>

/**
 * @brief 按键消抖时间（毫秒）
 */
#define BTN_DEBOUNCE_TICKS 10

/**
 * @brief 按键状态枚举
 */
typedef enum {Pressed, Unpressed} BtnState;





/**
 * @brief 获取按键状态
 * @return 按键当前状态（按下/未按下）
 */
BtnState getMainBtnState(){
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET ? Pressed : Unpressed;
}

BtnState getSecondBtnState(){
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET ? Pressed : Unpressed;
}

/**
 * @brief 获取系统时间
 * @return 系统时间戳
 */
uint32_t getTick(){
	return HAL_GetTick();
}


/** 旋钮按下回调函数指针 */
MainBtnCallback onMainPressedCallback = NULL;
SecondBtnCallback onSecondPressedCallback = NULL;


/**
 * @brief 设置旋钮按下回调函数
 * @param callback 回调函数指针
 */
void Btn_SetMainPressedCallback(MainBtnCallback callback){
	onMainPressedCallback = callback;
}

void Btn_SetSecondPressedCallback(SecondBtnCallback callback){
	onSecondPressedCallback = callback;
}



/**
 * @brief 旋钮循环处理函数
 * @details 检测旋钮转动和按键状态，触发相应的回调函数
 */
void Main_Btn_Loop(){


	BtnState btnState = getMainBtnState();
	static uint8_t callbackState = 0; // 0:未调用 1:已调用
	static uint32_t pressedTime = 0;

	if (btnState == Pressed){
		if (pressedTime == 0){
			pressedTime = getTick();
		}else if (callbackState == 0 && getTick() - pressedTime > BTN_DEBOUNCE_TICKS){
			// 按键消抖完成 且尚未执行回调函数
			if (onMainPressedCallback != NULL){
				onMainPressedCallback();
			}
			callbackState = 1;
		}
	}else {
		// 按键松开
		pressedTime = 0;
		callbackState = 0;
	}
}

void Second_Btn_Loop(){


	BtnState btnState = getSecondBtnState();
	static uint8_t callbackState = 0; // 0:未调用 1:已调用
	static uint32_t pressedTime = 0;

	if (btnState == Pressed){
		if (pressedTime == 0){
			pressedTime = getTick();
		}else if (callbackState == 0 && getTick() - pressedTime > BTN_DEBOUNCE_TICKS){
			// 按键消抖完成 且尚未执行回调函数
			if (onSecondPressedCallback != NULL){
				onSecondPressedCallback();
			}
			callbackState = 1;
		}
	}else {
		// 按键松开
		pressedTime = 0;
		callbackState = 0;
	}
}

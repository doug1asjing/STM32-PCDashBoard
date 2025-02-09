/*
 * speaker.c
 *
 *  Created on: Feb 8, 2025
 *      Author: dougl
 */

#include <gpio.h>

void Speaker_Beep()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_Delay(50);

}

void Speaker_Beep_Tomato_Clock()
{
	uint32_t startTime = HAL_GetTick(); // 获取当前时间（毫秒）
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	while (HAL_GetTick() - startTime < 200);  // 等待 200ms
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	while (HAL_GetTick() - startTime < 500);  // 等待 200ms
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	while (HAL_GetTick() - startTime < 200);  // 等待 200ms
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);


}

/*
 * task_main.h
 *
 *  Created on: Feb 6, 2025
 *      Author: dougl
 */

#ifndef INC_TASK_MAIN_H_
#define INC_TASK_MAIN_H_

extern uint8_t recvInfo[];

void task_main();
void UI_draw();
void RTC_SerialSetTime();
void task_main_init();
void onMainBtnPressed();
void onSecondBtnPressed();


#endif /* INC_TASK_MAIN_H_ */

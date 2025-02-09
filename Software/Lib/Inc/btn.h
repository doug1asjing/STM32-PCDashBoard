#ifndef INC_BTN_H_
#define INC_BTN_H_




typedef void (*MainBtnCallback)(void);
typedef void (*SecondBtnCallback)(void);

void Main_Btn_Loop();
void Second_Btn_Loop();
void Btn_SetMainPressedCallback(MainBtnCallback callback);
void Btn_SetSecondPressedCallback(SecondBtnCallback callback);

#endif /* INC_BTN_H_ */

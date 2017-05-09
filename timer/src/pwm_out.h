#ifndef SRC_PWM_OUT_H_
#define SRC_PWM_OUT_H_

void PWM_Init();

void PWM_SetDutyCycle(uint8_t duty);
void PWM_Enable();
void PWM_Disable();

#endif /* SRC_PWM_OUT_H_ */

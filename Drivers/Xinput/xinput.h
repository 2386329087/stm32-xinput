#ifndef __XINPUT_H
#define __XINPUT_H

//Button
#define A_GPIO_PORT                  GPIOE
#define A_GPIO_PIN                   GPIO_PIN_4
#define A_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

#define B_GPIO_PORT                  GPIOE
#define B_GPIO_PIN                   GPIO_PIN_3
#define B_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

#define X_GPIO_PORT                  GPIOA
#define X_GPIO_PIN                   GPIO_PIN_0
#define X_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

//Button Read
#define Button_A        HAL_GPIO_ReadPin(A_GPIO_PORT, A_GPIO_PIN)
#define Button_B       	HAL_GPIO_ReadPin(B_GPIO_PORT, B_GPIO_PIN)
#define Button_X    		HAL_GPIO_ReadPin(X_GPIO_PORT, X_GPIO_PIN)

void Xinput_Init(void);
void ADC_Read(void);
void Xinput_Send(void);

#endif

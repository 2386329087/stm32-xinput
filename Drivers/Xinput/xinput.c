#include "main.h"
#include "stdio.h"
#include "string.h"
#include "xinput.h"
#include "adc.h"
#include "usbd_customhid.h"

int16_t ADC_Values[2];
extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t TxData[20] = {0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  //Holds USB transmit packet data

//有关Xinput各种硬件初始化
void Xinput_Init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	
	//Button Init
	A_GPIO_CLK_ENABLE();
	B_GPIO_CLK_ENABLE();
	X_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = A_GPIO_PIN;               
	gpio_init_struct.Mode = GPIO_MODE_INPUT;         
	gpio_init_struct.Pull = GPIO_PULLUP;             
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;   
	HAL_GPIO_Init(A_GPIO_PORT, &gpio_init_struct);   

	gpio_init_struct.Pin = B_GPIO_PIN;               
	gpio_init_struct.Mode = GPIO_MODE_INPUT;         
	gpio_init_struct.Pull = GPIO_PULLUP;             
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;   
	HAL_GPIO_Init(B_GPIO_PORT, &gpio_init_struct);   

	gpio_init_struct.Pin = X_GPIO_PIN;               
	gpio_init_struct.Mode = GPIO_MODE_INPUT;         
	gpio_init_struct.Pull = GPIO_PULLDOWN;           
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;   
	HAL_GPIO_Init(X_GPIO_PORT, &gpio_init_struct);   
	
	//ADC DMA START
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Values,2);
	
	
}

//检测按键，将对应的值放入TxData
void Button_Read(void)
{
	if(Button_A == 0 || Button_B == 0 || Button_X == 1)
	{
		HAL_Delay(10);
		if (Button_A == 0)  TxData[3] |= 0X10;

		if (Button_B == 0)  TxData[3] |= 0X20;
	
		if (Button_X == 1)  TxData[3] |= 0X40;	
	}
}

//转换ADC_Values
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//将转换后ADC_Values放入TxData
void ADC_Read(void)
{
	int16_t LX;
	int16_t LY;
	
	LX=ADC_Values[0];
	LY=ADC_Values[1];
	
	LX = (int16_t)map( LX, 0, 4096, INT16_MIN, INT16_MAX );
	LY = (int16_t)map( LY, 0, 4096, INT16_MIN, INT16_MAX );
	
	LY=~LY;
	
	//Left Stick X Axis
	TxData[6] = LOBYTE(LX);
	TxData[7] = HIBYTE(LX);
	//Left Stick Y Axis
	TxData[8] = LOBYTE(LY);
	TxData[9] = HIBYTE(LY);
	
	printf("%d,%d\r\n",LX,LY);
}

//发送Xinput report
void Xinput_Send(void)
{
	memset(TxData,0x00,20);
	TxData[0]=0x00;
	TxData[1]=0x14;
	
	Button_Read();
	ADC_Read();
	
	while(USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&TxData,20)!= USBD_OK);
}


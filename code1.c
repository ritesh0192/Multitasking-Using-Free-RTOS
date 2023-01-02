#include<FreeRTOS.h>
#include<task.h>
#include <stdint.h>
#include "stm32f4xx.h"

/* offest of the AHB1ENR register from RCC block */
#define RCC_AHB1ENR_OFFSET         0x30

/* Base address of the RCC.. we got this address by looking into memory map */
#define RCC_BASE_ADDR              (0x40023800)

/* actual address of the RCC_AHB1ENR Register */
#define RCC_AHB1ENR_ADDR            *((volatile unsigned long *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET) )

#define GPIOD_MODER_OFFSET        0X00

#define BASE_ADDR_GPIOD           (0x40020C00)

#define GPIOD_MODER                *((volatile unsigned long  *)(BASE_ADDR_GPIOD + GPIOD_MODER_OFFSET))
	
#define OFFSET_OF_OUTPU_DATA_REG 0X14
#define GPIOD_OPDATAREG      *( (volatile unsigned long *)(BASE_ADDR_GPIOD+OFFSET_OF_OUTPU_DATA_REG))

#define OFFSET_OF_INPUT_DATA_REG 0X10
#define GPIOD_IPDATAREG      *( (volatile unsigned long *)(BASE_ADDR_GPIOD+OFFSET_OF_INPUT_DATA_REG))

void toggle_led1(void *s);
void toggle_led2(void *s);



int main(void)
{
	uint32_t i=0;
	int j=0;
	
	/* 1. Enable the clock for GPIOD port */
	RCC_AHB1ENR_ADDR |= (1<<3);//bit 3 enables the clock for the GPIOD port
	
	/* 2. Configure the GPIO PIN to output mode using MODE register  */
	
	GPIOD_MODER |= (1 << (13 * 2)); //set 24th bit and 25 bit to 0x01 to make 12pin output
  GPIOD_MODER |= (1 << 15*2); //set 26th bit and 27 bit to 0x01 to make 12pin output
	GPIOD->OTYPER=0x00;
	GPIOD->PUPDR=0x00;
		
	/* 3. use the DATA REGISTER of GPIOD port to write to or read from LED */
	
	xTaskCreate(toggle_led2,"led blinking2",500,0,2,0);
	xTaskCreate(toggle_led1,"led blinking1",500,0,1,0);
  vTaskStartScheduler();
	/*while(1)
	{
	GPIOD->ODR = (1<<15);
		for(j=0; j<1000000; j++);//vTaskDelay(1000);
	GPIOD->ODR = ~(1<<15);	
		for(j=0; j<100000; j++);
	//vTaskDelay(1000);
  }
	return 0;
}*/
while(1);
}
void toggle_led1(void *s)
{
	while(1)
	{
	GPIOD->ODR|= (1<<15);
		vTaskDelay(1000);
	GPIOD->ODR &= ~(1<<15);	
	vTaskDelay(1000);
  
}
	}

void toggle_led2(void *s)
{
	while(1)
	{
		
	GPIOD_OPDATAREG |= (1<<13);	
	vTaskDelay(1000);
	GPIOD_OPDATAREG &= ~(1<<13);	
	vTaskDelay(1000);
	vTaskDelete(NULL);	
	}
}

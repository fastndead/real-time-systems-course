#include "stm32f4xx.h" // library in accordance with your hardware
 
// the variable reflects current time
unsigned long _time;
unsigned long _delay;
unsigned long _timestamp;
// system function, do not call it!
void SysTick_Handler(){
    _time++;
}
 
void SystemInit() {
    
    // enable clocking of used ports
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
 
    // enable output mode for LED pin
    GPIOD->MODER |= 1<<24;
	
	
    // configure system timer
    _time = 0;
    SysTick->LOAD = 24000;      //quant = 1ms
    SysTick->VAL = 0;
    SysTick->CTRL = 7;          //SysTick is enabled

}

volatile uint32_t timestamp = 0;
 
void toggle(){
    GPIOD->ODR ^= 1<<12;
}
 
int getPressed(){
    return ((GPIOA->IDR & 0x00000001)== 0)?(0):(1);
}
 

 
int main (){
	int isPressed = 0;
	int wasPressed;
	int _timestamp1;
	int flag = 0;
	int flag1 = 1;
	_timestamp = _time;
	_delay = 1000;
	wasPressed = 0;
	while (1) {	
		isPressed = getPressed();
		if(isPressed && flag1) {
			_timestamp1 = _time;
			wasPressed = 1;
			flag = 1;
			flag1 = 0;
		}
		if (!isPressed && flag && wasPressed) {
			_delay = _time - _timestamp1;
			flag = 0;
			flag1 = 1;
		}
		
		if(_time - _timestamp >= _delay) {
			toggle();
			_timestamp = _time;
		}

	}
}

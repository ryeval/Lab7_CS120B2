/*	Author: ryan
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #1  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//haha do arrays start at 0 or 1
enum States{start, wait, wait2, inc, dec, res} state;
unsigned char out = 0x00;
unsigned char btn0 = 0x00;
unsigned char btn1 = 0x00;

Tick(){
	LCD_ClearScreen();
	btn0 = ~PINA & 0x01;
	btn1 = ~PINA & 0x02;
	switch(state){
		case start:
			state = res;
			break;
		case res:
			if(btn0){
				state = inc;
			}
			else if(btn1){
				state = dec;
			}
			break;
		case wait:
			if(btn0){
				state = inc;
			}
			else if(btn1){
				state = dec;
			}
			else if(!(btn0||btn1)){
				state = wait2;
			}
			else if(btn0&&btn1){
				state = res;
			}
			break;
		case wait2:
			if(btn0&&btn1){
				state = res;
			}
			else if(btn0){
				state = inc;
			}
			else if(btn1){
				state = dec;
			}
			break;
		case inc:
			state = (btn0&&btn1) ? res : wait;
			break;
		case dec:
			state = (btn0&&btn1) ? res : wait;
			break;
	}
	switch(state){
		case start:
			break;
		case res:
			out = 0;
			break;
		case wait:
			break;
		case wait2:
			break;
		case inc:
			if(out == 9){
				break;
			}
			else{
				out++;
			}
			break;
		case dec:
			if(out == 0){
				break;
			}
			else{
				out--;
			}
			break;
	}
	LCD_WriteData(out + '0');
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;
	DDRC = 0xFF;	PORTC = 0x00;
	DDRD = 0xFF;	PORTD = 0xFF;
	
	TimerSet(100);
	TimerOn();

	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	state = start;
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

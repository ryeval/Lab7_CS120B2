/*	Author: ryan
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #1  Exercise #2
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

//
enum States{start, zero, one, two, secretOne, catch, win} state;
unsigned char out = 0x05;
unsigned char btn = 0x00;
unsigned char lights = 0x00;
unsigned char i = 0x01;


void Tick(){
	LCD_ClearScreen();
	btn = ~PINA & 0x01;
	switch(state){
		case start:
			state = zero;
			break;
		case zero:
			if(btn){
				if(out>0){
					out--;
				}
			}
			state = one;
			break;
		case one:			
			state = btn ? catch : two;
			break;
		case two:
			if(btn){
                                if(out>0){
                                        out--;  
                                }
                        }
			state = secretOne;
			break;
		case secretOne:
			state = btn ? catch : zero;
			break;
		case catch:
			state = zero;
			if(out == 9){
				state = win;
			}
			break;
		case win:
			if(btn){
				state = zero;
				out = 5;
			}
			break;
	}
	switch(state){
		case start:
			break;
		case zero:
			lights = 0x01;
			break;
		case one:
			lights = 0x02;
			break;
		case two:
			lights = 0x04;
			break;
		case secretOne:
			lights = 0x02;
			break;
		case catch:
			if(out < 9){
				out++;
			}
			lights = 0x02;
			break;
		case win:
			LCD_Cursor(i);
			i++;
			if(i == 16){
				i = 1;
			}
			break;
	}
	PORTB = lights;
	LCD_WriteData(out + '0');
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00;	PORTA = 0xFF;
	DDRB = 0xFF;	PORTB = 0x00;
	DDRC = 0xFF;	PORTC = 0x00;
	DDRD = 0xFF;	PORTD = 0xFF;

	TimerSet(250);
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

/*
 * fsm_setting.c
 *
 *  Created on: Oct 28, 2025 
 *      Author: nguye
 */

#include "fsm_automatic.h"
#include "fsm_setting.h"
#include "button_reading.h"
#include "display_7seg.h"
#include "main.h"
#include "software_timer.h"
#include "global.h"


void blinkyLed(int LED) {
	switch(BLINKY_LED_STATUS){
	case INIT:
		// Turn off all LED
		allLEDoff();
		if (LED == RED) BLINKY_LED_STATUS = LED;
		if (LED == GREEN) BLINKY_LED_STATUS = GREEN;
		if (LED == YELLOW) BLINKY_LED_STATUS = YELLOW;
		setTimer3(250); // 2Hz
		break;
	case RED:
		if (timer3_flag == 1) {
			setTimer3(250);
			HAL_GPIO_TogglePin(GPIOA, LED_RED_1_Pin | LED_RED_2_Pin);
		}
		break;

	case YELLOW:
		if (timer3_flag == 1) {
			setTimer3(250);
			HAL_GPIO_TogglePin(GPIOA, LED_YELLOW_1_Pin| LED_YELLOW_2_Pin);
		}
		break;

	case GREEN:
		if (timer3_flag == 1) {
			setTimer3(250);
			HAL_GPIO_TogglePin(GPIOA, LED_GREEN_1_Pin | LED_GREEN_2_Pin);
		}
		break;

	default:
		break;
	}
}


void fsm_setting() {
	switch(FSM_STATUS){
		case INIT:
		FSM_STATUS = MODE1;
		updateBufferWithMode(FSM_STATUS);
		displaySegment();
		break;
		case MODE1:
			if (timeRed != timeGreen + timeYellow) {
				timeRed = 5;
				timeGreen = 3;
				timeYellow = 2;
			}
			fsm_automatic();
			updateBufferWithMode(FSM_STATUS);
			displaySegment();

			if (isButton1Pressed()) {
				FSM_STATUS = MODE2;
				valueSetting = timeRed;
				BLINKY_LED_STATUS = INIT;
			}
			break;
		case MODE2:
			blinkyLed(RED);
			updateBufferWithMode(FSM_STATUS);
			displaySegment();
			if (isButton1Pressed()) {
				FSM_STATUS = MODE3;
				valueSetting = timeYellow;
				BLINKY_LED_STATUS = INIT;
			}
			else if (isButton2Pressed()){
				valueSetting++;
				if (valueSetting > 99)
					valueSetting = 1;
			}
			else if (isButton3Pressed()) {
				timeRed = valueSetting;
			}
			break;
		case MODE3:
			blinkyLed(YELLOW);
			updateBufferWithMode(FSM_STATUS);
			displaySegment();
			if (isButton1Pressed()) {
				FSM_STATUS = MODE4;
				valueSetting = timeGreen;
				BLINKY_LED_STATUS = INIT;
			}
			else if (isButton2Pressed()){
				valueSetting++;
				if (valueSetting > 99)
					valueSetting = 1;
			}
			else if (isButton3Pressed()) {
				timeYellow = valueSetting;
			}

			break;
		case MODE4:
			blinkyLed(GREEN);
			updateBufferWithMode(FSM_STATUS);
			displaySegment();
			if (isButton1Pressed()) {
				FSM_STATUS = MODE1;
				autoStatus = INIT;
				STATUS_7SEG = INIT;
			}
			else if (isButton2Pressed()){
				valueSetting++;
				if (valueSetting > 99)
					valueSetting = 1;
			}
			else if (isButton3Pressed()) {
				timeGreen = valueSetting;
			}
			break;
		default:
			break;
	}
}

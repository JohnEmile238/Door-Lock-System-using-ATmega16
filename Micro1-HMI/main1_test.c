/******************************************************************************
 *
 * File Name: main1.c
 *
 * Description: 
 *
 * Author: John Emile
 *
 * Date : Nov 2, 2021
 *
 *******************************************************************************/

#include "application_micro1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define NUMBER_OF_COMPARE_MATCHES_PER_SECONDS  			4

#define F_CPU 8000000UL

/************************************************************************
 *                           Global variables                           *
 ************************************************************************/

uint8 g_change=0;


int main(void)
{
	/* To hold the value sent by micro1 */
	uint8 received_byte = 0;

	/* Modules initialization */
	MICROCONTROLLER_init();

	/* Enable Global Interrupt */
	SREG |= (1<<7);

	/* Wait until MC2 is ready to initialize communication */
	while(UART_recieveByte() != MICRO2_READY);

	/* Send to Micro 2 that you're ready  to initialize communication */
	UART_sendByte(MICRO1_READY);




	for (;;)
	{
		received_byte = UART_recieveByte();
		/* Reset received value in every switch case in order not to execute again */

		switch(received_byte)
		{

		case PASSWORDS_MATCHED :

			LCD_clearScreen();
			LCD_displayString("MATCH");

			/* Delay for 1 sec using timer */
			APP_startTimer_1Sec();
			APP_Timer1DelayForScreenCheck();

			break;

		case PASSWORDS_UNMATCHED_FIRST_TIME :

			LCD_clearScreen();
			LCD_displayString("Doesn't Match");

			/* Delay for 1 sec using timer */
			APP_startTimer_1Sec();
			APP_Timer1DelayForScreenCheck();

			APP_getNewPassword();
			break;

		case CHANGE_PASSWORD: /* - */

			APP_getNewPassword();

			break;

		case ASK_FOR_NEW_PASSWORD :

			APP_getNewPassword();

			break;

		case PASSWORDS_UNMATCHED :

			/* LCD Commands */
			LCD_clearScreen();
			LCD_displayString("Unmatched");

			/* Delay for 1 sec using timer */
			APP_startTimer_1Sec();
			APP_Timer1DelayForScreenCheck();

			//			_delay_ms(1000);
			break;

		case ASK_FOR_PASSWORD : /* + */

			g_change=2;
			APP_askForPassword();

			break;

		case ASK_FOR_DOOR_PASSWORD :
			g_change = 0;
			APP_askForPassword();
			break;

		case ASK_FOR_CHANGING_PASSWORD :
			g_change=1;
			APP_askForPassword();
			break;

		case PASSWORD_WRONG :

			APP_wrongPassword();

			break;

		case DOOR_OPENING :

			APP_doorProcessing();

			break;

			//		case DOOR_CLOSING :
			//
			//			APP_doorClosing();
			//
			//			break;

		case INVALID_INPUT :

			APP_invalidInput();

			break;

		case SHOW_MENU :
			APP_mainMenu();
			break;
		}
	}
}
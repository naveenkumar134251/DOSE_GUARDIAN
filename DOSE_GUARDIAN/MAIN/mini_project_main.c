#include <LPC21xx.h>              // LPC21xx microcontroller register definitions
#include "defines.h"              // General macro definitions
#include "lcd.h"                  // LCD function declarations
#include "lcd_defines.h"           // LCD command and cursor macros
#include "keypad.h"               // Keypad interface functions
#include "rtc.h"                  // RTC initialization and access functions
#include "delay.h"                // Delay functions
#include "edit.h"                 // RTC and medicine edit functions
#include "pindefines.h"            // GPIO pin definitions

s32 med_hour=7,med_min,med_sec;   // Medicine reminder time variables (default hour = 7)
s32 hour,min,sec,date,month,year,day; // RTC time and date variables
u8 flag = 0;                      // Flag to avoid repeated buzzer triggering

int main()
{
	u8 choice,count,delay;         // Local variables for keypad choice, counters, and delay
	IODIR1 |= (1<<BUZZER|1<<LED);  // Configure BUZZER and LED pins as output

	/* Initialize the RTC, LCD, and 4x4 Keypad */
	RTC_Init();                    // Initialize RTC module
	InitLCD();                     // Initialize LCD display
	KeypadInit();                  // Initialize keypad GPIO pins

	while(1)                       // Infinite loop
	{
	        count++;               // Increment general-purpose counter
		    Display();             // Display current RTC time, date, and day

			if((((IOPIN0>>SW1)&1)==0))  // Check if menu switch (SW1) is pressed (active low)
				{
				   Disp_menu();        // Display menu options on LCD
				   WRITENIBBLE(IOPIN1,16,0); // Clear keypad rows
				   while(ColStat());   // Wait until a key is pressed
                   choice=ReadKeyVal(); // Read keypad value

   				   switch(choice)     // Process menu choice
				   {
					   case '1':CmdLCD(CLEAR_LCD); // Clear LCD
					    		edit_RTC();           // Edit RTC settings
								break;
					   case '2':CmdLCD(CLEAR_LCD); // Clear LCD
								edit_MED(&med_hour,&med_min,&med_sec); // Set medicine time
								break;
					   case '3':CmdLCD(CLEAR_LCD); // Clear LCD and exit menu
								break;
					/* If user presses a key other than '1','2','3',
					   display error message */
					   default :CmdLCD(CLEAR_LCD); // Clear LCD
							    StrLCD("INCORRECT INPUT"); // Display error message
							    delay_s(3);              // Hold message for 3 seconds
							    CmdLCD(CLEAR_LCD);       // Clear LCD
				   }
				}

				/* If the medicine time set by the user
				   matches the current RTC time */
			
				if(((hour==med_hour)&&(min==med_min))&&(flag==0)) // Check reminder time
				{
					flag = 1;             // Set flag to prevent retrigger
					count = 0;            // Reset counter
					Display_medicine_time(); // Display medicine reminder
					SETBIT(IOSET1,BUZZER); // Turn ON buzzer
					delay_ms(1000);        // Initial buzzer delay

					/* Wait for user confirmation using SW2 */
					CmdLCD(CLEAR_LCD);     // Clear LCD
					for(delay=0;delay<30;delay++) // Loop for reminder duration
					{
					     count++;          // Increment reminder counter
						 CmdLCD(GOTO_LINE1_POS0); // Move cursor to first line
						 StrLCD("TIME TO TAKE");  // Display reminder message
						 CmdLCD(GOTO_LINE2_POS0); // Move cursor to second line
						 StrLCD("MEDICINE");      // Display reminder message

			        	 if((READBIT(IOPIN0,SW2))==0) // Check if confirm switch is pressed
					     {
					        IOCLR1=1<<BUZZER; // Turn OFF buzzer
							CmdLCD(CLEAR_LCD); // Clear LCD
					        CmdLCD(GOTO_LINE1_POS0); // Move cursor to first line
						    StrLCD("MEDICINE TAKEN"); // Display confirmation
						    delay_s(1);               // Display delay
							count=1;                  // Reset count
							break;                    // Exit reminder loop
						 }
					}

						 if(count>=30)        // If no confirmation within time
					     {
								 IOCLR1=1<<BUZZER; // Turn OFF buzzer
						     CmdLCD(CLEAR_LCD); // Clear LCD
							 CmdLCD(GOTO_LINE1_POS0); // Move cursor
							 StrLCD("MEDICINE MISSED"); // Display missed message
					         SETBIT(IOSET1,LED); // Turn ON LED indicator
						     while(((IOPIN0>>SW2)&1)!=0); // Wait for acknowledgement
							   IOCLR1=1<<LED;  // Turn OFF LED
					     }
					CmdLCD(CLEAR_LCD); // Clear LCD after reminder handling
				} 
				
				if(min != med_min) // Reset flag once minute changes
				{
					 flag = 0;    // Allow next reminder trigger
				}
			}
    }		 

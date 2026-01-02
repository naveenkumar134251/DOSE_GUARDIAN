#include <LPC21xx.h>          // LPC21xx microcontroller register definitions
#include "keypad.h"          // Keypad interface function declarations
#include "lcd.h"             // LCD function prototypes
#include "defines.h"         // Common macro definitions
#include "rtc.h"             // RTC function declarations
#include "delay.h"           // Delay utility functions
#include "lcd_defines.h"     // LCD command and cursor position macros

void set_values(int* variable,int min,int max); // Function prototype for value input and validation

s32 hr,mn,sc,dy,dt,mm,yy;    // Global variables to store RTC hour, minute, second, day, date, month, year

/* Function to edit RTC parameters */
void edit_RTC()
{ 
	u8 choice;                // Variable to store keypad choice
	s32 flag=0;               // Flag used to exit the RTC edit menu
	while(1)                  // Infinite loop until user exits
	{
		//CmdLCD(CLEAR_LCD);   // LCD clear commented out intentionally
		CmdLCD(GOTO_LINE1_POS0);      // Move cursor to first line
		StrLCD("1:H 2:M 3:S 4:DY");   // Display RTC edit options
		CmdLCD(GOTO_LINE2_POS0);      // Move cursor to second line
		StrLCD("5:D 6:M 7:Y 8:EXT");  // Display remaining options
		WRITENIBBLE(IOPIN1,16,0);     // Clear keypad row outputs
		while(ColStat());             // Wait until a key is pressed
		choice=ReadKeyVal();          // Read key pressed
		CmdLCD(CLEAR_LCD);            // Clear LCD after key press
		CmdLCD(GOTO_LINE1_POS0);      // Move cursor to first line
		switch(choice)                // Process user selection
		{
			case '1':StrLCD("SET_HOUR(0-23)"); // Prompt hour setting
				     set_values(&hr,0,23);   // Accept hour input
			         SetRTCTimeInfo(hr,mn,sc,1); // Update RTC hour
				     break;		
			case '2':StrLCD("SET_MIN(0-59)");  // Prompt minute setting
				     set_values(&mn,0,59);   // Accept minute input
			         SetRTCTimeInfo(hr,mn,sc,2); // Update RTC minute
				     break;
			case '3':StrLCD("SET_SEC(0-59)");  // Prompt second setting
					 set_values(&sc,0,59);   // Accept second input
					 SetRTCTimeInfo(hr,mn,sc,3); // Update RTC second
				     break;
			case '4':StrLCD("SET_DAY(0-6)");   // Prompt day setting
				     set_values(&dy,0,6);    // Accept day input
			         SetRTCDay(dy);          // Update RTC day
				     break;
			case '5':StrLCD("SET_DATE(1-31)"); // Prompt date setting
				     set_values(&dt,1,31);  // Accept date input
			         SetRTCDateInfo(dt,mm,yy,5); // Update RTC date
				     break;
			case '6':StrLCD("SET_MONTH(1-12)");// Prompt month setting
					 set_values(&mm,1,12);  // Accept month input
					 SetRTCDateInfo(dt,mm,yy,6); // Update RTC month
				     break;
			case '7':StrLCD("SET_YEAR(2020-99)");// Prompt year setting
					 set_values(&yy,2020,2099); // Accept year input
			         SetRTCDateInfo(dt,mm,yy,7); // Update RTC year
				     break;
			case '8':flag=1;               // Set exit flag
			         break;
		}
		if(flag==1)                         // Check exit condition
		{
			break;                          // Exit edit_RTC loop
		}
	}
}

/* Function to edit medicine reminder time */
void edit_MED(s32* HH, s32* MM, s32* SS)
{
	u8 choice;                             // Variable to store keypad choice
	while(1)                               // Loop until exit selected
	{
		CmdLCD(CLEAR_LCD);                 // Clear LCD display
		CmdLCD(GOTO_LINE1_POS0);           // Move cursor to first line
		StrLCD("1:HOUR 2:MIN");            // Display medicine time options
		CmdLCD(GOTO_LINE2_POS0);           // Move cursor to second line
		StrLCD("3:SEC 4:EXIT");            // Display exit option
		WRITENIBBLE(IOPIN1,16,0);          // Clear keypad rows
		while(ColStat());                  // Wait for key press
		choice=ReadKeyVal();               // Read keypad value
		CmdLCD(CLEAR_LCD);                 // Clear LCD
		CmdLCD(GOTO_LINE1_POS0);           // Move cursor to first line
		switch(choice)                     // Process selection
		{
			
			case'1':StrLCD("MEDICINE_HOUR"); // Prompt medicine hour
			        delay_ms(200);          // Small delay for readability
					set_values(HH,0,23);   // Set medicine hour
				    break;		
			case'2':StrLCD("MEDICINE_MINUTE"); // Prompt medicine minute
			        delay_ms(200);             // Delay for LCD stability
					set_values(MM,0,59);      // Set medicine minute
				    break;
			case'3':StrLCD("MEDICINE_SECOND"); // Prompt medicine second
			        delay_ms(200);             // Delay for LCD stability
					set_values(SS,0,59);      // Set medicine second
				    break;
			case'4':CmdLCD(CLEAR_LCD);        // Clear LCD before exit
					goto BREAK;              // Jump to exit label
		}
		BREAK:if(choice=='4')               // Exit condition check
		{
			break;                          // Exit edit_MED loop
		}
	}
}

/* Function to accept numeric input with validation */
void set_values(int *variable,int min,int max)
{
	u32 num=0;                             // Variable to store entered number
    u8 digit;                             // Variable to store keypad digit

label:while(1)                            // Loop for digit entry
	  {
		  WRITENIBBLE(IOPIN1,16,0);        // Clear keypad rows
		  while(ColStat());                // Wait for key press
		  digit = ReadKeyVal();             // Read keypad value
		  delay_ms(150);                   // Debounce delay
		  if(digit=='#')                   // '#' confirms input
		  break;
		  else if(digit=='*')              // '*' deletes last digit
		  {
			  if(num>0)                    // Check if number exists
			  num = num / 10;              // Remove last digit
			  CmdLCD(GOTO_LINE2_POS0);     // Move cursor to second line
			  StrLCD("                ");  // Clear displayed number
		  }
		  else                             // Numeric key pressed
		  {
		     	num=(num*10)+(digit-'0');  // Append digit to number
		  }
		  CmdLCD(GOTO_LINE2_POS0);         // Move cursor to display line
		  IntLCD(num);                    // Display current number
      }

	 if(((num>=min)&&(num<=max)))          // Validate input range
	 {
		 *variable=num;                   // Store validated value
	     CmdLCD(CLEAR_LCD);               // Clear LCD
	     CmdLCD(GOTO_LINE1_POS0);         // Move cursor to first line
		 StrLCD("VALUE MODIFIED");        // Confirmation message
		 delay_ms(500);                  // Display delay
	 }
	else                                  // Invalid input case
	{
		 CmdLCD(CLEAR_LCD);               // Clear LCD
		 CmdLCD(GOTO_LINE1_POS0);         // Move cursor to first line
		 StrLCD("INCORRECT INPUT");       // Error message
		 delay_ms(500);                  // Delay for readability
		 CmdLCD(CLEAR_LCD);               // Clear LCD
		 CmdLCD(GOTO_LINE1_POS0);         // Move cursor to first line
		 StrLCD("Modify Again");          // Retry message
		 num=0;                           // Reset number
		 goto label;                     // Restart input process
	}
}

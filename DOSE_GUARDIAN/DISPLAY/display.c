#include <LPC21xx.h>        // LPC21xx microcontroller register definitions
#include "delay.h"         // Delay function declarations
#include "types.h"         // User-defined data types (u8, u32, s32, etc.)
#include "defines.h"       // General macro definitions
#include "lcd.h"           // LCD function prototypes
#include "lcd_defines.h"   // LCD command and position macros
#include "pindefines.h"    // GPIO pin definitions
#include "rtc.h"           // RTC function declarations

extern s32 hour,min,sec,date,month,year,day;  
// External RTC variables declared in another source file

/* Function to display medicine reminder message */
void Display_medicine_time()
{
     CmdLCD(CLEAR_LCD);        // Clear the LCD display
	 CmdLCD(GOTO_LINE1_POS0);  // Move cursor to first line, first position
	 StrLCD("TIME TO TAKE");   // Display reminder text on first line
	 CmdLCD(GOTO_LINE2_POS0);  // Move cursor to second line, first position
	 StrLCD("MEDICINE");       // Display medicine message on second line
}

/* Function to display current RTC time, date, and day */
void Display()
{
	  GetRTCTimeInfo(&hour,&min,&sec);   // Read current time from RTC
      DisplayRTCTime(hour,min,sec);      // Display time on LCD
	  GetRTCDateInfo(&date,&month,&year);// Read current date from RTC
	  DisplayRTCDate(date,month,year);   // Display date on LCD
	  GetRTCDay(&day);                   // Read current day from RTC
	  DisplayRTCDay(day);                // Display day on LCD
}

/* Function to display medicine missed warning */
void Disp_medicine_fail()
{
      CmdLCD(CLEAR_LCD);        // Clear LCD display
	  CmdLCD(GOTO_LINE1_POS0);  // Move cursor to first line
	  StrLCD("FORGET TO TAKE"); // Display missed reminder message
	  CmdLCD(GOTO_LINE2_POS0);  // Move cursor to second line
	  StrLCD("TABLET-PILL");    // Display tablet/pill message									
}

/* Function to display main menu options */
void Disp_menu()
{
      CmdLCD(CLEAR_LCD);                // Clear LCD display
      CmdLCD(GOTO_LINE1_POS0);          // Move cursor to first line
	  StrLCD("1:EDIT_RTC 3:EXIT");      // Display menu options on first line
	  CmdLCD(GOTO_LINE2_POS0);          // Move cursor to second line
	  StrLCD("2:SET_MEDICINE_TIME");    // Display menu options on second line
	  delay_s(1);                       // Hold menu display for 1 second
}

#include <LPC21xx.h>           // LPC21xx microcontroller register definitions
#include "delay.h"             // Delay function declarations
#include "types.h"             // User-defined data types (u8, s8, s32, f32, etc.)
#include "defines.h"           // General macro definitions
#include "lcd.h"               // LCD function prototypes
#include "lcd_defines.h"       // LCD command macros and configuration values
#include "pindefines.h"        // GPIO pin definitions for LCD
#include "rtc.h"               // RTC function declarations

extern s32 hour,min,sec,date,month,year,day; // External RTC variables

/* Initializing LCD */
void InitLCD(void)
{
	  IODIR0 |= (LCD_data)|(1<<RS)|(1<<RW)|(1<<EN); // Configure LCD data and control pins as output
	  delay_ms(20);                                // Initial power-on delay
	  CmdLCD(MODE_8BIT);                           // Set LCD to 8-bit mode
	  delay_ms(8);                                 // Delay for LCD processing
	  CmdLCD(MODE_8BIT);                           // Repeat 8-bit mode command
	  delay_ms(1);                                 // Short delay
	  CmdLCD(MODE_8BIT);                           // Ensure LCD is in 8-bit mode
	  delay_ms(1);                                 // Short delay
	  CmdLCD(SET_INTERFACE);                       // Configure LCD interface parameters
	  CmdLCD(DISPLAY_OFF);                         // Turn off LCD display
	  CmdLCD(CLEAR_LCD);                           // Clear LCD screen
	  CmdLCD(SHIFT_CUR_RIGHT);                     // Set cursor move direction
	  CmdLCD(DISPLAY_ON_CUR_OFF);                  // Turn display ON and cursor OFF
}

/* Displaying the command-0/data-1 which is received from the RS */
void DispLCD(u8 val)
{
	    IOCLR0 = 1<<RW;        // Select write operation (RW = 0)
	    WRITEBYTE(IOPIN0,0,val); // Write value to LCD data pins
	    IOSET0 = 1<<EN;        // Enable LCD to latch data/command
	    delay_ms(2);           // Enable pulse width delay
	    IOCLR0 = 1<<EN;        // Disable LCD enable pin
	    delay_us(2);           // Short delay for LCD execution
}

/* Received the command */
void CmdLCD(u8 cmd)
{
	     IOCLR0 = 1<<RS;       // Select command register (RS = 0)
	     DispLCD(cmd);         // Send command to LCD
	     delay_ms(2);          // Delay for command execution
}

/* Received the data */
void CharLCD(s8 data)
{
	     IOSET0 = 1<<RS;       // Select data register (RS = 1)
	     DispLCD(data);        // Send data to LCD
	     delay_ms(2);          // Delay for data write
} 

/* For displaying the string */
void StrLCD(s8 *ptr)
{
	while(*ptr!='\0')          // Loop until null terminator
	{
		 CharLCD(*ptr);        // Display one character
		 ptr++;                // Move to next character
	}
}

/* For displaying the number by converting integer to ASCII */
void IntLCD(s32 num)
{
	  s8 a[10],i=0;            // Array to store digits and index variable
	  if(num==0)               // Check for zero value
		{
			CharLCD(0+48);      // Display ASCII '0'
		}
		else{
			if(num<0)           // Check for negative number
			{
				  CharLCD('-'); // Display negative sign
				  num=-num;     // Convert to positive
			}
			while(num)          // Extract digits
			{
				  a[i++]=(num%10)+48; // Convert digit to ASCII
				  num=num/10;          // Remove last digit
			}
			--i;                // Adjust index to last valid digit
			for(;i>=0;i--)      // Display digits in correct order
			{
				   CharLCD(a[i]); // Display digit
			}
		}
}

/* For Displaying the customized character which is 
written in CGRAM and sent to DDRAM */
void WriteToCGRAM(void)
{
   /* Array for 5x8 dot matrix custom character */
	   s8 i,a[]={0x00,0x1f,0x11,0x11,0x11,0x1f,0x00,0x00}; // Custom character pattern
	   CmdLCD(0x60);          // Set CGRAM address
	   for(i=0;i<8;i++)       // Write 8 bytes to CGRAM
		    CharLCD(a[i]);    // Send pattern byte to LCD
}

/* For displaying the float value 
by converting float to ASCII */
void FloatLCD(f32 fN,u32 nDP)
{
	  u32 n;                  // Variable for integer part
	  s32 i;                  // Loop counter
	  if(fN<0.0)              // Check for negative float
		{
			  CharLCD('-');   // Display negative sign
			  fN = -fN;       // Convert to positive
		}
		n=fN;                  // Extract integer part
		IntLCD(n);              // Display integer part
		CharLCD('.');           // Display decimal point
		for(i=0;i<nDP;i++)      // Loop for decimal places
		{
			   fN = (fN-n)*10; // Shift decimal place
			   n=fN;           // Extract digit
			   CharLCD(n+48);  // Display decimal digit
		}
}

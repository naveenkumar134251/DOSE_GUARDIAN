#include <LPC21xx.h>              // LPC21xx microcontroller register definitions
#include "keypad_defines.h"       // Keypad row and column pin definitions
#include "keypad.h"               // Keypad function prototypes and LUT declaration
#include "types.h"                // User-defined data types (s8, u8, etc.)

/* Function to initialize keypad GPIO pins */
void KeypadInit(void)
{
	IODIR1 |= (1<<R0)|(1<<R1)|(1<<R2)|(1<<R3); // Configure keypad rows as output pins
}

/* Function to check column status (key press detection) */
int ColStat(void)
{
	if(((IOPIN1>>C0)&0xf)== 0x0f)   // Check if all column inputs are high (no key pressed)
	{
		return 1;                  // Return 1 if no key is pressed
	}
	return 0;                      // Return 0 if any key is pressed
}

/* Function to read the pressed key value */
char ReadKeyVal(void)
{
	s8 rowval,colval;              // Variables to store detected row and column values
	
	IOCLR1 = 1<<R0;                // Drive row 0 low
	IOSET1 = (1<<R1)|(1<<R2)|(1<<R3); // Drive remaining rows high
	if(((IOPIN1>>C0) & 0xf) != 0xf) // Check if any column is active for row 0
	{
		rowval = 0;                // Store detected row index
		 goto colcheck;            // Jump to column detection logic
	}
	
	IOCLR1 = 1<<R1;                // Drive row 1 low
	IOSET1 = (1<<R0)|(1<<R2)|(1<<R3); // Drive remaining rows high
	if(((IOPIN1 >> C0) &0xf) != 0xf) // Check if any column is active for row 1
	{
		rowval = 1;                // Store detected row index
		goto colcheck;             // Jump to column detection logic
	}
	
	IOCLR1 = 1<<R2;                // Drive row 2 low
	IOSET1 = (1<<R0)|(1<<R1)|(1<<R3); // Drive remaining rows high
	if(((IOPIN1 >> C0) & 0xf) != 0xf) // Check if any column is active for row 2
	{
		rowval = 2;                // Store detected row index
		goto colcheck;             // Jump to column detection logic
	}

	IOCLR1 = 1<<R3;                // Drive row 3 low
	IOSET1 = (1<<R0)|(1<<R1)|(1<<R2); // Drive remaining rows high
	if(((IOPIN1 >> C0) & 0xf) != 0xf) // Check if any column is active for row 3
	{
		rowval = 3;                // Store detected row index
		goto colcheck;             // Jump to column detection logic
	}
	
	colcheck:                       // Label for column detection
		if(((IOPIN1 >> C0) & 1) == 0) // Check if column 0 is active
		{
			colval = 0;             // Store column index
		}
		else if(((IOPIN1 >> C1) & 1) == 0) // Check if column 1 is active
		{
			colval = 1;             // Store column index
		}
		else if(((IOPIN1 >> C2) & 1) == 0) // Check if column 2 is active
		{
			colval = 2;             // Store column index
		}
		else                            // If none above, column 3 is active
		{
			colval = 3;             // Store column index
		}
		
		while(((IOPIN1 >> C0) & 0xf) != 0xf); // Wait until key is released
		
		return LUT[rowval][colval]; // Return the corresponding key value from lookup table
}

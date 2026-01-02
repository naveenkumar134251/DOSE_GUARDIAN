#include"types.h"          // Include user-defined data type definitions (e.g., u32)

/* Microsecond delay function */
void delay_us(u32 delay)   // Function to generate delay in microseconds
{
	delay *= 12;           // Scale delay count based on 12 MHz clock frequency
	while (delay--);       // Empty loop executes until delay count reaches zero
}

/* Millisecond delay function */
void delay_ms(u32 delay)   // Function to generate delay in milliseconds
{
	delay *= 12000;        // Convert milliseconds to loop count (12 MHz × 1000)
	while (delay--);       // Busy-wait loop for millisecond delay
}

/* Second delay function */
void delay_s(u32 delay)    // Function to generate delay in seconds
{
	delay *= 12000000;     // Convert seconds to loop count (12 MHz clock)
	while (delay--);       // Loop executes until required second delay completes
}

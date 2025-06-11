#include "TM4C123.h" // Device header

void delay_ms(uint32_t milliseconds) 
{
	
    // Configure Timer0 for delay
    SYSCTL->RCGCTIMER |= (1 << 0);        // Enable clock to Timer0

    TIMER0->CTL = 0;                      // Disable Timer0 during configuration
    TIMER0->CFG = 0x04;                   // 16-bit timer configuration
    TIMER0->TAMR = 0x01;                  // One-Shot timer mode
    TIMER0->TAILR = 16000 * milliseconds; // Set the reload value for the desired delay

    TIMER0->ICR |= 1 << 0;                // Clear Timer0 timeout flag
    TIMER0->CTL |= 1 << 0;                // Enable Timer0

    // Wait until the timeout flag is set
    while (!(TIMER0->RIS & (1 << 0))) 
		{
        // Waiting for the timer to reach the desired delay
    }

    TIMER0->ICR |= 1 << 0;           // Clear Timer0 timeout flag
    TIMER0->CTL = 0;                 // Disable Timer0 after the delay
}
int main(void)
{
    SYSCTL->RCGCGPIO |= (1 << 1); // Set bit 1 of RCGCGPIO to enable clock to PORTB

    // Initialize PB0,PB1,PB2 & PB3 as digital output & PB4,PB5,PB6 & PB7 as digital input for the button
    GPIOB->DIR |= (1 << 0)|(1 << 1)|(1 << 2)|(1 << 3);                                                    // Set PB0,PB1,PB2 & PB3  as digital output
    GPIOB->DIR &= ~(1 << 4)|~(1 << 5)|~(1 << 6)|~(1 << 7);                                                // Set PB4,PB5,PB6 & PB7 as digital input for the button
	  GPIOB->DEN |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) ; // Enable digital function for PB0,PB1,PB2,PB3,PB4,PB5,PB6 & PB7
    GPIOB->PUR |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);                                              // Enable pull-up for PB3,PB4 & PB5

    // Configure PB4,PB5,PB6 & PB7 for falling edge trigger interrupt
    GPIOB->IS &= ~(1 << 4)|~(1 << 5)|~(1 << 6)|~(1 << 7);           // Make PB4,PB5,PB6 & PB7  edge-sensitive
    GPIOB->IEV &= ~(1 << 4)|~(1 << 5)|~(1 << 6)|~(1 << 7);          // Falling edge trigger
		GPIOB->ICR |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);        // Clear any prior interrupt
    GPIOB->IM |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);         // Unmask interrupt

    // Enable interrupt in NVIC and set priority to 3
    NVIC->IP[1] = 3 << 5;          // Set interrupt priority to 3 for PORTB
    NVIC->ISER[0] |= (1 << 1);     // Enable IRQ1 (D1 of ISER[0])

    while (1)
    {
        // Do nothing and wait for the interrupt to occur
    }
}
void GPIOB_Handler(void)
{
    GPIOB->ICR |= (1 << 4) ; // Clear the interrupt flag for PB4

    delay_ms(5000); // Simple delay for debouncing

    if (!(GPIOB->DATA & (1 << 4))) // Check if PB4/SW1 is low (button pressed)
    {
        GPIOB->DATA &= ~(1 << 0); // Set PB0 to HIGH 
    }
    else // Button released
    {
        GPIOB->DATA |= (1 << 0); // Set PB0 to LOW 
    }
		
		if (!(GPIOB->DATA & (1 << 5))) // Check if PB5/SW2 is low (button pressed)
    {
        GPIOB->DATA &= ~(1 << 1); // Set PB1 to HIGH 
    }
    else // Button released
    {
        GPIOB->DATA |= (1 << 1); // Set PB1 to LOW
		}
		if (!(GPIOB->DATA & (1 << 6))) // Check if PB6/SW3 is low (button pressed)
    {
        GPIOB->DATA &= ~(1 << 2); // Set PB2 to HIGH
    }
    else // Button released
    {
        GPIOB->DATA |= (1 << 2); // Set PB2 to LOW?
		}
		if (!(GPIOB->DATA & (1 << 7))) // Check if PB7/SW4 is low (button pressed)
    {
        GPIOB->DATA &= ~(1 << 3); // Set PB3 to HIGH
    }
    else // Button released
    {
        GPIOB->DATA |= (1 << 3); // Set PB3 to LOW?
		}
}
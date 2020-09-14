#include <msp430.h> 
#include <stdbool.h>
#include <stdint.h>

/*
 * testprogramma voor multiplexer
 * 2017 Harry Broeders
 * Als de multiplexer correct functioneert, gaat de groene led branden.
 * Als de multiplexer niet correct functioneert, gaat de rode led branden.
 * Zodra een fout gevonden is stopt het programma. Dus je kunt door de outputs
 * van het programma te testen, zien in welke situatie de fout is opgetreden. 
 *
 * Verbindingen:
 * Pin 1.1 = SEL0
 * Pin 1.2 = SEL1
 * Pin 2.0 = A
 * Pin 2.1 = B
 * Pin 2.2 = C
 * Pin 2.3 = D
 * Pin 2.4 = OUT
 */

bool mux(uint8_t sel, uint8_t in)
{
    return in & (1<<sel);
}

int main(void) 
{
    // stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    
    // Pin 1.0 = groene led
    // Pin 1.6 = rode led
    
    P1DIR = 1<<6 | 1<<2 | 1<<1 | 1<<0;
    P1OUT = 0;
    P2DIR = 1<<3 | 1<<2 | 1<<1 | 1<<0;
    P2OUT = 0;

    uint8_t select, input;
    bool output, expected_output;
    for (select = 0; select < 4; select++)
    {
        for (input = 0; input < 16; input++)
        {
            P1OUT &= ~(1<<2 | 1<<1);
            P1OUT |= select << 1;
            P2OUT &= ~(1<<3 | 1<<2 | 1<<1 | 1<<0);
            P2OUT |= input;
            __delay_cycles(1100); // delay 1 ms @ 1.1 MHz
            output = (P2IN & 1<<4) == 1<<4;
            expected_output = mux(select, input);
            if (output != expected_output)
            {
                P1OUT |= 1<<6;
                while (1); // stop when error is found
            }
        }
    }
    P1OUT |= 1<<0; 
    	
    return 0;
}

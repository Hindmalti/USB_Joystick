#include <avr/io.h>		// for the input/output registers
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// For the serial port

#define CPU_FREQ        16000000L       // Assume a CPU frequency of 16Mhz

void init_serial(int speed)
{
	/* Set baud rate */
	UBRR0 = CPU_FREQ/(((unsigned long int)speed)<<4)-1;

	/* Enable transmitter & receiver */
	UCSR0B = (1<<TXEN0 | 1<<RXEN0 | 1<<RXCIE0);

	/* Set 8 bits character and 1 stop bit */
	UCSR0C = (1<<UCSZ01 | 1<<UCSZ00);

	/* Set off UART baud doubler */
	UCSR0A &= ~(1 << U2X0);
}

//fonction donnée permettant d'envoyer sur le port série
void send_serial(unsigned char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

//fonction donnée permettant de récuperer du port série
unsigned char get_serial(void) 
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

/* void send_msg(char* message)
{
    int i = 0;
    while(message[i]!='\0'){
        send_serial(message[i]);
        _delay_ms(100);
        i++;
    }
}
 */

//Fonction permettant d'initialiser les pins en E/S
void output_init(void) {
	DDRB |= 0xff; // The 8 PINS as output
}

void input_init(void)
{ 
	DDRD &= 0xFC; // PIN 2, 3, 4, 5, 6 , 7 as input
	PORTD|= 0xFC; // pull-up activé
}

void interrupt_init() 
{
	PCICR = 1<<PCIE2;
	PCMSK2 = (1<<PCINT19) | (1<<PCINT20) | (1<<PCINT21) | (1<<PCINT22);
	sei();
}

//Fonction  
void light_led(unsigned char value){
	PORTB = (value & 0x7F); // On récupère X-------- 
	PORTD = (value & 0x80); // On récupère -XXXXXXXX
}

/* void toggle_led(void) {
	PORTB ^= 0xff;
	_delay_ms(100);
} */

//Interruption qui permet d'envoyer en série la valeur du PIND
ISR(PCINT2_vect)
{
	//send_serial('a');
	send_serial(PIND);
	//toggle_led();	
}

//Interruption qui en recevant sur le port série allume les leds
ISR(USART_RX_vect)
{
	uint8_t tmp = UDR0;
	light_led(tmp);
	/* if(tmp == 'a')
	{
		light_led(1);
	}
	if(tmp == 'b')
	{
		light_led(0);
	} */
} 

// Dummy main

int main(void)
{	
	output_init();
	input_init();
	interrupt_init();
	init_serial(9600);
	while(1){
	}
	
	return 0;
}

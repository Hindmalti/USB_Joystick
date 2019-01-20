#include <avr/io.h>		// for the input/output registers
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// For the serial port

#define CPU_FREQ        16000000L       // Assume a CPU frequency of 16Mhz

/*
PD2 = 2 => Joystick PCINT18
PD3 = 3 => Bouton droite PCINT19
PD4 = 4 => Bouton haut PCINT20
PD5 = 5 => Bouton bas PCINT21
PD6 = 6 => Bouton gauche PCINT22

PB0 = 8 => LED0
PB1 = 9 => LED1
PB2 = 10 => LED2
PB3 = 11 => LED3
PB4 = 12 => LED4
PB5 = 13 => LED5

*/

#define LED0 0
#define LED1 1 
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5

#define JOYSTICK 2
#define HAUT 4
#define GAUCHE 6
#define BAS 5
#define DROITE 3 

#define MASQUE_BOUTONS 0x7A

uint8_t octet = 0;

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


//Fonction permettant d'initialiser les pins en E/S
void output_init(void) {
	DDRB |= ((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED5));
}

void input_init(void)
{ 

	DDRD &= ~((1 << JOYSTICK) | (1 << BAS) | (1 << HAUT) | (1 << DROITE) | (1 << GAUCHE));
	// Activation de la résistance de pull-up sur les entrées
	PORTD |= ((1 << JOYSTICK) | (1 << BAS) | (1 << HAUT) | (1 << DROITE) | (1 << GAUCHE));

}

void interrupt_init() 
{
	PCICR = 1<<PCIE2;
	PCMSK2 = (1<<PCINT18) | (1<<PCINT19) | (1<<PCINT20) | (1<<PCINT21) | (1<<PCINT22);
	sei();
}


void light_led(unsigned char value)
{
	if(value >= 'A' && value <= 'F') 
	{
		/*
		Si on reçoit A, PORTB |= 0b00000001
		Si on reçoit F, PORTB |= 0b00100000
		*/
		PORTB |= (1 << (value - 'A'));

	} 
	else if (value >= 'a' && value <= 'f')
	{
		//PORT B = PORTB AND NOT(VALUE)
		PORTB &= ~(1<< (value - 'a'));
		
	} 
	
	send_serial(value);
}

//Interruption qui permet d'envoyer en série la valeur du PIND dès que l'état d'un bouton change
ISR(PCINT2_vect)
{
	octet = 0x00; 
	octet = (PIND & MASQUE_BOUTONS) >> 2;
	octet |= (1 << 5);
	send_serial(octet);
	_delay_ms(500);
}

//Interruption qui allume les leds grâce à ce qu'il reçoit en série
ISR(USART_RX_vect)
{
	unsigned char tmp = UDR0;
	light_led(tmp);
} 

// Dummy main

int main(void)
{	
	output_init();
	input_init();
	interrupt_init();
	init_serial(9600);
	while(1) {
	}
	
	return 0;
}

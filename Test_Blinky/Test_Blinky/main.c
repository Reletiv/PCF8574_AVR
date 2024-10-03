/*
 * Test_Blinky.c
 *
 * Created: 10/03/2024 22:23:23
 * Author : Wietse Houwers
 */ 
#define F_CPU (16000000UL)
#include <avr/io.h>
#include <util/delay.h>

// I2C address of the PCF8574
#define PCF8574_ADDRESS 0x20  // Adjust depending on A0, A1, A2 settings

// Function prototypes
void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t data);
uint8_t TWI_read_nack(void);

void PCF8574_write(uint8_t data);

int main(void)
{
	// Initialize TWI (I2C)
	TWI_init();

	uint8_t led_state = 0x00;  // Initially, LED is off (P0 = 0)

	while (1)
	{
		// Toggle LED state on pin P0
		led_state ^= (1 << 0);  // Toggle the bit corresponding to P0

		// Write the state to PCF8574
		PCF8574_write(led_state);

		// Delay to create a visible blink effect
		_delay_ms(500);
	}
}

void TWI_init(void)
{
	// Set SCL frequency to 100kHz, assuming F_CPU = 16MHz
	TWSR = 0x00;
	TWBR = 0x48;
	// Enable TWI
	TWCR = (1 << TWEN);
}

void TWI_start(void)
{
	// Send START condition
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));  // Wait for TWINT flag to be set
}

void TWI_stop(void)
{
	// Send STOP condition
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void TWI_write(uint8_t data)
{
	// Load data into TWDR register
	TWDR = data;
	// Start transmission
	TWCR = (1 << TWEN) | (1 << TWINT);
	// Wait for TWINT flag to be set
	while (!(TWCR & (1 << TWINT)));
}

void PCF8574_write(uint8_t data)
{
	TWI_start();
	TWI_write((PCF8574_ADDRESS << 1) | 0);  // Send address with write bit
	TWI_write(data);  // Send data to PCF8574
	TWI_stop();
}


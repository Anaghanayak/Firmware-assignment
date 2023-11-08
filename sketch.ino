#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PB5 //On-board LED on Arduino Uno
#define TEMPERATURE_THRESHOLD 30 //Temperature threshold in degrees Celsius

volatile uint16_t adc_value;

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect)
{
  TCNT1 = 0; //reset timer1

  //Read ADC value
  ADCSRA |= (1 << ADSC); //Start conversion
  while (ADCSRA & (1 << ADSC)); //Wait for conversion to complete

  adc_value = ADC; //Read 10-bit ADC value

  //Calculate temperature in degrees Celsius
  float temperature = (adc_value * 5.0 * 100.0) / 1024.0;

  //Control LED blinking rate based on temperature
  if (temperature < TEMPERATURE_THRESHOLD) {
    PORTB ^= (1 << LED_PIN); //Toggle LED every 250 ms
    TCCR1B = (1 << CS12) | (1 << CS10); //Set prescaler to 1024
  }
  else {
    PORTB ^= (1 << LED_PIN); //Toggle LED every 500 ms
    TCCR1B = (1 << CS12); //Set prescaler to 256
  }
}

void setup()
{
  //Set LED pin as output
  DDRB = (1 << LED_PIN);

  //Setup ADC
  ADMUX = (1 << REFS0); //Set reference voltage to AVCC
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Enable ADC and set prescaler to 128

  //Setup timer1
  TCCR1A = 0; //Normal mode
  TIMSK1 = (1 << TOIE1); //Enable overflow interrupt

  //Enable global interrupts
  sei();
}

void loop()
{
  //Main loop is empty as everything is handled by interrupts
}

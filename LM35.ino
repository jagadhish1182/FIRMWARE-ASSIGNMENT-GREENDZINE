// Define LM35 pin
const int LM35_Pin = A0; // Use 'const' instead of '#define' for better type checking

// Define LED pin
const int LED_Pin = 13;

boolean ledState = false; // Determines the current state of the LED

void setup() {
  pinMode(LED_Pin, OUTPUT);
  Serial.begin(9600); // Serial communication at 9600 baud rate

  // Timer1 setup for generating interrupts
  TCCR1A = 0; // Reset Timer1 control Register A
  
  // Set CTC mode
  bitClear(TCCR1B, WGM13);
  bitSet(TCCR1B, WGM12);
  
  // Set prescaler to 1024
  bitSet(TCCR1B, CS12);
  bitClear(TCCR1B, CS11);
  bitSet(TCCR1B, CS10);
  
  // Reset Timer1
  TCNT1 = 0;
  
  // Enable Timer1 compare interrupt
  bitSet(TIMSK1, OCIE1A);
  sei(); // Enable global interrupts
}

// Calculate compare value and temperature value
// Constants for calculation:
// OCR1A = (time(s) * clock Freq.)/prescaler
// Temperature value: every 10mV reads 1 degree Celsius in LM35
void loop() {
  int temp_adc_val; // ADC value
  float temp_val;   // Temperature value

  temp_adc_val = analogRead(LM35_Pin); // Read analog ADC value (0-1023)
  temp_val = (temp_adc_val * 4.88) / 10; // Convert ADC value to temperature value
  
  // Temperature value display
  Serial.print("Temperature: ");
  Serial.print(temp_val);
  
  // Control LED blinking based on temperature
  if (temp_adc_val < 30) {
    OCR1A = 3906; // Set compare value for 250 ms
  } else {
    OCR1A = 7812; // Set compare value for 500 ms
  }
}

// Interrupt service routine for Timer1 compare match
ISR(TIMER1_COMPA_vect) {
  // Toggle LED state
  ledState = !ledState;
  
  // Set LED pin state based on ledState
  digitalWrite(LED_Pin, ledState);
}

// PIN FOR MOTION SENSOR
#define SENSOR_PIN 2
// PIN FOR PHOTO SENSOR
#define PHOTO_SENSOR_PIN A0
// LED OFF DELAY
#define DELAY_OFF 5
// PHOTOSENSITIVITY FOR PHOTO SENSOR [49;969]
// 49 - MAX LIGHT, 969 - MIN LIGHT
#define PHOTOSENSITIVITY 80

// INTERRUPT COUNTER
uint16_t counter = 0;

// TIMER:
// PRESCALE = 1024
// COMPARE MODE
// CALL INTERRUPT EVERY 1 SECOND
void Timer_start()
{
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15624;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
}

// OFF LED AND STOP TIMER
void light_off()
{
  TCCR1B &= 0;
  TIMSK1 &= 0;
  PORTB &= 0;
}
// 0-1 PINS INPUT, 8-9 PINS INPUT, 10 PIN OUTPUT, 11 - 15 PINS INPUT
void setup() {
  DDRD |= 0b00000000;
  DDRB |= 0b00000100;
}

// PREV STATUS FOR MOTION SENSOR
uint8_t prev_status = 0;

void loop() {
  // READ VALUE OF MOTION SENSOR
  uint16_t sensor = digitalRead(SENSOR_PIN);
  // READ VALUE OF PHOTO SENSOR
  uint16_t val = analogRead(PHOTO_SENSOR_PIN);

  // LAST VALUE GREATER THEN 0 OF MOTION SENSOR
  if ((prev_status != sensor) && (prev_status > sensor)) {
    prev_status = 0;
    counter = 0;
    Timer_start();
  }
  
  // IF MOTION SENSOR's VALUE GREATER THEN 0
  // SAVE LAST VALUE IN prev_status
  // TURN ON LED
  if (sensor)
  {
    if (val > PHOTOSENSITIVITY) {
      prev_status = sensor;
      PORTB |= 0b00000100;
    }
  }
}

// TIMER's INTERRUPT
ISR(TIMER1_COMPA_vect)
{
  counter++;
  // IF COUNTER GREATER THEN DELAY_OFF
  // MINUS 1, BECAUSE TIMER [0;4] - 5 SECONDS
  if (counter > DELAY_OFF - 1) {
    light_off();
  }
}
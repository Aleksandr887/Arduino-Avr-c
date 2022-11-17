#include <LiquidCrystal.h>

#define RS 8
#define EN 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13
#define LCD_ROWS 16
#define LCD_COLS 2
#define WATERING_TIME 60
#define INACTION_TIME 900
#define MOISTURE_LOWER_LIMIT 50.0

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// 0 - 876
const uint16_t moisture_max = 876;
uint16_t moisture = 0;
uint16_t counter = 0;
uint8_t mode = 0;

uint16_t hours;
uint16_t minutes;
uint16_t seconds;

const String modes[2] = {"Standby ", "Watering "};

void sec_to_time()
{
    seconds = counter;
    minutes = seconds / 60;
    seconds = seconds - minutes * 60;
    hours = minutes / 60;
    minutes = minutes - hours * 60;
}

void Print_info()
{
    lcd.setCursor(0, 0);
    lcd.print(modes[mode] + "      ");
    lcd.setCursor(0, 1);
    lcd.print("h=" + String(hours) + " m=" + String(minutes) + " s=" + String(seconds) + "    ");
}

void Timer_start()
{
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 156;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
}

void setup()
{
    Serial.begin(9600);

    DDRD |= 0b01000000;
    lcd.begin(LCD_ROWS, LCD_COLS);
    moisture = analogRead(A1);
    if (moisture < ((double)MOISTURE_LOWER_LIMIT / 100) * moisture_max) {
        mode = 1;
        counter = WATERING_TIME;
    } else {
        mode = 0;
        counter = INACTION_TIME;
    }
    sec_to_time();
    Timer_start();
}

void loop()
{
    moisture = analogRead(A1);
    Serial.println(moisture);
    Print_info();
}

ISR(TIMER1_COMPA_vect)
{
    if (mode) {
        PORTD |= 0b01000000;
        counter--;
        sec_to_time();
        if (counter < 1) {
            counter = INACTION_TIME;
            mode = 0;
        }
    } else {
        PORTD &= 0b00000000;
        counter--;
        sec_to_time();
        if (counter < 1) {
            if (moisture < ((double)MOISTURE_LOWER_LIMIT / 100) * moisture_max) {
                counter = WATERING_TIME;
                mode = 1;
            } else {
                counter = INACTION_TIME / 2;
            }
        }
    }
}
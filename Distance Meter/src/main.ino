#include <LiquidCrystal.h>
#include <Keypad.h>
#define RS 8
#define EN 9
#define D4 10
#define D5 11
#define D6 12
#define D7 13
#define TRIGPIN 7
#define ECHOPIN 6
#define LCD_ROWS 16
#define LCD_COLS 2
#define TIMEOUT 999999

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte colPins[rows] = {2, 3, 4, 5};
byte rowPins[cols] = {14, 15, 16, 17};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

unsigned long my_pulseIn(uint8_t pin, uint8_t state)
{
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    uint8_t stateMask = (state ? bit : 0);
    unsigned long width = 0;
    unsigned long numloops = 0;
    unsigned long maxloops = microsecondsToClockCycles(TIMEOUT) / 16;

    while ((*portInputRegister(port) & bit) == stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
    }

    while ((*portInputRegister(port) & bit) != stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
    }

    while ((*portInputRegister(port) & bit) == stateMask) {
        if (numloops++ == maxloops) {
            return 0;
        }
        width++;
    }

    return clockCyclesToMicroseconds(width * 21 + 16);
}

float distance_measurement()
{
    float duration;
    float distance;

    PORTD &= (0 << TRIGPIN);
    delayMicroseconds(2);

    PORTD |= (1 << TRIGPIN);
    delayMicroseconds(10);

    PORTD &= (0 << TRIGPIN);
    duration = my_pulseIn(ECHOPIN, HIGH) * 6.59;

    distance = duration / (float)58;

    return distance;
}

void disp_area(float a, float b)
{
    lcd.setCursor(0, 0);
    lcd.print("a=" + String(a, 2) + " b=" + String(b, 2) + "      ");

    lcd.setCursor(0, 1);
    lcd.print("S = " + String(a * b, 2) + "             ");
}

void rectangle_area()
{
    float a = 0.0;
    float b = 0.0;
    char ch = 0;
    uint8_t q = 1;

    disp_area(a, b);

    while (q) {
        ch = keypad.waitForKey();
        switch (ch) {
        case '1':
            a = distance_measurement();
            break;
        case '2':
            b = distance_measurement();
            break;
        case 'A':
            q = 0;
            break;
        }
        disp_area(a, b);
    }

    lcd.clear();
}

void circle_area()
{
    float r = 0.0;
    char ch = 0;
    uint8_t q = 1;

    lcd.setCursor(0, 0);
    lcd.print("r = " + String(r, 2) + "         ");
    lcd.setCursor(0, 1);
    lcd.print("S = " + String(r * r * 3.14, 2) + "       ");

    while (q) {
        ch = keypad.waitForKey();
        switch (ch) {
        case '7':
            r = distance_measurement();
            break;
        case 'C':
            q = 0;
            break;
        }

        lcd.setCursor(0, 0);
        lcd.print("r = " + String(r, 2) + "         ");
        lcd.setCursor(0, 1);
        lcd.print("S = " + String(r * r * 3.14, 2) + "       ");
    }

    lcd.clear();
}

void disp_volume(float a, float b, float c)
{
    lcd.setCursor(0, 0);
    lcd.print("a" + String(a, 0) + " b" + String(b, 0) + " c" + String(c, 0) + "     ");

    lcd.setCursor(0, 1);
    lcd.print("S = " + String(a * b * c, 2) + "             ");
}

void volume_parall()
{
    float a = 0.0;
    float b = 0.0;
    float c = 0.0;
    char ch = 0;
    uint8_t q = 1;

    disp_volume(a, b, c);

    while (q) {
        ch = keypad.waitForKey();
        switch (ch) {
        case '4':
            a = distance_measurement();
            break;
        case '5':
            b = distance_measurement();
            break;
        case '6':
            c = distance_measurement();
            break;
        case 'B':
            q = 0;
            break;
        }
        disp_volume(a, b, c);
    }

    lcd.clear();
}

void setup()
{
    Serial.begin(9600);
    DDRD |= 0b10000000;
    lcd.begin(LCD_ROWS, LCD_COLS);
    lcd.setCursor(0, 0);
    lcd.print("Distance");
}

void loop()
{
    float distance;
    char ch;

    lcd.setCursor(0, 0);
    lcd.print("digital ruler");

    ch = keypad.waitForKey();

    switch (ch) {
    case 'A':
        rectangle_area();
        break;
    case 'B':
        volume_parall();
        break;
    case 'C':
        circle_area();
        break;
    case 'D':
        lcd.setCursor(0, 1);
        lcd.print(String(distance_measurement()) + " cm            ");
        break;
    }
}
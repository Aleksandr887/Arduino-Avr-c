#include <LiquidCrystal.h>

#define R1 (double)100000
#define R2 (double)10000
#define ARDUINO_V (double)5
#define ADC_SCALE (double)1024
#define RS 8
#define EN 9
#define DB4 10
#define DB5 11
#define DB6 12
#define DB7 13
#define LCD_ROWS 16
#define LCD_COLS 2

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7); // контакты, к которым подключен ЖК дисплей

double input_v = 0.0;
float temp = 0.0;
float min = 999.0;
float max = 0.0;
String volts;

unsigned int ADC_read(uint8_t pin)
{
    // set pin for read value
    ADMUX |= ((pin & 0b1000) << MUX3) | ((pin & 0b0100) << MUX2) | ((pin & 0b0010) << MUX1) | ((pin & 0b0001) << MUX0);
    // enable converting input analog signal to binary code
    ADCSRA |= (1 << ADSC);
    // wait converting
    while (!(ADCSRA & (1 << ADIF)));
    // reset ADIF
    ADCSRA |= (1 << ADIF);
    // return value of ADC
    return ADC;
}

double ADC_measur_voltage(uint8_t pin)
{
    // read register value
    int i = ADC_read(1);
    // calculate voltage
    double j = ((double)ARDUINO_V / (double)ADC_SCALE) * i;
    j = j / (R2 / (R1 + R2));
    return j;
}

void ADC_init()
{
    // voltage from AVcc
    ADMUX = (1 << REFS0);
    // ADEN - enable adc
    // ADPSn = 1 (n - 0..2) prescaler = 128
    // F_ADC = 16M/128 = 125kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void setup()
{
    ADC_init();
    lcd.begin(LCD_ROWS, LCD_COLS);
}

void loop()
{
    input_v = ADC_measur_voltage(1);

    if (max < input_v) {
        max = input_v;
    }
    if (min > input_v) {
        min = input_v;
    }

    lcd.setCursor(0, 1);
    volts = String(input_v, 2) + "v ";
    lcd.print("Voltage = ");
    lcd.print(volts);
    lcd.setCursor(0, 0);
    lcd.print("max=" + String(max, 1) + "min=" + String(min, 1) + " ");

    _delay_ms(100);
}
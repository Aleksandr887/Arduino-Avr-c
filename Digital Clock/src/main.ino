#include <Wire.h>

#define HT16K33_ON 0x21
#define HT16K33_STANDBY 0x20
#define HT16K33_DISPLAYON 0x81
#define HT16K33_DISPLAYOFF 0x80 
#define HT16K33_BRIGHTNESS 0xE0

static const uint8_t charmap[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // B
    0x39, // C
    0x5E, // D
    0x79, // E
    0x71, // F
    0x00, // space
    0x40, // minus
};

class LEDPANEL
{
private:
    void writeCmd(uint8_t cmd)
    {
        Wire.beginTransmission(_addr);
        Wire.write(cmd);
        Wire.endTransmission();
    }
    void writePos(uint8_t pos, uint8_t mask)
    {
        if (_displayCache[pos] == mask)
            return;
        Wire.beginTransmission(_addr);
        Wire.write(pos * 2);
        Wire.write(mask);
        Wire.endTransmission();
        _displayCache[pos] = mask;
    }
    void writePos(uint8_t pos, uint8_t mask, bool pnt)
    {
        if (pnt)
            mask |= 0x80;
        writePos(pos, mask);
    }

    uint8_t _addr;
    uint8_t _displayCache[5];
    uint8_t _leadingZeroPlaces;
    uint8_t _bright;

public:
    LEDPANEL(const uint8_t address)
    {
        _addr = address;
    }

    void begin()
    {
        Wire.begin();
    }

    void displayOn()
    {
        writeCmd(HT16K33_ON);
        writeCmd(HT16K33_DISPLAYON);
        brightness(_bright);
    }

    void displayOff()
    {
        writeCmd(HT16K33_DISPLAYOFF);
        writeCmd(HT16K33_STANDBY);
    }

    void brightness(uint8_t val)
    {
        if (val == _bright)
            return;
        _bright = val;
        if (_bright > 0x0F)
            _bright = 0x0F;
        writeCmd(HT16K33_BRIGHTNESS | _bright);
    }

    void displayInt(int n)
    {
        uint8_t x[4], h, l;
        h = n / 100;
        l = n - h * 100;
        x[0] = h / 10;
        x[1] = h - x[0] * 10;
        x[2] = l / 10;
        x[3] = l - x[2] * 10;
        display(x);
    }

    void displayTime(uint8_t left, uint8_t right)
    {
        uint8_t x[4];
        x[0] = left / 10;
        x[1] = left - x[0] * 10;
        x[2] = right / 10;
        x[3] = right - x[2] * 10;
        display(x);
        displayColon(true);
    }

    void display(uint8_t *arr)
    {
        writePos(0, charmap[arr[0]]);
        writePos(1, charmap[arr[1]]);
        writePos(3, charmap[arr[2]]);
        writePos(4, charmap[arr[3]]);
    }

    void display(uint8_t *arr, uint8_t pnt)
    {
        writePos(0, charmap[arr[0]], pnt == 0);
        writePos(1, charmap[arr[1]], pnt == 1);
        writePos(3, charmap[arr[2]], pnt == 2);
        writePos(4, charmap[arr[3]], pnt == 3);
    }

    void displayColon(uint8_t on)
    {
        writePos(2, on ? 2 : 0);
    }
};

LEDPANEL disp(0x70);

void Timer_start()
{
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 15624; // 15624
    //OCR1A = 156;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << OCIE1A);
}

void Timer_stop()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B &= (0 << WGM12);
    TIMSK1 &= (0 << OCIE1A);
}

int sec = 0;
int mins = 0;
int hours = 0;
int value_5 = 0;
int value_6 = 0;
int value_7 = 0;
int mode = 0;
int timer = 0;
int settings = 0;
int set_mins = 0;
int set_hours = 0;

void setup()
{
    Wire.setClock(100000);
    disp.displayOn();
    DDRD = 0b00000000;
}

void loop()
{
    if (mode == 0)
    {
        disp.displayInt(0);
    }
    else if (mode == 1)
    {
        disp.displayTime(hours, mins);
    }
    else if (mode == 2)
    {
        disp.displayTime(0, sec);
    }
  
    if (digitalRead(5) & !value_5 & (mode == 1))
    {
        if (!settings)
        {
            Timer_stop();
            settings = 1;
        }
        else if (settings)
        {
            Timer_start();
            settings = 0;
        }
        value_5 = 1;
    }
    else if (!digitalRead(5))
    {
        value_5 = 0;
    }

    if (digitalRead(6) && !value_6)
    {
        if (settings == 1)
        {
            hours = (hours > 22) ? 0 : ++hours;
        }
        else
        {
            mode = (mode == 1) ? 2 : 1;
        }
        value_6 = 1;
    }
    else if (!digitalRead(6))
    {
        value_6 = 0;
    }

    if (digitalRead(7) && !value_7)
    {
        if (settings == 0)
        {	
            if (timer == 0)
            {
                Timer_start();
                timer = 1;
                mode = 1;
            }
            else
            {
                mode = (mode == 1) ? 2 : 1;
            }
        }
        else
        {
            mins = (mins > 58) ? 0 : ++mins;
        }
        value_7 = 1;
    }
    else if (!digitalRead(7))
    {
        value_7 = 0;
    }
}

ISR(TIMER1_COMPA_vect)
{
    sec++;
    if (sec > 59)
    {
        sec = 0;
        mins++;
        if (mins > 59)
        {
            mins = 0;
            hours++;
            if (hours > 23)
            {
                hours = 0;
            }
        }
    }
}
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "delay.h"
#include "I2C.h"
#include "LiquidCrystal_I2C.h"

uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};

// Function declarations
int strlen(const char *);
char *strrev(char *);
char *itoa(int, char *, int);

int strlen(const char *str)
{
	const char *s;
	s = str;
	while (*s)
		s++;
	return s - str;
}

char *strrev(char *str)
{
	char *p1, *p2;
	if (!str || !*str)
		return str;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

char *itoa(int n, char *s, int b) {
	static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int i=0, sign;
	if ((sign = n) < 0)
		n = -n;
	do
	{
		s[i++] = digits[n % b];
	} while ((n /= b) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	return strrev(s);
}

void displayKeyCodes(void)
{
  uint8_t i = 0;
  while (1) {
    LCDI2C_clear();
    LCDI2C_setCursor(0, 0);
	char buf[10];
	itoa(i, buf, 10);
    LCDI2C_write_String("Code 0x"); LCDI2C_write_String(buf);
	itoa(i+19, buf, 10);
    LCDI2C_write_String("-0x"); LCDI2C_write_String(buf);
    LCDI2C_setCursor(0, 1);
    int j;
    for (j=0; j<20; j++) {
      LCDI2C_write(i+j);
    }
    i+=16;
    if (i<15) break;
    Delay(1000);
  }
}

int main()
{
  uint8_t data;
  Delay(1000);
  LCDI2C_init(0x27,16,2); // adr, col, row
  LCDI2C_clear();
// ------- Quick 3 blinks of backlight  -------------
//  int i;
//  for( i = 0; i< 1; i++)
//  {
//    LCDI2C_backlight();
//    Delay(250);
//    LCDI2C_noBacklight();
//    Delay(250);
//  }
  //-------------------------------------------------
  LCDI2C_backlight(); // finish with backlight on
  //LCDI2C_write(65);
  Delay(1000);
  LCDI2C_clear();
  displayKeyCodes();
  Delay(1000);
  LCDI2C_createChar(0, bell); //код, описание
  LCDI2C_clear();
  LCDI2C_write_String("Hello Mouse ^_^");
  LCDI2C_setCursor(0,1);
  LCDI2C_write(0);
}

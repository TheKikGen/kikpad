
#include <string.h>
#include <stdarg.h>
///////////////////////////////////////////////////////////////////////////////
// SerialPrintf : a light printf like to Serial.
// %%  : %
// %n  : new line
// %s  : strings
// %m  : string minor case
// %M  : string major case
// %y  : string beautify (fist letter major case)
// %c  : character
// %x  : hexa
// %d  : integer
// %b  : binary
// %(n)s : print n char from string or char array
// %(nn)d/x/b : pad space left until size n
// %(0n)d/x/b : pad 0 left untile size n (nb for b : n=32 bits max).
///////////////////////////////////////////////////////////////////////////////
#define _PRINT_OUT Serial.print
void SerialPrintf(const char *format, ...)
{
  va_list varg;
  va_start(varg, format);

  while ( *format != 0 ) {
    if (*format == '%') {
      format++;
      if (*format == '%') _PRINT_OUT(*format);
      else if (*format == 'b') _PRINT_OUT(va_arg(varg, long),BIN); // Binary
      else if (*format == 'c') _PRINT_OUT((char)va_arg(varg, int)); // Char (must be casted to int)
      else if (*format == 'd') _PRINT_OUT(va_arg(varg, long ));  // long int
      else if (*format == 'u') _PRINT_OUT(va_arg(varg, unsigned long));  // u long int
      else if (*format == 'x') _PRINT_OUT(va_arg(varg, long),HEX);  // hexa
			else if (*format == 's' || *format == 'M' || *format == 'm' || *format == 'y') {
				char * str=va_arg(varg, char*);
				uint16_t i = 0;
				while (*str) {
						char c = *str;
						if ( c >= 'A' && c <= 'Z' ) {
							if ( *format == 'm' || (*format == 'y' && i > 0) ) c +=  0x20;
						} else
						if ( c >= 'a' && c <= 'z') {
							if ( *format == 'M' || (*format == 'y' && i == 0) ) c -=  0x20;
						}
						_PRINT_OUT(c); str++; i++;
				}
			}
      else if (*format >= '0' && *format <= '9' ) {
        char p =' ';
        if ( *format == '0') { format++; p = '0';}
        if ( *format >='1' && *format <= '9' ) {
          uint8_t pad = *format - '0';
          format++;
          if ( *format >='0' && *format <= '9' ) pad = pad*10 + *(format++) - '0';
					if ( *format == 'c' ) {
						while (--pad) _PRINT_OUT(p);
						_PRINT_OUT((char)va_arg(varg, int));
					}
          else if ( *format == 's' || *format == 'M' || *format == 'm' || *format == 'y') {
            char * str=va_arg(varg, char*);
						uint16_t i = 0;
						while (pad--) {
								if (*str) {
									char c = *str;
									if ( c >= 'A' && c <= 'Z' ) {
										if ( *format == 'm' || (*format == 'y' && i > 0) ) c +=  0x20;
									} else
									if ( c >= 'a' && c <= 'z') {
										if ( *format == 'M' || (*format == 'y' && i == 0) ) c -=  0x20;
									}
									_PRINT_OUT(c); str++; i++;
								}
								else break;
						}
					}
          else {
            uint8_t  base = 0;
            if ( *format == 'd' ) base = 10;
            else if ( *format =='x' ) base = 16;
            else if ( *format =='b' ) {
                base = 2;
                if ( pad > 32 ) pad = 8; // 32 bits max
            }
            if (base) {
							int value = va_arg(varg, long);
              uint32_t pw   = base;
              while (pad--) {
                if ( value < (int)pw ) while (pad) { _PRINT_OUT(p);pad--;}
                else pw *= base;
              }
              if (base == 16) _PRINT_OUT(value,HEX);
              else if (base == 2) _PRINT_OUT(value,BIN);
              else _PRINT_OUT(value);
            }
          }
        } else return;
      }
      else if (*format == 'n') Serial.println(); // new line
      else return;
    } else Serial.print(*format);
    format++;
  }
  va_end(varg);
}

#define DELAY_RGB 400
#define TIMER_RATE_MICROS 200
#define DELAY_DMC 1
#define LED_BANK_SIZE 32
#define PAD_SIZE 64

#define PAD_PATTERN0 0B11111111111111111111111111111111
#define PAD_PATTERN1 0B01010101010101010101010101010101
#define PAD_PATTERN2 0B10101010101010101010101010101010
#define PAD_PATTERN3 0B11110000111100001111000011110000
#define PAD_PATTERN4 0B00001111000011110000111100001111
#define PAD_PATTERN5 0B11111111000000001111111100000000
#define PAD_PATTERN6 0B10000001010000100010010000011000

enum {
  BLACK   = 0B000,
  RED     = 0B100,
  GREEN   = 0B010,
  BLUE    = 0B001,
  YELLOW  = 0B110,
  MAGENTA = 0B101,
  CYAN    = 0B011,
  WHITE   = 0B111
} LedColors;

uint8_t LedColorsPalette[]={BLACK,RED, GREEN, BLUE,YELLOW,MAGENTA,CYAN,WHITE};

enum {
  LED_BANK_HPAD_B,
  LED_BANK_HPAD_R,
  LED_BANK_HPAD_G,
  LED_BANK_LPAD_B,
  LED_BANK_LPAD_R,
  LED_BANK_LPAD_G,
  LED_BANK_BT1,
  LED_BANK_BT2,
} LedBanks;

enum {
  DM_DAI = PB15,
  DM_DCK = PB14,
  DM_LAT = PB13,
  DM_EN  = PB12,
  LS_EN  = PA2,
  LS_A0  = PA3,
  LS_A1  = PA4,
  LS_A2  = PA5,
} LedDriverPins;


volatile uint32_t BtStates1 = PAD_PATTERN0;
volatile uint32_t BtStates2 = PAD_PATTERN0;

uint32_t PadStatesH = PAD_PATTERN0;
uint32_t PadStatesL = PAD_PATTERN0;

uint8_t PadColorsH[] {
   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN
};

uint8_t PadColorsL[] {
   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN
};

volatile uint32_t PadColorRedMskH   = 0 ;
volatile uint32_t PadColorGreenMskH = 0 ;
volatile uint32_t PadColorBlueMaskH = 0 ;
volatile uint32_t PadColorRedMskL   = 0 ;
volatile uint32_t PadColorGreenMskL = 0 ;
volatile uint32_t PadColorBlueMaskL = 0 ;
boolean  PadColorMskNeedUpdate = false;



void UpdatePadColorMskHL() {

  PadColorRedMskH = PadColorGreenMskH = PadColorBlueMaskH = 0;
  PadColorRedMskL = PadColorGreenMskL = PadColorBlueMaskL = 0;

  for ( uint8_t i = 0; i != 32 ; i++ ) {
    if ( PadStatesH & (1<<i) ) {
        if ( PadColorsH[i] & RED )   PadColorRedMskH   |= 1<<i;
        if ( PadColorsH[i] & GREEN ) PadColorGreenMskH |= 1<<i;
        if ( PadColorsH[i] & BLUE )  PadColorBlueMaskH |= 1<<i;
    }
    if ( PadStatesL & (1<<i) ) {
        if ( PadColorsL[i] & RED )   PadColorRedMskL   |= 1<<i;
        if ( PadColorsL[i] & GREEN ) PadColorGreenMskL |= 1<<i;
        if ( PadColorsL[i] & BLUE )  PadColorBlueMaskL |= 1<<i;
    }
  }

}

void WriteDMC(uint32_t mask) {
  digitalWrite(DM_DCK,LOW);
  digitalWrite(DM_LAT,LOW);
  delayMicroseconds(DELAY_DMC);
  for ( uint8_t i = 0; i!= 32 ; i++ ) {
    if ( mask & 1 ) digitalWrite(DM_DAI, HIGH );
    else digitalWrite(DM_DAI, LOW );
    delayMicroseconds(DELAY_DMC);
    digitalWrite(DM_DCK,HIGH);
    delayMicroseconds(DELAY_DMC);
    digitalWrite(DM_DCK,LOW);
    delayMicroseconds(DELAY_DMC);
    mask >>= 1;
  }
  digitalWrite(DM_LAT,HIGH);
  delayMicroseconds(DELAY_DMC);
  digitalWrite(DM_LAT,LOW);
  delayMicroseconds(DELAY_DMC);
}

void SetLedBank(uint8_t addr ) {

  if ( addr & 1 ) digitalWrite(LS_A0, HIGH  ) ;
  else digitalWrite(LS_A0, LOW  ) ;

  if ( addr & 2 ) digitalWrite(LS_A1, HIGH  ) ;
  else digitalWrite(LS_A1, LOW  ) ;

  if ( addr & 4 ) digitalWrite(LS_A2, HIGH  ) ;
  else digitalWrite(LS_A2, LOW  ) ;

  return;
}

void Parallel() {
  uint32_t shift[]={ 0x55555555,
                     0xaa55aa55,
                     0x55aa55aa,
                     0x5a5a5a5a,
                     0xa5a5a5a5,
                     0x55555555,
                     0xaaaaaaaa,
                   };

  uint8_t bank = 0;

  WriteDMC(0);
  while (1) {
    digitalWrite(LS_EN,HIGH);
    delayMicroseconds(DELAY_DMC);

    if (bank & 0B001 ) digitalWrite(LS_A0,HIGH);
    else digitalWrite(LS_A0,LOW);

    if ( bank & 0B010 ) digitalWrite(LS_A1,HIGH);
    else digitalWrite(LS_A1,LOW);

    if ( bank & 0B100 ) digitalWrite(LS_A2,HIGH);
    else digitalWrite(LS_A2,LOW);

    WriteDMC(shift[bank]);

    digitalWrite(DM_EN,LOW);
    digitalWrite(LS_EN,LOW);
    delayMicroseconds(DELAY_DMC);
    digitalWrite(LS_EN,LOW);
    delayMicroseconds(400);

    if (++bank == 8 ) bank =0;
  }
}




void ColorTest() {


  // for ( uint8_t i = 0; i < PAD_SIZE ; i++ ) {
  //   if ( MyPads[i].on ) PadStates |= (1 << i);
  //   else PadStates &= ~(1 << i);
  // }

  uint32_t maskRed = 0 ;
  uint32_t maskGreen = 0 ;
  uint32_t maskBlue = 0 ;

  PadStatesH = PAD_PATTERN0;

  for ( uint8_t j = 0; j != 32 ; j++ ) {
    if ( PadStatesH & (1<<j) ) {
        if ( PadColorsH[j] & RED ) maskRed     |= 1<<j;
        if ( PadColorsH[j] & GREEN ) maskGreen |= 1<<j;
        if ( PadColorsH[j] & BLUE ) maskBlue   |= 1<<j;
    }
  }
  WriteDMC(0);
  digitalWrite(LS_EN,LOW);

  while (1) {
    // generate RGB colors
    // Red
    SetLedBank(LED_BANK_HPAD_R);
    WriteDMC(maskRed);
    delayMicroseconds(DELAY_RGB);
WriteDMC(0);
    // Green
    if ( maskGreen ) {
      SetLedBank(LED_BANK_HPAD_G);
      WriteDMC(maskGreen);
      delayMicroseconds(DELAY_RGB);
    }
    WriteDMC(0);

    // Blue
    if ( maskBlue ) {
      SetLedBank(LED_BANK_HPAD_B);
      WriteDMC(maskBlue);
      delayMicroseconds(DELAY_RGB);
    }
    WriteDMC(0);

  } // For
}


//  ISR
void Timer4Handler() {
  static uint8_t rgb = 0;





  // Compose RGB colors

  // // Red
  // if ( rgb == 0 ) {
  //   SetLedBank(LED_BANK_HPAD_R);
  //   WriteDMC(PadColorRedMskH);
  //   delayMicroseconds(DELAY_RGB);
  //   SetLedBank(LED_BANK_LPAD_R);
  //   WriteDMC(PadColorRedMskL);
  //
  // } else
  //
  // // Green H
  // if ( rgb == 1 ) {
  //   SetLedBank(LED_BANK_HPAD_G);
  //   WriteDMC(PadColorGreenMskH);
  //   delayMicroseconds(DELAY_RGB);
  //   SetLedBank(LED_BANK_LPAD_G);
  //   WriteDMC(PadColorGreenMskL);
  //
  // } else
  // // Blue H
  // if ( rgb == 2 ) {
  //   SetLedBank(LED_BANK_HPAD_B);
  //   WriteDMC(PadColorBlueMaskH);
  //   delayMicroseconds(DELAY_RGB);
  //   SetLedBank(LED_BANK_LPAD_B);
  //   WriteDMC(PadColorBlueMaskL);
  // } else
  // // Buttons
  // if ( rgb == 3 ) {
  //   SetLedBank(LED_BANK_BT1);
  //   WriteDMC(BtStates1);
  //   delayMicroseconds(DELAY_RGB);
  //   SetLedBank(LED_BANK_BT2);
  //   WriteDMC(BtStates2);
  //
  // }
  // if ( ++rgb == 4 ) rgb = 0;



  WriteDMC(0);
  // digitalWrite(LS_EN,HIGH);

  // Red H
  if ( rgb == 0 && PadColorRedMskH) {
    SetLedBank(LED_BANK_HPAD_R);
    WriteDMC(PadColorRedMskH);
  } else
  // Green H
  if ( rgb == 1 && PadColorGreenMskH) {
    SetLedBank(LED_BANK_HPAD_G);
    WriteDMC(PadColorGreenMskH);
  } else
  // Blue H
  if ( rgb == 2 && PadColorBlueMaskH) {
    SetLedBank(LED_BANK_HPAD_B);
    WriteDMC(PadColorBlueMaskH);
  } else
  // Red L
  if ( rgb == 3 && PadColorRedMskL) {
    SetLedBank(LED_BANK_LPAD_R);
    WriteDMC(PadColorRedMskL);
  } else
  // Green l
  if ( rgb == 4 && PadColorGreenMskL ) {
    SetLedBank(LED_BANK_LPAD_G);
    WriteDMC(PadColorGreenMskL);
  } else
  // Blue l
  if ( rgb == 5 && PadColorBlueMaskL) {
    SetLedBank(LED_BANK_LPAD_B);
    WriteDMC(PadColorBlueMaskL);
  } else

  // BUTTONS BARS
  // Refresh buttons states 1
  if ( rgb == 6 ) {
    SetLedBank(LED_BANK_BT1);
    WriteDMC(BtStates1);
  } else
  // Refresh buttons states 2
  if ( rgb == 7 ) {
    SetLedBank(LED_BANK_BT2);
    WriteDMC(BtStates2);
  }

  if ( ++rgb == 8 )  rgb = 0;

  // digitalWrite(LS_EN,LOW);

}

void SetPadColor(uint8_t padIdx,uint8_t color) {

  if (padIdx > 63) return;

  volatile uint32_t * padStates;
  volatile uint32_t * padRedMsk;
  volatile uint32_t * padGreenMsk;
  volatile uint32_t * padBlueMsk;
  uint32_t   mask;
  uint8_t previousColor;

  if ( padIdx < 32 ) {
    padStates = &PadStatesH;
    padRedMsk = &PadColorRedMskH;
    padGreenMsk = &PadColorGreenMskH;
    padBlueMsk = &PadColorBlueMaskH;
    padIdx = 31 - padIdx;
    previousColor = PadColorsH[padIdx];
    PadColorsH[padIdx] = color ;
  } else {
    padStates = &PadStatesL;
    padRedMsk = &PadColorRedMskL;
    padGreenMsk = &PadColorGreenMskL;
    padBlueMsk = &PadColorBlueMaskL;
    padIdx = 63 - padIdx;
    previousColor = PadColorsL[padIdx];
    PadColorsL[padIdx] = color  ;
  }

  mask = 1 << padIdx;

  if ( *padStates & mask ) {

    // Remove previous color
    if ( previousColor & RED )   *padRedMsk &= ~mask;
    if ( previousColor & GREEN ) *padGreenMsk &= ~mask;
    if ( previousColor & BLUE )  *padBlueMsk &= ~mask;

    // Set new color
    if ( color & RED )   *padRedMsk |= mask;
    if ( color & GREEN ) *padGreenMsk |= mask;
    if ( color & BLUE )  *padBlueMsk |= mask;
  }
}


void SetPad(uint8_t padIdx,uint8_t state) {

  if (padIdx > 63) return;

  volatile uint32_t * padStates;
  volatile uint32_t * padRedMsk;
  volatile uint32_t * padGreenMsk;
  volatile uint32_t * padBlueMsk;
  uint32_t   mask;
  uint8_t color;

  if ( padIdx < 32 ) {
    padStates = &PadStatesH;
    padRedMsk = &PadColorRedMskH;
    padGreenMsk = &PadColorGreenMskH;
    padBlueMsk = &PadColorBlueMaskH;
    padIdx = 31 - padIdx;
    color = PadColorsH[padIdx];
  } else {
    padStates = &PadStatesL;
    padRedMsk = &PadColorRedMskL;
    padGreenMsk = &PadColorGreenMskL;
    padBlueMsk = &PadColorBlueMaskL;
    padIdx = 63 - padIdx;
    color = PadColorsL[padIdx];
  }

  mask = 1 << padIdx;

  if ( state == 0 ) {
    *padStates &= ~mask ;
    *padRedMsk &= ~mask;
    *padGreenMsk &= ~mask;
    *padBlueMsk &= ~mask;
  } else {
    *padStates |= mask ;
    if ( color & RED )   *padRedMsk |= mask;
    if ( color & GREEN ) *padGreenMsk |= mask;
    if ( color & BLUE )  *padBlueMsk |= mask;
  }
}

void setup() {

  // USB DISC PIN
  gpio_set_mode(PIN_MAP[PA8].gpio_device, PIN_MAP[PA8].gpio_bit, GPIO_OUTPUT_PP);
  gpio_write_bit(PIN_MAP[PA8].gpio_device, PIN_MAP[PA8].gpio_bit, 1);
  Serial.begin(115200);
  delay(1000);

  pinMode(DM_EN,OUTPUT_OPEN_DRAIN);
  digitalWrite(PA2,HIGH);
  pinMode(LS_A0,OUTPUT);
  pinMode(LS_A1,OUTPUT);
  pinMode(LS_A2,OUTPUT);

  pinMode(LS_EN,OUTPUT_OPEN_DRAIN);
  digitalWrite(LS_EN,HIGH);
  pinMode(DM_DAI,OUTPUT);
  pinMode(DM_DCK,OUTPUT);
  pinMode(DM_LAT,OUTPUT);

  // Configure TIMER.
  HardwareTimer *timerMillis = new HardwareTimer(1);

  // Configure the millisec timer ISR
  timerMillis->pause();
  timerMillis->setPeriod(TIMER_RATE_MICROS); // in microseconds
  // Set up an interrupt on channel 4
  timerMillis->setChannel1Mode(TIMER_OUTPUT_COMPARE);
  // Interrupt 1 count after each update
  timerMillis->setCompare(TIMER_CH4, 1);
  timerMillis->attachInterrupt(TIMER_CH4, Timer4Handler);
  timerMillis->refresh();


UpdatePadColorMskHL();

  WriteDMC(0);
  SetLedBank(LED_BANK_BT2);
  digitalWrite(DM_EN,LOW);
digitalWrite(LS_EN,LOW);


timerMillis->resume();
}


void loop() {

  //ColorTest();
  // Update color mask is necessary
  if ( PadColorMskNeedUpdate ) {
    UpdatePadColorMskHL();
    PadColorMskNeedUpdate = false;
  }

  // for ( uint8_t i=0; i!=64 ; i++ ) {
  //   SetPad(i,0);
  //   delay(5);
  // }
  // for ( uint8_t i=0; i!=64 ; i++ ) {
  //   SetPad(i,1);
  //   delay(5);
  // }
  for ( uint8_t j=0 ; j< sizeof(LedColorsPalette); j++) {
    for ( uint8_t i=0; i!=64 ; i++ ) {
      SetPadColor(i,LedColorsPalette[j]);
    }
    delay(200);
  }

}

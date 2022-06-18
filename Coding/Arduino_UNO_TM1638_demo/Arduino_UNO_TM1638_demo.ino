/***********************************************************************
 * Project      :     Arduino_UNO_TM1638_demo
 * Description  :     ทดสอบโมดูล TM1638            
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     14 Nov 2018
 * Revision     :     1.0
 * Rev1.0       :     Original
 * Facebook     :     https://www.facebook.com/tenergy.innovation
 * Email        :     tenergy.innovation@gmail.com
 * TEL          :     +6689-140-7205
 ***********************************************************************/

/**************************************/
/*        GPIO define                 */
/**************************************/ 
#define strobe  8
#define clock  12
#define data  13



 /***********************************************************************
 * FUNCTION:    sendCommand
 * DESCRIPTION: sendCommand
 * PARAMETERS:  value
 * RETURNED:    nothing
 ***********************************************************************/
void sendCommand(uint8_t value)
{
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

 /***********************************************************************
 * FUNCTION:    reset
 * DESCRIPTION: reset
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void reset()
{
  sendCommand(0x40); // set auto increment mode
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);   // set starting address to 0
  for(uint8_t i = 0; i < 16; i++)
  {
    shiftOut(data, clock, LSBFIRST, 0x00);
  }
  digitalWrite(strobe, HIGH);
}

 /***********************************************************************
 * FUNCTION:    setup
 * DESCRIPTION: setup process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println("************************************************");
  Serial.println("Project      :  Arduino_UNO_TM1638_demo");                 
  Serial.println("Author       :  Tenergy Innovation Co., Ltd.");
  Serial.println("Date         :  14 Nov 2018");
  Serial.println("Facebook     :  https://www.facebook.com/tenergy.innovation");
  Serial.println("Email        :  tenergy.innovation@gmail.com");
  Serial.println("TEL          :  +6689-140-7205");
  Serial.println("************************************************");
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);

  sendCommand(0x8f);  // activate
  reset();
}


#define COUNTING_MODE 0
#define SCROLL_MODE 1
#define BUTTON_MODE 2

 /***********************************************************************
 * FUNCTION:    loop
 * DESCRIPTION: loop process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void loop()
{
  static uint8_t mode = COUNTING_MODE;

  switch(mode)
  {
  case COUNTING_MODE:
    mode += counting();
    break;
  case SCROLL_MODE:
    mode += scroll();
    break;
  case BUTTON_MODE:
    buttons();
    break;
  }

  delay(200);
}

 /***********************************************************************
 * FUNCTION:    counting
 * DESCRIPTION: counting
 * PARAMETERS:  nothing
 * RETURNED:    boolean
 ***********************************************************************/
bool counting()
{
                       /*0*/ /*1*/ /*2*/ /*3*/ /*4*/ /*5*/ /*6*/ /*7*/ /*8*/ /*9*/
  uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };

  static uint8_t digit = 0;

  sendCommand(0x40);
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);
  for(uint8_t position = 0; position < 8; position++)
  {
    shiftOut(data, clock, LSBFIRST, digits[digit]);
    shiftOut(data, clock, LSBFIRST, 0x00);
  }

  digitalWrite(strobe, HIGH);

  digit = ++digit % 10;
  return digit == 0;
}

 /***********************************************************************
 * FUNCTION:    scroll
 * DESCRIPTION: scroll
 * PARAMETERS:  nothing
 * RETURNED:    boolean
 ***********************************************************************/
bool scroll()
{
  uint8_t scrollText[] =
  {
    /* */ /* */ /* */ /* */ /* */ /* */ /* */ /* */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*H*/ /*E*/ /*L*/ /*L*/ /*O*/ /*.*/ /*.*/ /*.*/
    0x76, 0x79, 0x38, 0x38, 0x3f, 0x80, 0x80, 0x80,
    /* */ /* */ /* */ /* */ /* */ /* */ /* */ /* */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*H*/ /*E*/ /*L*/ /*L*/ /*O*/ /*.*/ /*.*/ /*.*/
    0x76, 0x79, 0x38, 0x38, 0x3f, 0x80, 0x80, 0x80,
  };

  static uint8_t index = 0;
  uint8_t scrollLength = sizeof(scrollText);

  sendCommand(0x40);
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xc0);

  for(int i = 0; i < 8; i++)
  {
    uint8_t c = scrollText[(index + i) % scrollLength];

    shiftOut(data, clock, LSBFIRST, c);
    shiftOut(data, clock, LSBFIRST, c != 0 ? 1 : 0);
  }

  digitalWrite(strobe, HIGH);

  index = ++index % (scrollLength << 1);

  return index == 0;
}

 /***********************************************************************
 * FUNCTION:    buttons
 * DESCRIPTION: buttons
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void buttons()
{
  uint8_t promptText[] =
  {
    /*P*/ /*r*/ /*E*/ /*S*/ /*S*/ /* */ /* */ /* */
    0x73, 0x50, 0x79, 0x6d, 0x6d, 0x00, 0x00, 0x00,
    /* */ /* */ /* */ /* */ /* */ /* */ /* */ /* */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*b*/ /*u*/ /*t*/ /*t*/ /*o*/ /*n*/ /*S*/ /* */
    0x7c, 0x1c, 0x78, 0x78, 0x5c, 0x54, 0x6d, 0x00,
    /* */ /* */ /* */ /* */ /* */ /* */ /* */ /* */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  static uint8_t block = 0;

  uint8_t textStartPos = (block / 4) << 3;
  for(uint8_t position = 0; position < 8; position++)
  {
    sendCommand(0x44);
    digitalWrite(strobe, LOW);
    shiftOut(data, clock, LSBFIRST, 0xC0 + (position << 1));
    shiftOut(data, clock, LSBFIRST, promptText[textStartPos + position]);
    digitalWrite(strobe, HIGH);
  }

  block = (block + 1) % 16;

  uint8_t buttons = readButtons();

  for(uint8_t position = 0; position < 8; position++)
  {
    uint8_t mask = 0x1 << position;

    setLed(buttons & mask ? 1 : 0, position);
  }
}

 /***********************************************************************
 * FUNCTION:    readButtons
 * DESCRIPTION: readButtons
 * PARAMETERS:  nothing
 * RETURNED:    uint8_t
 ***********************************************************************/
uint8_t readButtons(void)
{
  uint8_t buttons = 0;
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0x42);

  pinMode(data, INPUT);

  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(data, clock, LSBFIRST) << i;
    buttons |= v;
  }

  pinMode(data, OUTPUT);
  digitalWrite(strobe, HIGH);
  return buttons;
}

 /***********************************************************************
 * FUNCTION:    setLed
 * DESCRIPTION: setLed
 * PARAMETERS:  value, position
 * RETURNED:    nothing
 ***********************************************************************/
void setLed(uint8_t value, uint8_t position)
{
  pinMode(data, OUTPUT);

  sendCommand(0x44);
  digitalWrite(strobe, LOW);
  shiftOut(data, clock, LSBFIRST, 0xC1 + (position << 1));
  shiftOut(data, clock, LSBFIRST, value);
  digitalWrite(strobe, HIGH);
}

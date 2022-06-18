/***********************************************************************
   Project      :     Arduino_ESP32_diagnostic_Test
   Description  :     Total test board Arduino_Learning_Kit
   Author       :     Tenergy Innovation Co., Ltd.
   Date         :     14 Nov 2018
   Revision     :     1.1  
   Rev1.0       :     Original
   Rev1.1       :     สั่งให้ DC Motor หยุดหลังการทดสอบเสร็จ  
   Facebook     :     https://www.facebook.com/tenergy.innovation
   Email        :     tenergy.innovation@gmail.com
   TEL          :     +6689-140-7205
 ***********************************************************************/

/**************************************/
/*         include library            */
/**************************************/

#include <DS3231.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "images.h"
#include <Ticker.h> //for LED status
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_PCF8574.h>
#include "DHTesp.h"


/**************************************/
/*           GPIO define              */
/**************************************/
//ADC Volume 1-3
#define ADC39_PIN     39
#define ADC34_PIN     34
#define ADC35_PIN     35

//ds1820
#define ONE_WIRE_BUS  23

//dht22
#define dhtPin         23

//LDR pin
#define  LDR          36 //analog pin to which LDR is connected
//LED RGB
#define ledR          25
#define ledG          26
#define ledB          27

//PWM Motor
#define PWM_PIN_32    32
#define PWM_PIN_33    33
#define MOTOR_1       PWM_PIN_32
#define MOTOR_2       PWM_PIN_33
#define VR_2          ADC34_PIN
#define VR_1          ADC35_PIN
#define VR_3          ADC39_PIN

//Motion Sensor
#define PIR           14
//Relay Module 2 Channel
#define RELAY_2       13
#define RELAY_1       12

//RS485
HardwareSerial rs485(1);
#define RS485_PIN_DIR 27
#define RXD2          25
#define TXD2          26

//Stepping Motor
#define motorPin1     14// Blue   - 28BYJ48 pin 1
#define motorPin2     12// Pink   - 28BYJ48 pin 2
#define motorPin3     13// Yellow - 28BYJ48 pin 3
#define motorPin4     15// Orange - 28BYJ48 pin 4
#define motorSpeed    10     //variable to set stepper speed
//TM1638
#define strobe        19
#define clock         18
#define data          5
//Ultrasonic Sensor
#define ULTRA_TRIG_PIN      4
#define ULTRA_ECHO_PIN      2






/**************************************/
/*        object define               */
/**************************************/
//oled
SSD1306Wire  display(0x3c, 21, 22);  //21 = SDA, 22 = SCL
DS3231 Clock;
Ticker ticker;
Ticker Logo_ticker;

//lcd
LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//ds1820
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;

//dht22
DHTesp dht;


/**************************************/
/*        define function define      */
/**************************************/
//Oled Image
typedef void (*Demo)(void);
void tenergyImage();
void WiFiImage();
void WiFiOnImage();
void WiFiOffImage();
void HotspotImage();
void WiFiCoffeeImage();
void FacebookImage();
void BugImage();
void FixImage();
void drawProgressBarDemo();
void CameraImage();
void ClockImage();
void MotorImage();
void EepromImage();
void HomeImage();
void LedImage();
void MotionImage();
void RelayImage();
void Rs485Image();
void SteppingImage();
void TemperatureImage();
void Tmp1638Image();
void UltrasonicImage();
void UtenImage();
void PotentiometerImage();
void LdrImage();
void PayuthImage();
void Dip3swImage();
void MjuImage();
typedef void (*Demo)(void);
#define DEMO_DURATION 3000
void tick();
void oled_tick();

//ds3231
void ds3231_getdata(void);

//eeprom
void writeAddress(int address, byte val);
byte readAddress(int address);

//ds1820
float printTemperature(DeviceAddress deviceAddress);
void printAddress(DeviceAddress deviceAddress);

//led rgb
void hueToRGB(uint8_t hue, uint8_t brightness);

//stepping
void stepping_counterclockwise ();
void stepping_clockwise();
void stepping_stop();

//tm1638
void sendCommand(uint8_t value);
void reset();
bool counting();
bool scroll();
void buttons();
uint8_t readButtons(void);
void setLed(uint8_t value, uint8_t position);

//dht22
bool initTemp();



/**************************************/
/*        global variable             */
/**************************************/
//oled
int demoMode = 0;
int counter = 1;
Demo demos[] = {tenergyImage, drawProgressBarDemo, WiFiImage, WiFiOnImage, WiFiOffImage, HotspotImage, WiFiCoffeeImage, FacebookImage, BugImage, FixImage,
                CameraImage, ClockImage, MotorImage, EepromImage, HomeImage, LedImage, MotionImage, RelayImage, Rs485Image, SteppingImage, TemperatureImage, Tmp1638Image,
                UltrasonicImage, UtenImage, PotentiometerImage, LdrImage, PayuthImage, Dip3swImage, MjuImage
               };
int demoLength = (sizeof(demos) / sizeof(Demo));
int logo_blink;

//led rgb
uint8_t ledArray[3] = {1, 2, 3}; // three led channels
const boolean invert = true; // set true if common anode, false if common cathode
uint8_t color = 0;          // a value from 0 to 255 representing the hue
uint32_t R, G, B;           // the Red Green and Blue color components
uint8_t brightness = 255;  // 255 is maximum brightness, but can be changed.  Might need 256 for common anode to fully turn off.



//ds3231
bool Century = false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
char date_string[20], time_string[20], temp_string[20];

//LCD
int show;
int error;

//ds1820
int numberOfDevices; // Number of temperature devices found

//LDR
int ldr_value = 0; //variable to store LDR values

//ADC VR1 VR2 VR3
int val_A0, val_A1, val_A2, val_A3;





/**************************************/
/*        word define               */
/**************************************/
#define ON    LOW
#define OFF   HIGH
#define DEMO_DURATION 3000
#define tenergy_logo        0
#define progressive         1
#define wifi_logo           2
#define wifi_on_logo        3
#define wifi_off_logo       4
#define hotspot_logo        5
#define wificoffee_logo     6
#define facebook_logo       7
#define bug_logo            8
#define maintenance_logo    9
#define camera_logo         10
#define clock_logo          11
#define motor_logo          12
#define eeprom_logo         13
#define home_logo           14
#define led_logo            15
#define motion_logo         16
#define relay_logo          17
#define rs485_logo          18
#define stepping_logo        19
#define temperature_logo    20
#define tmp1638_logo        21
#define ultrasonic_logo     22
#define uten_logo           23
#define potentiometer_logo  24
#define ldr_logo            25
#define payuth_logo         26
#define dip3sw_logo         27
#define mju_logo            28

//eeprom
#define EEPROM_I2C_ADDRESS 0x50

//ds1820
#define TEMPERATURE_PRECISION 12 // Lower resolution

//rs485
#define RS485_WRITE     1
#define RS485_READ      0

//tm1638
#define COUNTING_MODE 0
#define SCROLL_MODE 1
#define BUTTON_MODE 2


/***********************************************************************
  FUNCTION:    setup
  DESCRIPTION: setup process
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void setup() {
        Serial.begin(115200);
        Serial.println("************************************************");
        Serial.println("Project      :  Arduino_ESP32_diagnostic_Test");
        Serial.println("Author       :  Tenergy Innovation Co., Ltd.");
        Serial.println("Date         :  14 Sep 2019");
        Serial.println("Facebook     :  https://www.facebook.com/tenergy.innovation");
        Serial.println("Email        :  tenergy.innovation@gmail.com");
        Serial.println("TEL          :  +6689-140-7205");
        Serial.println("************************************************");
      
        //Inital GPIO
        pinMode(ledR,OUTPUT);
        pinMode(ledG,OUTPUT);
        pinMode(ledB,OUTPUT);
        digitalWrite(ledR,LOW);
        digitalWrite(ledG,LOW);
        digitalWrite(ledB,LOW);
        
        //ds3231 initial
        Serial.print("Info: Intial DS3231...");
        Wire.begin();
        //**** setting time ***/
        Serial.print("Info: Setting DS3231...");
//          Clock.setYear(18);
//          Clock.setMonth(12);
//          Clock.setDate(20);
//          Clock.setDoW(5);
//          Clock.setHour(11);
//          Clock.setMinute(36);
//          Clock.setSecond(30);
          Serial.println("done");
        ds3231_getdata(); //**Need to call before display.init() **
        Serial.println("done");
      
        //ds1820 initial
        Serial.print("Info: Intial ds1820...");
        sensors.begin();
        // Grab a count of devices on the wire
        numberOfDevices = sensors.getDeviceCount();
      
        // locate devices on the bus
        Serial.print("Locating devices...");
      
        Serial.print("Found ");
        Serial.print(numberOfDevices, DEC);
        Serial.println(" devices.");
      
        
        // report parasite power requirements
        Serial.print("Parasite power is: ");
        if (sensors.isParasitePowerMode()) Serial.println("ON");
        else Serial.println("OFF");
       
        // Loop through each device, print out address
        for (int i = 0; i < numberOfDevices; i++)
        {
          // Search the wire for address
          if (sensors.getAddress(tempDeviceAddress, i))
          {
            Serial.print("Found device ");
            Serial.print(i, DEC);
            Serial.print(" with address: ");
            printAddress(tempDeviceAddress);
            Serial.println();
      
            Serial.print("Setting resolution to ");
            Serial.println(TEMPERATURE_PRECISION, DEC);
      
            // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
            sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
      
            Serial.print("Resolution actually set to: ");
            Serial.print(sensors.getResolution(tempDeviceAddress), DEC);
            Serial.println();
          } else {
            Serial.print("Found ghost device at ");
            Serial.print(i, DEC);
            Serial.print(" but could not detect address. Check power and cabling");
          }
          Serial.println("done");
      
        }

        Serial.print("Info: DHT22 initial....");
        dht.setup(dhtPin, DHTesp::DHT22);  //เปลี่ยน DHT11 หรือ DHT22 ได้ที่นี่
        Serial.println("done");  
    
        //lcd16x2 initial
        Serial.print("Info: Intial LCD16x2...");
        Wire.beginTransmission(0x27);
        error = Wire.endTransmission();
        Serial.print("Error: ");
        Serial.print(error);
    
        if (error == 0) {
          Serial.println(": LCD found.");
    
        } else {
          Serial.println(": LCD not found.");
        } // if
    
        lcd.begin(16, 2); // initialize the lcd
        Serial.println("done");
    
    
    
    
        //oled initial
        Serial.print("Info: SSD1306 oled...");
        display.init();
        display.flipScreenVertically();
        display.setFont(ArialMT_Plain_10);
        Serial.println("done");
    
    
    
//    
//        //lcd16x2 show
//        lcd.setBacklight(255);
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("*   Tenergy    *");
//        lcd.setCursor(0, 1);
//        lcd.print("*  Innovation  *");
//    
//        //oled show
//        display.clear();
//        Serial.print("Info: tenergy logo...");
//        demos[tenergy_logo]();
//        display.display();
//        delay(2000);
//        Serial.println("done");
//    
//        //  //select switch
//        //  logo_blink = dip3sw_logo;
//        //  Logo_ticker.attach(0.3, oled_tick);
//        //  while(1);
//    
//        //oled progres show
//        display.clear();
//        Serial.print("Info: progressive logo...");
//        counter = 0;
//        do {
//          display.clear();
//          demos[progressive]();
//          display.display();
//          delay(5);
//        } while (counter++ <= 498);
//        Serial.println("done");
//      
//  
//
//    //lcd16x2 show
//    lcd.setBacklight(255);
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Maejo University");
//    lcd.setCursor(0, 1);
//    lcd.print("*   SEE U LAB  *");
//
//    //oled show
//    display.clear();
//    Serial.print("Info: Meajo logo...");
//    demos[mju_logo]();
//    display.display();
//    delay(2000);
//    Serial.println("done");
//    delay(3000);

      
}



void loop(){
  
    int _val_int = 99; // ตัวแปลกำหนดการหมุนของมอเตอร์

  
    /* get value from user */
    if(Serial.available()){

      char _val_char = Serial.read();

      if(_val_char == '1'){
        Serial.println(_val_char);
        _val_int = 1;
      }
      else if (_val_char == '2'){
        Serial.println(_val_char);
        _val_int = 2;
      }  
      else if(_val_char == '3'){
        Serial.println(_val_char);
        _val_int = 3;
      }
      else if(_val_char == '4'){
        Serial.println(_val_char);
        _val_int = 4;
      }
      else if(_val_char == '5'){
        Serial.println(_val_char);
        _val_int = 5;
      }
      else if(_val_char == '6'){
        Serial.println(_val_char);
        _val_int = 6;
      }
      else if(_val_char == '7'){
        Serial.println(_val_char);
        _val_int = 7;
      }
      else if(_val_char == '8'){
        Serial.println(_val_char);
        _val_int = 8;
      }
      else if(_val_char == '9'){
        Serial.println(_val_char);
        _val_int = 9;
      }
      else if(_val_char == 'a' || _val_char == 'A'){
        Serial.println(_val_char);
        _val_int = 10;
      }
      else if(_val_char == 'b' || _val_char == 'B'){
        Serial.println(_val_char);
        _val_int = 11;
      }
      else if(_val_char == 'c' || _val_char == 'C'){
        Serial.println(_val_char);
        _val_int = 12;
      }
      else if(_val_char == 'd' || _val_char == 'D'){
        Serial.println(_val_char);
        _val_int = 13;
      }

      else if(_val_char == 'e' || _val_char == 'E'){
        Serial.println(_val_char);
        _val_int = 14;
      }

      else if(_val_char == 'f' || _val_char == 'F'){
        Serial.println(_val_char);
        _val_int = 15;
      }                        
      else if(_val_char == '0'){
          Serial.println("*********************************************");
          Serial.println("### Please select the item test below ###");
          Serial.println("0) Show menu item");
          Serial.println("1) DS3231 Real Time Clock testing");
          Serial.println("2) External EEPROM testing");
          Serial.println("3) Temperature sensor  testing");
          Serial.println("4) LDR sensor testing");
          Serial.println("5) ADC (VR1, VR2 and VR3) testing");
          Serial.println("6) LED RGB testing");
          Serial.println("7) Relay Module  testing");
          Serial.println("8) Motion sensor testing");
          Serial.println("9) Ultrasonic sensor testing");
          Serial.println("a) Stepping motor testing");
          Serial.println("b) DC Motor (PWM) testing");
          Serial.println("c) RS485 testing");
          Serial.println("d) TM1638 testing");
          Serial.println("e) LCD testing");
          Serial.println("f) OLED testing");
          
          Serial.print("Enter :");
        
        }
    }
    

    if(_val_int == 1){
        ds3231_testing();
      }
    else if(_val_int == 2){
        eeprom_testing();
      }  
    else if(_val_int == 3){
        temperature_sensor_testing();
      } 
    else if(_val_int == 4){
        ldr_testing();
      } 
    else if(_val_int == 5){
        adc_testing();
      }             
    else if(_val_int == 6){
        led_rgb_testing();
      }  
    else if(_val_int == 7){
        relay_testing();
      } 
    else if(_val_int == 8){
        motion_testing();
      } 
    else if(_val_int == 9){
        ultrasonic_testing();
      }     
    else if(_val_int == 10){
        stepping_testing();
      }  
    else if(_val_int == 11){
        motor_pwm_testing();
      } 
    else if(_val_int == 12){
        rs485_testing();
      } 
    else if(_val_int == 13){
        tm1638_testing();
      }         
  }


void ds3231_testing(void){
 Serial.print("Info: Intial DS3231...");
  Wire.begin();
  //**** setting time ***/
  Serial.print("Info: Setting DS3231...");
//    Clock.setYear(19);
//    Clock.setMonth(9);
//    Clock.setDate(14);
//    Clock.setDoW(7);
//    Clock.setHour(11);
//    Clock.setMinute(02);
//    Clock.setSecond(30);
    Serial.println("done");
  ds3231_getdata(); //**Need to call before display.init() **
  Serial.println("done");
        
  Serial.println("Info: DS3231 testing...");
  display.clear(); demos[clock_logo](); display.display();
  for (int i = 0; i < 10; i++) {
    sprintf(date_string, "   %d/%02d/20%02d", Clock.getDate(), Clock.getMonth(Century), Clock.getYear());
    sprintf(time_string, "   [%d:%02d:%02d]", Clock.getHour(h12, PM), Clock.getMinute(), Clock.getSecond());
    sprintf(temp_string, "T=%.2f C\r\n", Clock.getTemperature());
    Serial.printf("Debug: DATE: %s\r\n", date_string);
    Serial.printf("Debug: TIME: %s\r\n", time_string);
    Serial.printf("Debug: TEMP: %s\r\n", temp_string);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(time_string);
    lcd.setCursor(0, 1);
    lcd.print(date_string);

    delay(1000);
  }
  Serial.println("Info: Done\r\n\r\n"); 
  
  }

  void eeprom_testing(void){
    
/***************** External EEPROM Testing *****************/
  Serial.println("Info: External EEPROM testing...");
  display.clear(); demos[eeprom_logo](); display.display();
  for (int i = 0; i < 10; i++) {
    writeAddress(i, i * 10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Write[0x0"); lcd.print(i); lcd.print("]="); lcd.print(i * 10);
    Serial.printf("Debug: Write to Address[0x%2d] = %d\r\n", i, i * 10);
    lcd.setCursor(0, 1);
    lcd.print("Read[0x0"); lcd.print(i); lcd.print("]="); lcd.print(readAddress(i));
    Serial.printf("Debug: Readd to Address[0x%2d] = %d\r\n", i, readAddress(i));
    delay(1000);
  }
  Serial.println("Info: Done\r\n\r\n");    
    
    }


void temperature_sensor_testing(void){
 /***************** DS1820 Testing *****************/
  if(numberOfDevices != 0){
    Serial.println("Info: DS1820 testing...");
    display.clear(); demos[temperature_logo](); display.display();
    for (int i = 0; i < 10; i++) {
      Serial.print("Debug: Requesting temperatures...");
      sensors.requestTemperatures(); // Send the command to get temperatures
      Serial.println("DONE");
      // Loop through each device, print out temperature data
      for (int i = 0; i < numberOfDevices; i++)
      {
        // Search the wire for address
        if (sensors.getAddress(tempDeviceAddress, i))
        {
          // Output the device ID
          Serial.print("Temperature for device: ");
          Serial.println(i, DEC);
          // It responds almost immediately. Let's print out the data
          printTemperature(tempDeviceAddress); // Use a simple function to print out the data
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Temp C: "); lcd.print(printTemperature(tempDeviceAddress));
          lcd.setCursor(0, 1);
          lcd.print("Temp F: "); lcd.print(DallasTemperature::toFahrenheit(printTemperature(tempDeviceAddress)));
        }
      }
      delay(1000);
    }
    Serial.println("Info: Done\r\n\r\n");

  }
  else{
    Serial.println("Info: DHT22 testing...");
    display.clear(); demos[temperature_logo](); display.display();
    for (int i = 0; i < 10; i++) {
      TempAndHumidity newValues = dht.getTempAndHumidity();
      Serial.println("Info: DHT22 => T:" + String(newValues.temperature) + " H:" + String(newValues.humidity));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp : "); lcd.print(String(newValues.temperature));lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Humi : "); lcd.print(String(newValues.humidity));;lcd.print(" %");
      delay(1000);
    }
    Serial.println("Info: Done\r\n\r\n");   
  }  
  }    


  void ldr_testing(void){
   /***************** LDR Sensor Testing *****************/
  Serial.println("Info: LDR Sensor testing...");
  display.clear(); demos[ldr_logo](); display.display();
  for (int i = 0; i < 50; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** LDR Test ***");
    Serial.printf("Debug: LDR Testing\r\n");
    lcd.setCursor(0, 1);
    ldr_value = analogRead(LDR); //reads the LDR values
    lcd.print("LDR Value:"); lcd.print(ldr_value);
    Serial.printf("Debug: LDR Value = %d\r\n", ldr_value);
    delay(200);
  }
  Serial.println("Info: Done\r\n\r\n");   
    
    }


void adc_testing(void){
  /***************** ADC VR1 VR2 VR3 Testing *****************/
  Serial.println("Info: ADC VR1 VR2 VR3 Testing...");
  display.clear(); demos[potentiometer_logo](); display.display();
  for (int i = 0; i < 50; i++) {
    val_A3 = analogRead(VR_3);
    val_A2 = analogRead(VR_2);
    val_A1 = analogRead(VR_1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V1:"); lcd.print(val_A1); lcd.print(" V2:"); lcd.print(val_A2);
    lcd.setCursor(0, 1);
    lcd.print("V3:"); lcd.print(val_A3);
    Serial.printf("Debug: VR1=%d\r\n", val_A1);
    Serial.printf("Debug: VR2=%d\r\n", val_A2);
    Serial.printf("Debug: VR3=%d\r\n", val_A3);
    delay(200);
  }
  Serial.println("Info: Done\r\n\r\n");  
  
  }



 void led_rgb_testing(void){
 /***************** LED RGB Testing *****************/
  //select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("* LED RGB TEST *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");

  Serial.print("Info: Initial LED RGB...");
  ledcAttachPin(ledR, 1); // assign RGB led pins to channels
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);
  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);
  Serial.println("done");
  delay(5000);
  Logo_ticker.detach();
  display.clear(); demos[led_logo](); display.display();
  for (int i = 0; i < 1; i++) {
    ledcWrite(1, 255);
    ledcWrite(2, 255);
    ledcWrite(3, 255);
    delay(2000);
    Serial.println("Send all LEDs a 0 and wait 2 seconds.");
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    delay(2000);
    for (color = 0; color < 255; color++) { // Slew through the color spectrum
      hueToRGB(color, brightness);  // call function to convert hue to RGB
      // write the RGB values to the pins
      ledcWrite(1, R); // write red component to channel 1, etc.
      ledcWrite(2, G);
      ledcWrite(3, B);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("R: "); lcd.print(R); lcd.print(" G: "); lcd.print(G);
      lcd.setCursor(0, 1);
      lcd.print("B: "); lcd.print(B);
      delay(100); // full cycle of rgb over 256 colors takes 26 seconds
    }
  }
  ledcWrite(1, 0);
  ledcWrite(2, 0);
  ledcWrite(3, 0);
  Serial.println("Info: Done\r\n\r\n");    
    
    }


 void relay_testing(void){
 /***************** Relay Module Testing *****************/
  //select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*RELAY TESTING *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  digitalWrite(RELAY_1, OFF);
  digitalWrite(RELAY_2, OFF);
  delay(5000);
  Logo_ticker.detach();
  Serial.println("Info: Relay Module Testing...");
  display.clear(); demos[relay_logo](); display.display();
  for (int i = 0; i < 10; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*RELAY TESTING *");
    lcd.setCursor(0, 1);
    lcd.print("RY1:ON");
    lcd.setCursor(8, 1);
    lcd.print("RY2:OFF");
    digitalWrite(RELAY_1, ON);
    digitalWrite(RELAY_2, OFF);
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*RELAY TESTING *");
    lcd.setCursor(0, 1);
    lcd.print("RY1:OFF");
    lcd.setCursor(8, 1);
    lcd.print("RY2:ON");
    digitalWrite(RELAY_1, OFF);
    digitalWrite(RELAY_2, ON);
    delay(1000);
  }
  digitalWrite(RELAY_1, OFF);
  digitalWrite(RELAY_2, OFF);
  Serial.println("Info: Done\r\n\r\n");
  
  }   


  void motion_testing(void){
/***************** MOTION Sensor Testing *****************/
  //select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*MOTION SENSOR *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  pinMode(PIR, INPUT);
  delay(5000);
  Logo_ticker.detach();

  Serial.println("Info: Motion Sensor Testing...");
  display.clear(); demos[motion_logo](); display.display();
  for (int i = 0; i < 200; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*MOTION SENSOR *");
    lcd.setCursor(0, 1);
    if (digitalRead(PIR) == HIGH) {
      lcd.print("PIR: HIGH");
    }
    else {
      lcd.print("PIR: LOW");
    }
    delay(100);
  }

  Serial.println("Info: Done\r\n\r\n");    
    
    }


    void ultrasonic_testing(void){
 /***************** Ultrasonic Sensor Testing *****************/
  //select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*  ULTRASONIC  *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  pinMode(ULTRA_TRIG_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
  delay(5000);
  Logo_ticker.detach();

  Serial.println("Info: Motion Sensor Testing...");
  display.clear(); demos[ultrasonic_logo](); display.display();
  for (int i = 0; i < 100; i++) {
    digitalWrite(ULTRA_TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(ULTRA_TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRA_TRIG_PIN, LOW);
    unsigned int PulseWidth = pulseIn(ULTRA_ECHO_PIN, HIGH);
    unsigned int distance = PulseWidth * 0.0173681;

    //  Serial.print("Distance is ");
    Serial.print(distance);
    Serial.println(" cm.");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*  ULTRASONIC  *");
    lcd.setCursor(0, 1);
    lcd.print("Distance:"); lcd.print(distance); lcd.print(" cm");
    delay(200);
  }

  Serial.println("Info: Done\r\n\r\n");      
      
      }

  void stepping_testing(void){
    
   /***************** Stepping Motor Testing *****************/
  //select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*STEPPING MOTOR*");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  delay(5000);
  Logo_ticker.detach();

  Serial.println("Info: Stepping Motor Testing...");
  display.clear(); demos[stepping_logo](); display.display();
  for (int i = 0; i < 3; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*STEPPING MOTOR*");
    lcd.setCursor(0, 1);
    lcd.print("Rotate clockwise");
    for (int j = 0; j < 5; j++) {
      stepping_clockwise();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*STEPPING MOTOR*");
    lcd.setCursor(0, 1);
    lcd.print("counterclockwise");
    for (int j = 0; j < 5; j++) {
      stepping_counterclockwise();
    }

    stepping_stop();

  }

  Serial.println("Info: Done\r\n\r\n");   
    }    


    void motor_pwm_testing(void){
 /***************** PWM Motor Testing *****************/
  ledcAttachPin(MOTOR_1, 1);
  ledcAttachPin(MOTOR_2, 2);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);

  Serial.println("Info: PWM Motor DC Testing...");
  display.clear(); demos[motor_logo](); display.display();
  for (int i = 0; i < 100; i++) {
    ledcWrite(1, (i * 2.56));
    ledcWrite(2, (256 - (i * 2.56)));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOTOR1: "); lcd.print(i); lcd.print(" %");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR2: "); lcd.print(100 - i); lcd.print(" %");
    delay(200);
  }

  for (int i = 0; i < 100; i++) {
    ledcWrite(1, (256 - (i * 2.56)));
    ledcWrite(2, (i * 2.56));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOTOR1: "); lcd.print(100 - i); lcd.print(" %");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR2: "); lcd.print(i); lcd.print(" %");
    delay(200);
  }

    /*stop dc motor*/
    ledcWrite(1, 0);
    ledcWrite(2, 0); 
  Serial.println("Info: Done\r\n\r\n");     
      
      }

  void rs485_testing(void){
  /***************** RS485 Motor Testing *****************/
  //  select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*  RS485 TEST  *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  rs485.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(RS485_PIN_DIR, OUTPUT);
  digitalWrite(RS485_PIN_DIR, RS485_READ);
  delay(5000);
  Logo_ticker.detach();

  Serial.println("Info: RS485 Testing...");
  display.clear(); demos[rs485_logo](); display.display();
  for (int i = 0; i < 10; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RS485 SENT DATA ");
    lcd.setCursor(0, 1);
    lcd.print("->  Hello world ");
    digitalWrite(RS485_PIN_DIR, RS485_WRITE);
    rs485.printf("Hello world");
    delay(1000);
  }

  Serial.println("Info: Done\r\n\r\n");   
    
    }


void tm1638_testing(void){
 /***************** TM1638 Testing *****************/
  //  select switch
  logo_blink = dip3sw_logo;
  Logo_ticker.attach(0.3, oled_tick);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*  TM1638 TEST *");
  lcd.setCursor(0, 1);
  lcd.print(">SELECT DIP SW< ");
  pinMode(strobe, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  sendCommand(0x8f);  // activate
  reset();
  delay(5000);
  Logo_ticker.detach();

  Serial.println("Info: TM1638 Testing...");
  display.clear(); demos[tmp1638_logo](); display.display();
  uint8_t mode = COUNTING_MODE;
  
  for (int i = 0; i < 150; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*  TM1638 TEST *");
    
    
    switch(mode)
      {
      case COUNTING_MODE:
        lcd.setCursor(0, 1);
        lcd.print("-> COUNTING MODE");
        mode += counting();
        break;
      case SCROLL_MODE:
        lcd.setCursor(0, 1);
        lcd.print("-> SCROLL MODE  ");      
        mode += scroll();
        break;
      case BUTTON_MODE:
        lcd.setCursor(0, 1);
        lcd.print("-> BUTTON MODE  ");      
        buttons();
        break;
      }
    delay(200);
  }

  Serial.println("Info: Done\r\n\r\n");



    //lcd16x2 show tenergy innovation
  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*   Tenergy    *");
  lcd.setCursor(0, 1);
  lcd.print("*  Innovation  *");

  //oled show tenergy innovation
  display.clear();
  Serial.print("Info: tenergy logo...");
  demos[tenergy_logo]();
  display.display();
  delay(3000);
  Serial.println("done"); 
  
  }

  


/***********************************************************************
   FUNCTION:    drawProgressBarDemo
   DESCRIPTION: drawProgressBarDemo
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

/***********************************************************************
  FUNCTION:    tenergyImage
  DESCRIPTION: tenergyImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void tenergyImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, Tenergy_Logo_width, Tenergy_Logo_height, Tenergy_Logo_bits);
}

/***********************************************************************
  FUNCTION:    WiFiImage
  DESCRIPTION: WiFiImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void WiFiImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

/***********************************************************************
  FUNCTION:    hotspotImage
  DESCRIPTION: hotspotImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void WiFiOnImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_on_Logo_width, WiFi_on_Logo_height, WiFi_on_bits);
}


/***********************************************************************
  FUNCTION:    hotspotoffImage
  DESCRIPTION: hotspotoffImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void WiFiOffImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(47, 16, WiFi_off_Logo_width, WiFi_off_Logo_height, WiFi_off_bits);
}

/***********************************************************************
  FUNCTION:    HotspotImage
  DESCRIPTION: HotspotImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void HotspotImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, hotspot_Logo_width, hotspot_Logo_height, hotspot_bits);
}


/***********************************************************************
  FUNCTION:    WiFiCoffeeImage
  DESCRIPTION: WiFiCoffeeImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void WiFiCoffeeImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 14, wificoffee_Logo_width, wificoffee_Logo_height, wificoffee_bits);
}


/***********************************************************************
  FUNCTION:    FacebookImage
  DESCRIPTION: FacebookImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void FacebookImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 19, facebook_Logo_width, facebook_Logo_height, facebook_bits);
}

/***********************************************************************
  FUNCTION:    BugImage
  DESCRIPTION: BugImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void BugImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 19, bug_Logo_width, bug_Logo_height, bug_bits);
}

/***********************************************************************
  FUNCTION:    FixImage
  DESCRIPTION: FixImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void FixImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, maintenance_Logo_width, maintenance_Logo_height, maintenance_bits);
}


/***********************************************************************
  FUNCTION:    CameraImage
  DESCRIPTION: CameraImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void CameraImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(30, 10, camera_Logo_width, camera_Logo_height, camera_bits);
}

/***********************************************************************
  FUNCTION:    ClockImage
  DESCRIPTION: ClockImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void ClockImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(35, 10, clock_Logo_width, clock_Logo_height, clock_bits);
}

/***********************************************************************
  FUNCTION:    MotorImage
  DESCRIPTION: MotorImage
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void MotorImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(35, 10, motor_dc_Logo_width, motor_dc_Logo_height, motor_dc_bits);
}

/***********************************************************************
   FUNCTION:    EepromImage
   DESCRIPTION: EepromImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void EepromImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(30, 6, eeprom_Logo_width, eeprom_Logo_height, eeprom_bits);
}

/***********************************************************************
   FUNCTION:    HomeImage
   DESCRIPTION: HomeImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void HomeImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, home_Logo_width, home_Logo_height, home_bits);
}

/***********************************************************************
   FUNCTION:    LedImage
   DESCRIPTION: LedImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void LedImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, led_Logo_width, led_Logo_height, led_bits);
}

/***********************************************************************
   FUNCTION:    MotionImage
   DESCRIPTION: MotionImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void MotionImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, motion_Logo_width, motion_Logo_height, motion_bits);
}

/***********************************************************************
   FUNCTION:    RelayImage
   DESCRIPTION: RelayImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void RelayImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 10, relay_Logo_width, relay_Logo_height, relay_bits);
}

/***********************************************************************
   FUNCTION:    Rs485Image
   DESCRIPTION: Rs485Image
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void Rs485Image() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(38, 10, rs485_Logo_width, rs485_Logo_height, rs485_bits);
}

/***********************************************************************
   FUNCTION:    SteppingImage
   DESCRIPTION: SteppingImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void SteppingImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, stepping_Logo_width, stepping_Logo_height, stepping_bits);
}

/***********************************************************************
   FUNCTION:    TemperatureImage
   DESCRIPTION: TemperatureImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void TemperatureImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, temperature_Logo_width, temperature_Logo_height, temperature_bits);
}

/***********************************************************************
   FUNCTION:    Tmp1638Image
   DESCRIPTION: Tmp1638Image
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void Tmp1638Image() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(12, 5, tmp1638_Logo_width, tmp1638_Logo_height, tmp1638_bits);
}

/***********************************************************************
   FUNCTION:    UltrasonicImage
   DESCRIPTION: UltrasonicImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void UltrasonicImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(35, 8, ultrasonic_Logo_width, ultrasonic_Logo_height, ultrasonic_bits);
}


/***********************************************************************
   FUNCTION:    UtenImage
   DESCRIPTION: UtenImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void UtenImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 5, uten_Logo_width, uten_Logo_height, uten_bits);
}

/***********************************************************************
   FUNCTION:    UtenImage
   DESCRIPTION: UtenImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void PotentiometerImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 5, potentiometer_Logo_width, potentiometer_Logo_height, potentiometer_bits);
}

/***********************************************************************
   FUNCTION:    LdrImage
   DESCRIPTION: LdrImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void LdrImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 5, ldr_Logo_width, ldr_Logo_height, ldr_bits);
}

/***********************************************************************
   FUNCTION:    PayuthImage
   DESCRIPTION: PayuthImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void PayuthImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 5, payuth_Logo_width, payuth_Logo_height, payuth_bits);
}

/***********************************************************************
   FUNCTION:    Dip3swImage
   DESCRIPTION: Dip3swImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void Dip3swImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(20, 3, dip3sw_Logo_width, dip3sw_Logo_height, dip3sw_bits);
}


/***********************************************************************
   FUNCTION:    MjuImage
   DESCRIPTION: MjuImage
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void MjuImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(15, 3, mju_Logo_width, mju_Logo_height, mju_bits);
}


/***********************************************************************
  FUNCTION:    ds3231_getdata
  DESCRIPTION: Get data from DS3231
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void ds3231_getdata(void) {

  // send what's going on to the serial monitor.
  // Start with the year
  Serial.print("2");
  if (Century) {      // Won't need this for 89 years.
    Serial.print("1");
  } else {
    Serial.print("0");
  }
  Serial.print(Clock.getYear(), DEC);
  Serial.print(' ');
  // then the month
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print(' ');
  // then the date
  Serial.print(Clock.getDate(), DEC);
  Serial.print(' ');
  // and the day of the week
  Serial.print(Clock.getDoW(), DEC);
  Serial.print(' ');
  // Finally the hour, minute, and second
  Serial.print(Clock.getHour(h12, PM), DEC);
  Serial.print(' ');
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(' ');
  Serial.print(Clock.getSecond(), DEC);
  // Add AM/PM indicator
  if (h12) {
    if (PM) {
      Serial.print(" PM ");
    } else {
      Serial.print(" AM ");
    }
  } else {
    Serial.print(" 24h ");
  }
  // Display the temperature
  Serial.print("T=");
  Serial.print(Clock.getTemperature(), 2);
  // Tell whether the time is (likely to be) valid
  if (Clock.oscillatorCheck()) {
    Serial.print(" O+");
  } else {
    Serial.print(" O-");
  }
  // Indicate whether an alarm went off
  if (Clock.checkIfAlarm(1)) {
    Serial.print(" A1!");
  }
  if (Clock.checkIfAlarm(2)) {
    Serial.print(" A2!");
  }
  // New line on display
  Serial.print('\n');
  // Display Alarm 1 information
  Serial.print("Alarm 1: ");
  Clock.getA1Time(ADay, AHour, AMinute, ASecond, ABits, ADy, A12h, Apm);
  Serial.print(ADay, DEC);
  if (ADy) {
    Serial.print(" DoW");
  } else {
    Serial.print(" Date");
  }
  Serial.print(' ');
  Serial.print(AHour, DEC);
  Serial.print(' ');
  Serial.print(AMinute, DEC);
  Serial.print(' ');
  Serial.print(ASecond, DEC);
  Serial.print(' ');
  if (A12h) {
    if (Apm) {
      Serial.print('pm ');
    } else {
      Serial.print('am ');
    }
  }
  if (Clock.checkAlarmEnabled(1)) {
    Serial.print("enabled");
  }
  Serial.print('\n');
  // Display Alarm 2 information
  Serial.print("Alarm 2: ");
  Clock.getA2Time(ADay, AHour, AMinute, ABits, ADy, A12h, Apm);
  Serial.print(ADay, DEC);
  if (ADy) {
    Serial.print(" DoW");
  } else {
    Serial.print(" Date");
  }
  Serial.print(' ');
  Serial.print(AHour, DEC);
  Serial.print(' ');
  Serial.print(AMinute, DEC);
  Serial.print(' ');
  if (A12h) {
    if (Apm) {
      Serial.print('pm');
    } else {
      Serial.print('am');
    }
  }
  if (Clock.checkAlarmEnabled(2)) {
    Serial.print("enabled");
  }
  // display alarm bits
  Serial.print('\nAlarm bits: ');
  Serial.print(ABits, BIN);

  Serial.print('\n');
  Serial.print('\n');
  delay(1000);

}


/***********************************************************************
  FUNCTION:    writeAddress
  DESCRIPTION: Write Eeprom
  PARAMETERS:  address, val
  RETURNED:    nothing
***********************************************************************/
void writeAddress(int address, byte val)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB


  Wire.write(val);
  Wire.endTransmission();

  delay(5);
}


/***********************************************************************
  FUNCTION:    readAddress
  DESCRIPTION: Read Eeprom
  PARAMETERS:  address
  RETURNED:    byte
***********************************************************************/
byte readAddress(int address)
{
  byte rData = 0xFF;

  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();


  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);

  rData =  Wire.read();

  return rData;
}

/***********************************************************************
  FUNCTION:    printAddress
  DESCRIPTION: print a device address
  PARAMETERS:  addeviceAddressdress
  RETURNED:    none
***********************************************************************/
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

/***********************************************************************
  FUNCTION:    printTemperature
  DESCRIPTION: print a device address
  PARAMETERS:  deviceAddress
  RETURNED:    float
***********************************************************************/
// function to print the temperature for a device
float printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);
  Serial.print("Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  return tempC;

}

/***********************************************************************
   FUNCTION:    oled_tick
   DESCRIPTION: logo on oled blink
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void oled_tick()
{
  //toggle state
  static boolean _show_logo = 0;
  if (_show_logo) {
    display.clear(); demos[logo_blink](); display.display();
  }
  else {
    display.clear(); display.display();
  }
  _show_logo = !_show_logo;
  //    Serial.printf("_show_logo = %d\r\n",_show_logo);

}


/***********************************************************************
   FUNCTION:    hueToRGB
   DESCRIPTION: hueToRGB
   PARAMETERS:  hue,brightness
   RETURNED:    nothing
 ***********************************************************************/
// Courtesy http://www.instructables.com/id/How-to-Use-an-RGB-LED/?ALLSTEPS
// function to convert a color to its Red, Green, and Blue components.

void hueToRGB(uint8_t hue, uint8_t brightness)
{
  uint16_t scaledHue = (hue * 6);
  uint8_t segment = scaledHue / 256; // segment 0 to 5 around the
  // color wheel
  uint16_t segmentOffset =
    scaledHue - (segment * 256); // position within the segment

  uint8_t complement = 0;
  uint16_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
  uint16_t next = (brightness *  segmentOffset) / 256;

  if (invert)
  {
    brightness = 255 - brightness;
    complement = 255;
    prev = 255 - prev;
    next = 255 - next;
  }

  switch (segment ) {
    case 0:      // red
      R = brightness;
      G = next;
      B = complement;
      break;
    case 1:     // yellow
      R = prev;
      G = brightness;
      B = complement;
      break;
    case 2:     // green
      R = complement;
      G = brightness;
      B = next;
      break;
    case 3:    // cyan
      R = complement;
      G = prev;
      B = brightness;
      break;
    case 4:    // blue
      R = next;
      G = complement;
      B = brightness;
      break;
    case 5:      // magenta
    default:
      R = brightness;
      G = complement;
      B = prev;
      break;
  }
}


/***********************************************************************
   FUNCTION:    stepping_counterclockwise
   DESCRIPTION: stepping_counterclockwise
   PARAMETERS:  nothing
   RETURNED:    nothing
 ***********************************************************************/
void stepping_counterclockwise () {

  // 1

  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);

  // 2

  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay (motorSpeed);

  // 3

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);

  // 4

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);

  // 5

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(motorSpeed);

  // 6

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, HIGH);
  delay (motorSpeed);

  // 7

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(motorSpeed);

  // 8

  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(motorSpeed);

}


/***********************************************************************
  FUNCTION:    stepping_clockwise
  DESCRIPTION: stepping_clockwise
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void stepping_clockwise() {

  // 1

  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);

  // 2

  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);
  delay (motorSpeed);

  // 3

  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, LOW);

  delay(motorSpeed);

  // 4

  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);

  // 5

  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, LOW);
  delay(motorSpeed);

  // 6

  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin1, HIGH);
  delay (motorSpeed);

  // 7

  digitalWrite(motorPin4, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(motorSpeed);

  // 8

  digitalWrite(motorPin4, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
  delay(motorSpeed);

}

/***********************************************************************
  FUNCTION:    stepping_stop
  DESCRIPTION: stepping_stop
  PARAMETERS:  nothing
  RETURNED:    nothing
***********************************************************************/
void stepping_stop() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

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

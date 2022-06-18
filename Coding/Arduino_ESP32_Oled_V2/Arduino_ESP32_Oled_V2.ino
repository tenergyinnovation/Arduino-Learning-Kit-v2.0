/***********************************************************************
 * Project      :     Arduino_ESP32_Oled_V2
 * Description  :     Oled test          
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     14 Nov 2018
 * Revision     :     1.0
 * Rev1.0       :     Original
 * Facebook     :     https://www.facebook.com/tenergy.innovation
 * Email        :     tenergy.innovation@gmail.com
 * TEL          :     +6689-140-7205
 ***********************************************************************/

/**************************************/
/*         include library            */
/**************************************/ 
#include <DS3231.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <Ticker.h> //for LED status
// Include custom images
#include "images.h"



/**************************************/
/*           GPIO define              */
/**************************************/
#define BUTTON_SW1  33
#define BUTTON_SW2  14
#define LED         12
#define BUZZER      13
#define SLID_SW     15
#define INPUT_1     39
#define INPUT_2     34
#define INPUT_3     35
#define INPUT_4     32
#define OUTPUT_1    19
#define OUTPUT_2    18
#define OUTPUT_3    5
#define OUTPUT_4    2
#define ledR        25
#define ledG        26
#define ledB        27


/**************************************/
/*        object define               */
/**************************************/
SSD1306Wire  display(0x3c, 21, 22);  //21 = SDA, 22 = SCL
DS3231 Clock;
Ticker ticker;
Ticker Logo_ticker;


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




void ds3231_getdata(void);
void ds3231_setting(void);
void buzzer_beep(int times);
void tick();
void oled_tick();
typedef void (*Demo)(void);

#define DEMO_DURATION 3000


/**************************************/
/*        global variable             */
/**************************************/
int demoMode = 0;
int counter = 1;
Demo demos[] = {tenergyImage, drawProgressBarDemo, WiFiImage, WiFiOnImage, WiFiOffImage, HotspotImage, WiFiCoffeeImage, FacebookImage, BugImage, FixImage,
CameraImage, ClockImage, MotorImage, EepromImage, HomeImage, LedImage, MotionImage, RelayImage, Rs485Image, SteppingImage, TemperatureImage, Tmp1638Image,
UltrasonicImage, UtenImage };

int demoLength = (sizeof(demos) / sizeof(Demo));
//
uint8_t ledArray[3] = {1, 2, 3}; // three led channels
const boolean invert = true; // set true if common anode, false if common cathode
uint8_t color = 0;          // a value from 0 to 255 representing the hue
//
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
char date_string[20],time_string[20],temp_string[20];
/* NETPIE Parameter */


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
#define steping_logo        19
#define temperature_logo    20
#define tmp1638_logo        21
#define ultrasonic_logo     22
#define uten_logo           23

/***********************************************************************
 * FUNCTION:    drawProgressBarDemo
 * DESCRIPTION: drawProgressBarDemo
 * PARAMETERS:  nothing
 * RETURNED:    nothing
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
 * FUNCTION:    tenergyImage
 * DESCRIPTION: tenergyImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void tenergyImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, Tenergy_Logo_width, Tenergy_Logo_height, Tenergy_Logo_bits);
}

 /***********************************************************************
 * FUNCTION:    WiFiImage
 * DESCRIPTION: WiFiImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void WiFiImage() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

 /***********************************************************************
 * FUNCTION:    hotspotImage
 * DESCRIPTION: hotspotImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void WiFiOnImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(47, 16, WiFi_on_Logo_width, WiFi_on_Logo_height, WiFi_on_bits);
}


 /***********************************************************************
 * FUNCTION:    hotspotoffImage
 * DESCRIPTION: hotspotoffImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void WiFiOffImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(47, 16, WiFi_off_Logo_width, WiFi_off_Logo_height, WiFi_off_bits);
}

 /***********************************************************************
 * FUNCTION:    HotspotImage
 * DESCRIPTION: HotspotImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void HotspotImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(47, 18, hotspot_Logo_width, hotspot_Logo_height, hotspot_bits);
}


 /***********************************************************************
 * FUNCTION:    WiFiCoffeeImage
 * DESCRIPTION: WiFiCoffeeImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void WiFiCoffeeImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 14, wificoffee_Logo_width, wificoffee_Logo_height, wificoffee_bits);
}


 /***********************************************************************
 * FUNCTION:    FacebookImage
 * DESCRIPTION: FacebookImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void FacebookImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 19, facebook_Logo_width, facebook_Logo_height, facebook_bits);
}

 /***********************************************************************
 * FUNCTION:    BugImage
 * DESCRIPTION: BugImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void BugImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 19, bug_Logo_width, bug_Logo_height, bug_bits);
}

 /***********************************************************************
 * FUNCTION:    FixImage
 * DESCRIPTION: FixImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void FixImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, maintenance_Logo_width, maintenance_Logo_height, maintenance_bits);
}


 /***********************************************************************
 * FUNCTION:    CameraImage
 * DESCRIPTION: CameraImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void CameraImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(30, 10, camera_Logo_width, camera_Logo_height, camera_bits);
}

 /***********************************************************************
 * FUNCTION:    ClockImage
 * DESCRIPTION: ClockImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void ClockImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, clock_Logo_width, clock_Logo_height, clock_bits);
}

 /***********************************************************************
 * FUNCTION:    MotorImage
 * DESCRIPTION: MotorImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void MotorImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(35, 10, motor_dc_Logo_width, motor_dc_Logo_height, motor_dc_bits);
}

/***********************************************************************
 * FUNCTION:    EepromImage
 * DESCRIPTION: EepromImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void EepromImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(30, 6, eeprom_Logo_width, eeprom_Logo_height, eeprom_bits);
}

/***********************************************************************
 * FUNCTION:    HomeImage
 * DESCRIPTION: HomeImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void HomeImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, home_Logo_width, home_Logo_height, home_bits);
}

/***********************************************************************
 * FUNCTION:    LedImage
 * DESCRIPTION: LedImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void LedImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, led_Logo_width, led_Logo_height, led_bits);
}

/***********************************************************************
 * FUNCTION:    MotionImage
 * DESCRIPTION: MotionImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void MotionImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 15, motion_Logo_width, motion_Logo_height, motion_bits);
}

/***********************************************************************
 * FUNCTION:    RelayImage
 * DESCRIPTION: RelayImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void RelayImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 10, relay_Logo_width, relay_Logo_height, relay_bits);
}

/***********************************************************************
 * FUNCTION:    Rs485Image
 * DESCRIPTION: Rs485Image
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void Rs485Image(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(38, 10, rs485_Logo_width, rs485_Logo_height, rs485_bits);
}

/***********************************************************************
 * FUNCTION:    SteppingImage
 * DESCRIPTION: SteppingImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void SteppingImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, stepping_Logo_width, stepping_Logo_height, stepping_bits);
}

/***********************************************************************
 * FUNCTION:    TemperatureImage
 * DESCRIPTION: TemperatureImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void TemperatureImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(45, 15, temperature_Logo_width, temperature_Logo_height, temperature_bits);
}

/***********************************************************************
 * FUNCTION:    Tmp1638Image
 * DESCRIPTION: Tmp1638Image
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void Tmp1638Image(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(12, 5, tmp1638_Logo_width, tmp1638_Logo_height, tmp1638_bits);
}

/***********************************************************************
 * FUNCTION:    UltrasonicImage
 * DESCRIPTION: UltrasonicImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void UltrasonicImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(30, 8, ultrasonic_Logo_width, ultrasonic_Logo_height, ultrasonic_bits);
}


/***********************************************************************
 * FUNCTION:    UtenImage
 * DESCRIPTION: UtenImage
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void UtenImage(){
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(40, 5, uten_Logo_width, uten_Logo_height, uten_bits);
}





 /***********************************************************************
 * FUNCTION:    setup
 * DESCRIPTION: setup process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.println("************************************************");
  Serial.println("Project      :  Arduino_ESP32_Oled_V2");                 
  Serial.println("Author       :  Tenergy Innovation Co., Ltd.");
  Serial.println("Date         :  14 Nov 2018");
  Serial.println("Facebook     :  https://www.facebook.com/tenergy.innovation");
  Serial.println("Email        :  tenergy.innovation@gmail.com");
  Serial.println("TEL          :  +6689-140-7205");
  Serial.println("************************************************");


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

        display.clear();
      Serial.println("Info: tenergy logo...");
      demos[tenergy_logo]();
      display.display();
      delay(2000);
      Serial.println("done");
      
      display.clear();
    
      Serial.print("Info: progressive logo...");
      counter =0;
      do{
        display.clear();
        demos[progressive]();
        display.display();
        delay(5);
      }while(counter++ <= 498);
      Serial.println("done");

}



 /***********************************************************************
 * FUNCTION:    loop
 * DESCRIPTION: loop process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void loop() {
  


        for(int i=2; i<24; i++){
            display.clear();
            Serial.print("Info: wifi logo...");
            demos[i]();
            display.display();
            delay(2000);          
          
          }
        while(1);
//      display.clear();
//      Serial.println("Info: tenergy logo...");
//      demos[tenergy_logo]();
//      display.display();
//      delay(2000);
//      Serial.println("done");
//      
//      display.clear();
//    
//      Serial.print("Info: progressive logo...");
//      counter =0;
//      do{
//        display.clear();
//        demos[progressive]();
//        display.display();
//        delay(5);
//      }while(counter++ <= 498);
//      Serial.println("done");
//    
//    
//      display.clear();
//      Serial.print("Info: wifi logo...");
//      demos[wifi_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: wifi_on logo...");
//      demos[wifi_on_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: wifi_off logo...");
//      demos[wifi_off_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: hotspot logo...");
//      demos[hotspot_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: wificoffee logo...");
//      demos[wificoffee_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: facebook logo...");
//      demos[facebook_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: bug logo...");
//      demos[bug_logo]();
//      display.display();
//      delay(2000);
//    
//      display.clear();
//      Serial.print("Info: maintenance logo...");
//      demos[maintenance_logo]();
//      display.display();
//      delay(2000);
}

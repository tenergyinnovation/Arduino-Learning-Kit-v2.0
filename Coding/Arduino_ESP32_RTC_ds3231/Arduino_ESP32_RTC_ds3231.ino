/***********************************************************************
 * Project      :     Arduino_ESP32_RTC_ds3231
 * Description  :     ติดต่อกับไอซีฐานเวลา ds3231            
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     14 Nov 2018
 * Revision     :     1.0
 * Rev1.0       :     Original
 * website      :     http://www.tenergyinnovation.co.th
 * Facebook     :     https://www.facebook.com/tenergy.innovation
 * Email        :     uten.boonliam@innovation.co.th
 * TEL          :     089-140-7205

/**************************************/
/*         include library            */
/**************************************/
#include <DS3231.h>
#include <Wire.h>

/**************************************/
/*        object define             */
/**************************************/
DS3231 Clock;

/**************************************/
/*       Global variable define       */
/**************************************/
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;


 /***********************************************************************
 * FUNCTION:    setup
 * DESCRIPTION: setup process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void setup() {
	// Start the I2C interface
	Wire.begin();
	// Start the serial interface
	Serial.begin(9600);
  Serial.println("************************************************");
  Serial.println("Project      :  Arduino_ESP32_RTC_ds3231");                 
  Serial.println("Author       :  Tenergy Innovation Co., Ltd.");
  Serial.println("Date         :  14 Nov 2018");
  Serial.println("website      :  http://www.tenergyinnovation.co.th");
  Serial.println("Email        :  uten.boonliam@innovation.co.th");
  Serial.println("TEL          :  089-140-7205");
  Serial.println("************************************************");
  //Date
  Clock.setYear(19);
  Clock.setMonth(04);
  Clock.setDate(17);
  Clock.setDoW(4);
  //Time
  Clock.setHour(16);
  Clock.setMinute(40);
  
}


 /***********************************************************************
 * FUNCTION:    loop
 * DESCRIPTION: loop process
 * PARAMETERS:  nothing
 * RETURNED:    nothing
 ***********************************************************************/
void loop() {
	// send what's going on to the serial monitor.
	// Start with the year
	Serial.print("2");
	if (Century) {			// Won't need this for 89 years.
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

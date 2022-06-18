/***********************************************************************
 * Project      :     Arduino_Nano_Buzzer_test
 * Description  :     Buzzer test           
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     22 Nov 2019
 * Revision     :     1.0
 * Rev1.0       :     Original
 * Facebook     :     https://www.facebook.com/tenergy.innovation
 * WWW          :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +6689-140-7205
 ***********************************************************************/

/**************************************/
/*           GPIO define              */
/**************************************/ 
const int buzzer = 11; //buzzer to arduino pin 9

 
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec

}

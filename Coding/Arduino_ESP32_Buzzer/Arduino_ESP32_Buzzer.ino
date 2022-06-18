/***********************************************************************
 * Project      :     Arduino_ESP32_Buzzer_test
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
const int buzzer = 27; //buzzer to arduino pin 9


/**************************************/
/*           GPIO define              */
/**************************************/ 
void setup() {
  // put your setup code here, to run once:
    ledcAttachPin(buzzer, 1);
    ledcSetup(1, 2048, 8); // 2.048 kHz PWM, 8-bit resolution
    Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

//  Serial.println("buzzer -> ON"); 
    ledcWrite(1, 128 );
//  delay(1000);
//  Serial.println("buzzer -> OFF"); 
//    ledcWrite(1, 0 );
  delay(1000);

}

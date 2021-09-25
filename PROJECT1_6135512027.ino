//********************************************************************************************************************************
#include <ESP8266WiFi.h>          //เรียกใช้ Library ของบอร์ด NodeMCU ESP8266    
#include <BlynkSimpleEsp8266.h>   //เรียกใช้ Library ของ Blynk เพื่อใช้งานร่วมกับบอร์ด 
#define SENSOR1 D3                //กำหนด Pin ให้เซนเซอร์ที่ขา D3                    
#define SENSOR2 D4                //กำหนด Pin ให้เซนเซอร์ที่ขา D4
#define SENSOR3 D8                //กำหนด Pin ให้เซนเซอร์ที่ขา D8
WidgetLED LED_onBlynk1(V3);       //กำหนด Pin ให้ LED ใน Blynk เพื่อแสดงผลที่ V3
WidgetLED LED_onBlynk2(V6);       //กำหนด Pin ให้ LED ใน Blynk เพื่อแสดงผลที่ V6
WidgetLED LED_onBlynk3(V7);       //กำหนด Pin ให้ LED ใน Blynk เพื่อแสดงผลที่ V7
WidgetLCD LCD(V1);                //กำหนด Pin ให้ LCD ใน Blynk เพื่อแสดงผลที่ V1
//********************************************************************************************************************************
char auth[] = "qXxTQkBNcVDbyFJJbQwhTy4Z6M3pMLVK";   //auth สำหรับเชื่อมต่อกับ Blynk
char ssid[] = "Jefyboy(2.4G)";                      //ชื่อ WIFI ที่ใช้เชื่อมต่อกัน
char pass[] = "0841852030";                         //รหัสผ่าน
//********************************************************************************************************************************
long previousMillis1 = 0;         //กำหนดตัวแปรรับค่าการหมุนของใบพัดในรอบถัดไป = 0
long previousMillis2 = 0;         //กำหนดตัวแปรรับค่าการหมุนของใบพัดในรอบถัดไป = 0
long previousMillis3 = 0;         //กำหนดตัวแปรรับค่าการหมุนของใบพัดในรอบถัดไป = 0
volatile byte pulseCount1;        //กำหนดตัวแปรรับค่าการหมุนของใบพัด
volatile byte pulseCount2;        //กำหนดตัวแปรรับค่าการหมุนของใบพัด
volatile byte pulseCount3;        //กำหนดตัวแปรรับค่าการหมุนของใบพัด
byte pulse1Sec1 = 0;              //กำหนดตัวแปรรับค่าใบพัดในเวลา 1 วินาที = 0
byte pulse1Sec2 = 0;              //กำหนดตัวแปรรับค่าใบพัดในเวลา 1 วินาที = 0
byte pulse1Sec3 = 0;              //กำหนดตัวแปรรับค่าใบพัดในเวลา 1 วินาที = 0
float flowRate1;                  //กำหนดตัวแปรรับค่าอัตราการไหลของน้ำ
float flowRate2;                  //กำหนดตัวแปรรับค่าอัตราการไหลของน้ำ
float flowRate3;                  //กำหนดตัวแปรรับค่าอัตราการไหลของน้ำ
//*********************************************************************************************************************************
void IRAM_ATTR pulseCounter1()   //สร้างฟังก์ชั่นนับค่าการหมุนของใบพัด
{
  pulseCount1++;
}
void IRAM_ATTR pulseCounter2()   //สร้างฟังก์ชั่นนับค่าการหมุนของใบพัด
{
  pulseCount2++;
}
void IRAM_ATTR pulseCounter3()   //สร้างฟังก์ชั่นนับค่าการหมุนของใบพัด
{
  pulseCount3++;
}

//*********************************************************************************************************************************
void setup()                           //กำหนดค่าเริ่มต้น
{
  Serial.begin(115200);                //กำหนดค่า uart หรือ ความเร็วในการส่งข้อมูล = 115200
  Blynk.begin(auth, ssid, pass);       //ตรวจสอบ auth,ssid,pass
  Blynk.run();                         //ให้ Blynk เริ่มต้นการทำงาน
  LCD.clear();                         //ล้างค่าหน้าจอ
  LCD.print(0,0,"System Ready ...");   //ปริ้นข้อความทางหน้าจอ LCD
  Blynk.notify("เชื่อมต่อระบบได้แล้ว");     //แจ้งเตือนเป็นข้อความ                            *************** แจ้งเตือน ****************
  pinMode(SENSOR1, INPUT_PULLUP);      //กำหนดให้รับค่าจากเซนเซอร์
  pinMode(SENSOR2, INPUT_PULLUP);      //กำหนดให้รับค่าจากเซนเซอร์
  pinMode(SENSOR3, INPUT_PULLUP);      //กำหนดให้รับค่าจากเซนเซอร์
  pulseCount1 = 0;                     //กำหนดค่าจำนวนการหมุนของใบพัดให้เริ่มต้นที่ 0
  pulseCount2 = 0;                     //กำหนดค่าจำนวนการหมุนของใบพัดให้เริ่มต้นที่ 0
  pulseCount3 = 0;                     //กำหนดค่าจำนวนการหมุนของใบพัดให้เริ่มต้นที่ 0
  flowRate1 = 0.0;                     //กำหนดให้ค่าอัตราการไหลของน้ำเริ่มต้นที่ 0
  flowRate2 = 0.0;                     //กำหนดให้ค่าอัตราการไหลของน้ำเริ่มต้นที่ 0
  flowRate3 = 0.0;                     //กำหนดให้ค่าอัตราการไหลของน้ำเริ่มต้นที่ 0
  previousMillis1 = 0;                 //กำหนดค่าการหมุนของใบพัดในรอบถัดไปเริ่มต้นที่ 0
  previousMillis2 = 0;                 //กำหนดค่าการหมุนของใบพัดในรอบถัดไปเริ่มต้นที่ 0
  previousMillis3 = 0;                 //กำหนดค่าการหมุนของใบพัดในรอบถัดไปเริ่มต้นที่ 0
  attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);           //รับค่าจากเซนเซอร์1,ค่าการหมุน,ค่าอนาล็อก
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);           //รับค่าจากเซนเซอร์2,ค่าการหมุน,ค่าอนาล็อก
  attachInterrupt(digitalPinToInterrupt(SENSOR3), pulseCounter3, FALLING);           //รับค่าจากเซนเซอร์2,ค่าการหมุน,ค่าอนาล็อก
}
//*********************************************************************************************************************************
void loop()
{
  if (millis() - previousMillis1 > 1000)                                             //เมื่อการนับครบ 1 วินาที
  {
    pulse1Sec1 = pulseCount1;                                                       //ค่าการหมุนใน 1 วิ = จำนวนการหมุนของใบพัด
    pulseCount1 = 0;                                                                //จำนวนการหมุน = 0 (เพื่อเริ่มนับใหม่ในรอบเวลา 1 วินาทีถัดไป)
    flowRate1 = ((1000.0 / (millis() - previousMillis1)) * pulse1Sec1) / 7.5;       //จากสูตรหาอัตราการไหลของน้ำ Q=AV
    previousMillis1 = millis();                                                     //จดจำค่าการหมุนของรอบที่ผ่านมา
    Serial.print("Flow rate1 :");                                                   //ปริ้นข้อความ Flow rate
    Serial.print(int(flowRate1));                                                   //ปริ้นค่าอัตราการไหลของน้ำ
    Serial.print(" L/min");                                                         //ปริ้น L/min
    Serial.print("\n");                                                             //ขึ้นบรรทัดใหม่
    Blynk.virtualWrite(V2, flowRate1);                                              //Blynk แสดงค่าอัตราการไหลของน้ำผ่าน Widget แบบกราฟครึ่งวงกลม
    if (flowRate1 > 0.5 && flowRate1 < 4)
    {
      LED_onBlynk1.on();
      LCD.clear();                                                                  //ล้างค่าหน้าจอ
      LCD.print(0,0,"System Working..");                                            //ปริ้นข้อความทางหน้าจอ LCD
    }
    else if (flowRate1 > 4 )                                                        //สร้างเงื่อนไขหากอัตราการไหลของน้ำมากกว่า 0.5 L/min
    {
      LCD.clear();
      LCD.print(2,0,"SENSOR 1");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 1 สปริงเกอร์มีปัญหา !!!");                 
    }
    else
    { 
      LED_onBlynk1.off();
    }
  }
  //********************************************************************* SENSOR 1 ******************************************************************
  
  if (millis() - previousMillis2 > 1000)                                             //เมื่อการนับครบ 1 วินาที
  {
    pulse1Sec2 = pulseCount2;                                                       //ค่าการหมุนใน 1 วิ = จำนวนการหมุนของใบพัด
    pulseCount2 = 0;                                                                //จำนวนการหมุน = 0 (เพื่อเริ่มนับใหม่ในรอบเวลา 1 วินาทีถัดไป)
    flowRate2 = ((1000.0 / (millis() - previousMillis2)) * pulse1Sec2) / 7.5;       //จากสูตรหาอัตราการไหลของน้ำ Q=AV
    previousMillis2 = millis();                                                     //จดจำค่าการหมุนของรอบที่ผ่านมา
    Serial.print("Flow rate2 :");                                                   //ปริ้นข้อความ Flow rate
    Serial.print(int(flowRate2));                                                   //ปริ้นค่าอัตราการไหลของน้ำ
    Serial.print(" L/min");                                                         //ปริ้น L/min
    Serial.print("\n");                                                             //ขึ้นบรรทัดใหม่
    Blynk.virtualWrite(V4, flowRate2);                                              //Blynk แสดงค่าอัตราการไหลของน้ำผ่าน Widget แบบกราฟครึ่งวงกลม
    if (flowRate2 > 0.5 && flowRate2 < 4)
    {
      LED_onBlynk2.on();
      LCD.clear();                                                                  //ล้างค่าหน้าจอ
      LCD.print(0,0,"System Working..");                                            //ปริ้นข้อความทางหน้าจอ LCD
    }
    else if (flowRate2 > 4 )                                                        //สร้างเงื่อนไขหากอัตราการไหลของน้ำมากกว่า 0.5 L/min
    {
      LCD.clear();
      LCD.print(2,0,"SENSOR 2");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 2 สปริงเกอร์มีปัญหา !!!");                 
    }
    else
    { 
      LED_onBlynk1.off();
    }
  }
  //********************************************************************* SENSOR 2 ******************************************************************

  if (millis() - previousMillis3 > 1000)                                             //เมื่อการนับครบ 1 วินาที
  {
    pulse1Sec3 = pulseCount3;                                                       //ค่าการหมุนใน 1 วิ = จำนวนการหมุนของใบพัด
    pulseCount3 = 0;                                                                //จำนวนการหมุน = 0 (เพื่อเริ่มนับใหม่ในรอบเวลา 1 วินาทีถัดไป)
    flowRate3 = ((1000.0 / (millis() - previousMillis3)) * pulse1Sec3) / 7.5;       //จากสูตรหาอัตราการไหลของน้ำ Q=AV
    previousMillis3 = millis();                                                     //จดจำค่าการหมุนของรอบที่ผ่านมา
    Serial.print("Flow rate3 :");                                                   //ปริ้นข้อความ Flow rate
    Serial.print(int(flowRate3));                                                   //ปริ้นค่าอัตราการไหลของน้ำ
    Serial.print(" L/min");                                                         //ปริ้น L/min
    Serial.print("\n");                                                             //ขึ้นบรรทัดใหม่
    Blynk.virtualWrite(V5, flowRate3);                                              //Blynk แสดงค่าอัตราการไหลของน้ำผ่าน Widget แบบกราฟครึ่งวงกลม
    if (flowRate3 > 0.5 && flowRate3 < 4)
    {
      LED_onBlynk3.on();
      LCD.clear();                                                                  //ล้างค่าหน้าจอ
      LCD.print(0,0,"System Working..");                                            //ปริ้นข้อความทางหน้าจอ LCD
    }
    else if (flowRate3 > 4 )                                                        //สร้างเงื่อนไขหากอัตราการไหลของน้ำมากกว่า 0.5 L/min
    {
      LCD.clear();
      LCD.print(2,0,"SENSOR 3");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 3 สปริงเกอร์มีปัญหา !!!");                 
    }
    else
    { 
      LED_onBlynk1.off();
    }
  }
  //********************************************************************* SENSOR 3 ******************************************************************

  if (flowRate1 > 4 && flowRate2 > 4)
  {
      LCD.clear();
      LCD.print(0,0,"SENSOR 1 AND 2");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 1 และ 2 สปริงเกอร์มีปัญหา !!!");  
  }
  else if (flowRate2 > 4 && flowRate3 > 4)
  {
      LCD.clear();
      LCD.print(0,0,"SENSOR 2 AND 3");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 2 และ 3 สปริงเกอร์มีปัญหา !!!");  
  }
  else if (flowRate1 > 4 && flowRate3 > 4)
  {
      LCD.clear();
      LCD.print(0,0,"SENSOR 1 AND 3");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 1 และ 3 สปริงเกอร์มีปัญหา !!!");  
  }
  else if (flowRate1 > 4 && flowRate2 > 4 && flowRate3 > 4)
  {
      LCD.clear();
      LCD.print(0,0,"SENSOR 1 AND 2 AND 3");
      LCD.print(0,1,"Problem !!!");
      Blynk.notify("แถวที่ 1 และ 2 และ 3 สปริงเกอร์มีปัญหา !!!");  
  }
}

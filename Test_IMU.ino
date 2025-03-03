#include <POP32.h>



float pvYaw;
uint8_t rxCnt = 0, rxBuf[8];

void setup() {
  
  Serial1.begin(115200); 
  delay(100);
  sw_ok_press();
  beep();
  zeroYaw();

}

void loop() {
  ShowYaw();
  delay(1000);

}
void zeroYaw()  //กำหนดมุมเริ่มต้นระนาบพื้นโลกเป็น 0
{
  Serial1.write(0XA5);
  Serial1.write(0X54);
  delay(60);
  Serial1.write(0XA5);
  Serial1.write(0X55);
  delay(60);
  Serial1.write(0XA5);
  Serial1.write(0X52);
  delay(60);
}
bool getIMU() {//0-360 องศา
  while (Serial1.available()) {
    rxBuf[rxCnt] = Serial1.read();
    if (rxCnt == 0 && rxBuf[0] != 0xAA) return false;
    rxCnt++;
    if (rxCnt == 8) {
      rxCnt = 0;
      if (rxBuf[0] == 0xAA && rxBuf[7] == 0x55) {
        pvYaw = (int16_t)(rxBuf[1] << 8 | rxBuf[2]) / 100.f;
        if(pvYaw<0) pvYaw = 360.000+pvYaw;  //เพิ่มเติม
        return true;
      }
    }
  }
  return false;
}
void ShowYaw() {//0-360 หมุนทวนเข็มนาฬิกา
  while (1) 
  {
    if (getIMU()) 
    {
      oled.text(0, 0, "Yaw=%f  ", pvYaw);
      oled.show();
    }
  }
}

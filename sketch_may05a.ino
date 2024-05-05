#define OLED_SCK_PIN 22
#define OLED_SDA_PIN 21
#define LED_BUILDIN 15
#include <Arduino.h>
#include <U8x8lib.h>

#define SR04_TRIG_PIN 23    // Vị trí chân GPIO của ESP32 được nối với Trig của SR04
#define SR04_ECHO_PIN 5 

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE, OLED_SCK_PIN, OLED_SDA_PIN);

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;

void setup(void)
{   
   pinMode(SR04_TRIG_PIN, OUTPUT);
   pinMode(SR04_ECHO_PIN, INPUT);
   
   u8x8.begin();
   u8x8.setFont(u8x8_font_chroma48medium8_r);
  
   u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
   u8x8log.setRedrawMode(1);   // 0: Update screen with newline, 1: Update screen for every char  
}

float GetDistance() {
   digitalWrite(SR04_TRIG_PIN, LOW);  // Đưa chân Trig xuống mức thấp trong 2uS
   delayMicroseconds(2);
   digitalWrite(SR04_TRIG_PIN, HIGH);  //Gửi luồng siêu âm kéo dài 10uS
   delayMicroseconds(10);
   digitalWrite(SR04_TRIG_PIN, LOW);   //Tắt luồng siêu âm
   unsigned int microseconds = pulseIn(SR04_ECHO_PIN, HIGH); // Đợi cho tới khi có phản hồi
   return microseconds / 2.0 / 29.412;          // Từ thời gian hành trình tính toán khoảng cách
}

float distance;

void loop(void) {
   distance = GetDistance();
   char distanceStr[10];
   dtostrf(distance, 5, 2, distanceStr);
   u8x8log.print("Dis = ");
   u8x8log.print(distanceStr);
   u8x8log.print(" cm");
   u8x8log.print("\n");
   delay(500);
}

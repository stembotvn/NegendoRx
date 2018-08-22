#ifndef NegendoRx_h
#define NegendoRx_h

#include "RF24.h"
#include <SPI.h>
#include <EEPROM.h>
#include "ServoTimer2.h"

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


#define CE_PIN   A2
#define CSN_PIN A3
#define Led_pin A1
#define SET 3
#define buzzer 2
#define RGB 4
#define servo1_pin 7
#define servo2_pin 8
#define motor1A 6
#define motor1B 5
#define motor2A 9
#define motor2B 10

class NegendoRx
{
public:
	NegendoRx();

	void init();			
	void convertAdd();		// Chuyển đổi địa chỉ lưu từ EEPROM
	void setAddress();		// Nhận địa chỉ mặc định từ Transmitter
	void runM1(int speed);	
	void runM2(int speed);
	void moveForward(int speed); // Hàm chạy thẳng về phía trước
	void moveForward(int M1speed, int M2speed);		// Hàm chạy thẳng về phía trước với tham số động cơ 1 và 2 tùy chỉnh
	void moveBackward(int speed); 	// Hàm đi lùi
	void stop();					// Hàm dừng lại
	void setServo(int sv, int Angle);	// Hàm set góc servo
	void tone(uint16_t frequency, uint32_t duration);	// Hàm điều chỉnh âm điệu của còi
	void tick(int n, uint16_t frequency, int times);	
	void onLed();	// Bật đèn
	void offLed();	// Tắt đèn
private:
	ServoTimer2 servo1;
	ServoTimer2 servo2;

	const uint64_t _AddDefault = 0xF0F0F0F001LL;
	uint64_t _AddRandom;
	byte _readAdd;
	byte _address;
	int _Add[1];
	long _duration;
	long _startTime;
	long _timeout = 10000L;
};
#endif 
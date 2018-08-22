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

#define Right_angle	0
#define Left_angle	180
#define Center_angle	90

class NegendoRx
{
public:
	NegendoRx(){}

	void init();			// Hàm khởi tạo
	void convertAdd();		// Chuyển đổi địa chỉ lưu từ EEPROM
	void setAddress();		// Nhận địa chỉ ngẫu nhiên từ Transmitter
	void runM1(int speed);	// Set tốc độ và chiều quay của động cơ 1
	void runM2(int speed);	// Set tốc độ và chiều quay của động cơ 2
	void moveForward(int speed); // Hàm chạy thẳng về phía trước
	void moveForward(int M1speed, int M2speed);		// Hàm chạy thẳng về phía trước với tham số động cơ 1 và 2 tùy chỉnh
	void Forward(int speed, int Angle);	// Hàm chạy về phía trước, điểu chỉnh góc lái bằng servo
	void moveBackward(int speed); 	// Hàm đi lùi
	void moveRight(int speed);	// Hàm chạy về bên phải(dùng servo để điều chỉnh cơ cấu lái)
	void moveLeft(int speed);	// Hàm chạy về bên trái (dùng servo để điều chỉnh cơ cấu lái)
	void moveBackRight(int speed); 	// Hàm di chuyển lùi về phía bên phải (dùng servo điều chỉnh cơ cấu lái)
	void moveBackLeft(int speed);	// Hàm di chuyển lùi về phía bên trái (dùng servo điều chỉnh cơ cấu lái)
	void stop();					// Hàm dừng lại
	void setServo(int sv, int Angle);	// Hàm set góc servo
	void tone(uint16_t frequency, uint32_t duration);	// Hàm điều chỉnh âm điệu của còi
	void tick(int n, uint16_t frequency, int times);	
	void onLed();	// Bật đèn
	void offLed();	// Tắt đèn
private:
	ServoTimer2 servo1;
	ServoTimer2 servo2;
	RF24 radio= RF24(CE_PIN, CSN_PIN);

	const uint64_t _AddDefault = 0xF0F0F0F001LL; // Địa chỉ truyền tín hiệu NRF24L01 mặc định
	uint64_t _AddRandom;	// Địa chỉ set ngẫu nhiên
	byte _readAdd;
	byte _address;
	int _Add[1];
	long _duration;
	long _startTime;
	long _timeout = 10000L;
};
#endif 
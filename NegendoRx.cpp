#include "NegendoRx.h"
#include "RF24.h"
#include "EEPROM.h"

void NegendoRx::init()
{
	Serial.begin(9600);

	pinMode(SET, INPUT);
	pinMode(Led_pin, OUTPUT);
	pinMode(buzzer, OUTPUT);
	pinMode(RGB, OUTPUT);
	pinMode(motor1A, OUTPUT);
	pinMode(motor1B, OUTPUT);
	pinMode(motor2A, OUTPUT);
	pinMode(motor2B, OUTPUT);

	radio.begin();
	radio.setChannel(108);
	radio.setDataRate(RF24_1MBPS);
	radio.setPALevel(RF24_PA_HIGH);
	convertAdd();
}
void NegendoRx::convertAdd()
{
	_readAdd = EEPROM.read(0);
	_AddRandom = (_AddDefault & ~0xFFLL) | _readAdd;
	radio.openReadingPipe(1,_AddRandom);
	radio.startListening();
	Serial.print("Address: ");
	Serial.print((unsigned long)(_AddRandom >> 32), 16);
	Serial.println((unsigned long)_AddRandom, 16); 
	Serial.println("Ready to receive data");
}
void NegendoRx::setAdddress()
{
	if(!digitalRead(SET))
	{
		Serial.println("Set Address");
		Serial.println("Wait 5s...");
		_startTime = millis();
		while(!digitalRead(SET));
		_duration = millis() - _startTime;
		if(_duration > 5000)
		{
			radio.openReadingPipe(1,_AddDefault);
			radio.startListening();
			Serial.println("Ready to receive new address...");
			for(unsigned long starts = millis(); (millis() - starts) < _timeout;)
			{
				if(radio.available())
				{
					radio.read(_Add, sizeof(_Add));
					_address = _SAdd[0];
					EEPROM.write(0,_address);
					Serial.println("Set address done.");
					break;
				}
			}
			convertAdd();
		}
		else
			Serial.println("No change of address.");
	}
}
void NegendoRx::runM1(int speed)
{
	speed = speed > 255 ? 255 : speed;   // if speed > 255 -> speed = 255
	speed = speed < -255 ? -255 : speed; // if speed < -255 -> speed = -255
	if(speed >= 0)
	{
		analogWrite(motor1A,speed);
		analogWrite(motor1B,0);
	}
	else
	{
		analogWrite(motor1A,0);
		analogWrite(motor1B,-speed);
	}	
}
void NegendoRx::runM2(int speed)
{
	speed = speed > 255 ? 255 : speed;   // if speed > 255 -> speed = 255
	speed = speed < -255 ? -255 : speed; // if speed < -255 -> speed = -255
	if(speed >= 0)
	{
		analogWrite(motor2A,speed);
		analogWrite(motor2B,0);
	}
	else
	{
		analogWrite(motor2A,0);
		analogWrite(motor2B,-speed);
	}
}
void NegendoRx::stop()
{
	analogWrite(motor1A, 0);
	analogWrite(motor1B, 0);
	analogWrite(motor2A, 0);
	analogWrite(motor2B, 0);
}
void NegendoRx::moveForward(int speed)
{
	int movespeed = speed*255/100; 
	runM1(movespeed);
	runM2(movespeed);
}
void NegendoRx::moveBackward(int speed)
{
	int movespeed = speed*255/100;
	runM1(-movespeed);
	runM2(-movespeed);
}
void NegendoRx::moveForward(int M1speed, int M2speed)
{
	int speedM1 = M1speed*255/100;
	int speedM2 = M2speed*255/100;
	runM1(speedM1);
	runM2(speedM2);
}
void NegendoRx::setServo(int sv, int Angle);
{
	if(sv = 1)
	{
		servo1.attach(servo1_pin);
		servo1.write(Angle);
	}
	else if(sv = 2)
	{
		servo2.attach(servo2_pin);
		servo2.write(Angle);
	}
}
void NegendoRx::tone(uint16_t frequency, uint32_t duration)
{
	int period = 1000000L / frequency;
	int pulse = period / 2;
	for (long i = 0; i < duration * 1000L; i += period)
	{
		digitalWrite(buzzer, HIGH);
		delayMicroseconds(pulse);
		digitalWrite(buzzer, LOW);
		delayMicroseconds(pulse);
	}
}
void NegendoRx::tick(int n, uint16_t frequency, int times)
{
	for(int i=0; i<n; i++)
	{
		tone(frequency, times);
		digitalWrite(buzzer, LOW);
		delay(times);
	}
}
void NegendoRx::onLed()
{
	digitalWrite(Led_pin, LOW);
}
void NegendoRx::offLed()
{
	digitalWrite(Led_pin, HIGH);
}

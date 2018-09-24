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
void NegendoRx::setAddress()
{
	if(!digitalRead(SET))
	{
		Serial.println("Set Address");
		Serial.println("Wait 5s...");
		_oldAdd = EEPROM.read(0);
		_startTime = millis();
		while(!digitalRead(SET))
		{
			if(millis() - _startTime == 5000)
			{
				Sound.sing(S_mode3);
			}
		}
		_duration = millis() - _startTime;
		if(_duration > 5000)
		{
			radio.openReadingPipe(1,_AddDefault);
			radio.startListening();
			Serial.println("Ready to receive new address...");
			for(unsigned long starts =  millis(); (millis() - starts) < _timeout;)
			{
				if(radio.available())
				{
					radio.read(_Add, sizeof(_Add));
					_address = _Add[0];
					EEPROM.write(0,_address);
					Serial.println("Set address done.");
					Sound.sing(S_connection);
					break;
				}
			}
			_newAdd = EEPROM.read(0);
			if(_oldAdd == _newAdd || _newAdd == 01)
			{
				Serial.println("The address doesn't change.");
				Sound.sing(S_cuddly);
			}
			convertAdd();
		}
		else
		{
			Serial.println("The address doesn't change.");
			Sound.sing(S_cuddly);
		}
	}
}
void NegendoRx::runM1(int speed)
{
	speed = speed > 255 ? 255 : speed;   // if speed > 255 => speed = 255
	speed = speed < -255 ? -255 : speed; // if speed < -255 => speed = -255
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
	int speedMotor = speed*255/100; 
	runM1(speedMotor);
	runM2(speedMotor);
}
void NegendoRx::moveBackward(int speed)
{
	int speedMotor = speed*255/100;
	runM1(-speedMotor);
	runM2(-speedMotor);
}
void NegendoRx::moveForward(int M1speed, int M2speed)
{
	int speedM1 = M1speed*255/100;
	int speedM2 = M2speed*255/100;
	runM1(speedM1);
	runM2(speedM2);
}
void NegendoRx::Forward(int speed, int Angle)
{
	int speedMotor = speed*255/100; 
	runM1(speedMotor);
	runM2(speedMotor);
	setServo1(Angle);
}
void NegendoRx::moveRight(int speed)
{
	int speedMotor = speed*255/100;
	runM1(speedMotor);
	runM2(speedMotor);
	setServo1(Right_angle);
}
void NegendoRx::moveLeft(int speed)
{
	int speedMotor = speed*255/100;
	runM1(speedMotor);
	runM2(speedMotor);
	setServo1(Left_angle);
}
void NegendoRx::moveBackRight(int speed)
{
	int speedMotor = speed*255/100;
	runM1(-speedMotor);
	runM2(-speedMotor);
	setServo1(Right_angle);
}
void NegendoRx::moveBackLeft(int speed)
{
	int speedMotor = speed*255/100;
	runM1(-speedMotor);
	runM2(-speedMotor);
	setServo1(Left_angle);
}
void NegendoRx::turnRight(int speed)
{
	int speedMotor = speed*255/100;
	runM1(-speedMotor);
	runM2(speedMotor);
}
void NegendoRx::turnLeft(int speed)
{
	int speedMotor = speed*255/100;
	runM1(speedMotor);
	runM2(-speedMotor);
}
void NegendoRx::setServo1(int Angle)
{
	servo1.attach(servo1_pin);
	servo1.write(Angle);
}
void NegendoRx::setServo2(int Angle)
{
	servo2.attach(servo2_pin);
	servo2.write(Angle);
}
void NegendoRx::tones(uint16_t frequency, uint32_t duration)
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
		tones(frequency, times);
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
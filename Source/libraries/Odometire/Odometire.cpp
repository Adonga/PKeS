#include "Odometire.h"
#include <Arduino.h>
  Odometrie::Odometrie()
  {
  }

  void Odometrie::init()
	{
		Serial.println("Bla");	
//		DDRD |= ( 1 << PD2 );
		PORTD |= (1<< PD2);
		_delay_ms(1);
		DDRJ |= (1 << PJ0);
		PORTJ |= (1 << PJ0);
		EICRA |= ( 1 << ISC11);
		EICRA |= ( 1 << ISC10);
		
		EIMSK |= ( 1 << INT2);
    sei();
  }

	void Odometrie::test()
	{
		Serial.println("Test1");
	}

	void Odometrie::test2()
	{
		Serial.println("Test2");
	}

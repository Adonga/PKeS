#include "Odometire.h"
#include <Arduino.h>
  Odometrie::Odometrie()
  {
  }

  void Odometrie::init()
	{
			cli();
//		DDRD |= ( 1 << PD2 );
		PORTD |= (1<< PD2);

		EICRA |= ( 1 << ISC21);
//	  EICRA |= ( 1 << ISC20);
		Serial.print("EICRA ");
		Serial.println(EICRA);
		EIFR = (1 << INTF2);
		EIMSK |= ( 1 << INT2);
    sei();



//		DDRJ |= (1 << PJ0);
		PORTJ |= (1 << PJ0);

		PCMSK1 |= (1 << PCINT9);
		PCICR |= (1 << PCIE1);


  }

	void Odometrie::test()
	{
		Serial.println("Test1");
	}

	void Odometrie::test2()
	{
		Serial.println("Test2");
	}
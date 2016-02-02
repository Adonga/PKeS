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

		EICRA |= ( 1 << ISC21); //fallede Flanke ein Interrupt
//	  EICRA |= ( 1 << ISC20); //+ das waere steigende Flanke Interrupt 
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
	int Odometrie::distanceR(int right)
	{
		/*ds = pi * d * n
		n ... anzahl Rad umdrehungen
		d ... druchmesser Rad  ca 5cm */
		distanceDrivenR = (314 * right * 500)  /1800;  // should be cm
		return distanceDrivenR;
	}

	int Odometrie::distanceL(int left)
	{
		/*ds = pi * d * n
		n ... anzahl Rad umdrehungen
		d ... druchmesser Rad  ca 5cm */
		distanceDrivenL = (314 * left * 500)  /1800;  // should be cm
		return distanceDrivenL;
	}





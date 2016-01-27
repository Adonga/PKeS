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
	int Odometrie::distance(int intr1, int intr2)
	{
		/*ds = pi * d * n
		n ... anzahl Rad umdrehungen
		d ... druchmesser Rad  ca 5cm */
		distanceDrivenL = (314 * intr1 * 500)  /18;  
		distanceDrivenR = (314 * intr2 * 500)  /18;  
		return distanceDrivenL>distanceDrivenR?distanceDrivenL:distanceDrivenR;
	}



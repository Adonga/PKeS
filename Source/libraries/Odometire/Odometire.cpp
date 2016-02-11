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

//		EICRA |= ( 1 << ISC21); //fallede Flanke ein Interrupt
	  EICRA |= ( 1 << ISC20); //+ das waere steigende Flanke Interrupt 
		//Serial.print("EICRA ");
		//Serial.println(EICRA);
		EIFR = (1 << INTF2);
		EIMSK |= ( 1 << INT2);
    sei();

//		DDRJ |= (1 << PJ0);
		PORTJ |= (1 << PJ0);

		PCMSK1 |= (1 << PCINT9);  //linke scheibe
		PCICR |= (1 << PCIE1);
  }
	int32_t Odometrie::distanceR()
	{
		/*ds = pi * d * n
		n ... anzahl Rad umdrehungen
		d ... druchmesser Rad  ca 5cm */
		distanceDrivenR = (3.14 * interrupt1 * 500)  /1800;  // should be cm
		return distanceDrivenR;
	}

	int32_t Odometrie::distanceL()
	{
		/*ds = pi * d * n
		n ... anzahl Rad umdrehungen
		d ... druchmesser Rad  ca 5cm */
		distanceDrivenL = (3.14 * interrupt2 * 500)  /1800;  // should be cm
		return distanceDrivenL;
	}

	void Odometrie::reset(){interrupt1= 0; interrupt2 = 0;}

  int Odometrie::left(){return interrupt2;}
	int Odometrie::right(){return interrupt1;}




#ifndef ODOMETRIE_H
#define ODOMETRIE_H
#include "avr/interrupt.h"
	
class Odometrie
{
	public:
		int distanceDrivenL;
		int distanceDrivenR;
		Odometrie();
		void init();
		int distanceR( int right );
		int distanceL( int left );
	private:


};


#endif

#ifndef ODOMETRIE_H
#define ODOMETRIE_H
#include "avr/interrupt.h"
	
class Odometrie
{
	public:
		int32_t distanceDrivenL;
		int32_t distanceDrivenR;
		Odometrie();
		void init();
		int32_t distanceR( );
		int32_t distanceL( );
		int left( );
		int right( );
		volatile int interrupt1; //right
		volatile int interrupt2; //left
		void reset();
		
// get left and right ticks
// get disance driven by  each weel
// sth that resets the ticks
// 
	private:


};


#endif

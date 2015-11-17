// Velocidad.h
// 
// Copyright 2015 Alex Vargas Benamburg <alex@algecr.tk>
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
// 
// 


#ifndef VELOCIDAD_H
#define VELOCIDAD_H
#include "mbed.h"

class Velocidad{
	public:
		Velocidad(
			PinName Ad, PinName a,
			PinName Bd, PinName b
			);
		virtual ~Velocidad();
		void init(void);
		
		void static decAfr(void);
		void static decAff(void);
		void static decBfr(void);
		void static decBff(void);
		void static update(void);
		
		float get(uint8_t);
		void getV(int16_t[]);
	
	private:
		static DigitalIn* decAd;
		static InterruptIn* decA;
		static DigitalIn* decBd;
		static InterruptIn* decB;

		Ticker timer;
		
		static int16_t promA;
		static int16_t promB;
		
		volatile static int16_t pasosDecA;
		volatile static int16_t pasosDecB;
		
};

#endif /* VELOCIDAD_H */ 

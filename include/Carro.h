// Carro.h
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
#ifndef CARRO_H
#define CARRO_H

#include "mbed.h"
#include "Ticker.h"
#include "Traccion.h"
#include "Velocidad.h"

#define PWM_FRQ 100 //100Hz
#define VEL_PERCENT 0.05f
class Carro{
	public:
		Velocidad velocidad;
		
		Carro(
			PinName chanAdecI, 	PinName chanBdecI,
			PinName chanAdecD, 	PinName chanBdecD,
			
			PinName pwmMI,		PinName dirMI,
			PinName pwmMD,		PinName dirMD
			);
		void init(void);
		void set(float velI,float velD);
		static void update(void);
		
		//GETERS
		Velocidad* 	getV(void);
		Traccion* 	getT(void);
	private:
		uint8_t velI;
		uint8_t velD;
		//Velocidad velocidad;
		Traccion traccion;
		static Carro* instancia;
		Ticker timer;
		
		// Temporales como globales para optimisacion.
		float pwmActuales[2];
		int16_t velsActuales[2];
		int16_t diferencias[2];
};

#endif //CARRO_H

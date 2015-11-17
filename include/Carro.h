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
#include "PID.h"
class Carro{
	private:
		Traccion motores;
		Velocidad movimiento;
		PID pidI;
		PID pidD;
		static Carro* instancia;
		Ticker timer;
	public:
		Carro(
			PinName chanAdecI, 	PinName chanBdecI,
			PinName chanAdecD, 	PinName chanBdecD,
			
			PinName pwmMI,		PinName dirMI,
			PinName pwmMD,		PinName dirMD
			);
		void set(float velI,velD);
		void get(void);
	
};

#endif //CARRO_H

// Carro.cpp
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


#include "Carro.h"
extern Carro* Carro::instancia;

Carro::Carro(PinName chanAdecI, PinName chanBdecI,
			 PinName chanAdecD, PinName chanBdecD,
			 PinName pwmMI,	PinName dirMI,
			 PinName pwmMD,	PinName dirMD
			):velocidad(chanAdecI,chanBdecI,chanAdecD,chanBdecD),
			  traccion(pwmMI,dirMI,pwmMD,dirMD){
				Carro::instancia = this;
				traccion.setPWMFrequency(PWM_FRQ);
}

void Carro::init(void){
    this->traccion.init();
    this->velocidad.init();
	//this->timer.attach(Carro::update,0.015);
}

void Carro::update(void){
	instancia->velocidad.getV(instancia->velsActuales);
	instancia->traccion.get(instancia->pwmActuales);
	
	if(instancia->velsActuales[0]!=instancia->velI){
		instancia->pwmActuales[0] += ((float)instancia->velsActuales[0])/((float)instancia->velI);
	}
	if(instancia->velsActuales[1]!=instancia->velD){
		instancia->pwmActuales[1] += ((float)instancia->velsActuales[1])/((float)instancia->velD);
	}
	instancia->traccion.set(instancia->pwmActuales[0],instancia->pwmActuales[1]);
}

void Carro::set(float velI,float velD){
	this->velI = velI;
	this->velD = velD;
}

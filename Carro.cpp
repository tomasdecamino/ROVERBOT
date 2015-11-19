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
			):velocidadI(chanAdecI,chanBdecI,NC,15,QEI::X4_ENCODING),
			  velocidadD(chanAdecD,chanBdecD,NC,15,QEI::X4_ENCODING),
			  traccion(pwmMI,dirMI,pwmMD,dirMD),
			  pidI(0.8, 0.2, 0.0001, 0.03),
			  pidD(0.8, 0.2, 0.0001, 0.03)
			  {
				Carro::instancia = this;
}

void Carro::init(void){
    this->traccion.init(PWM_FRQ);
	this->pidI.setInputLimits(-30,30);
	this->pidD.setInputLimits(-30,30);
	this->pidI.setOutputLimits(-1.0,1.0);
	this->pidD.setOutputLimits(-1.0,1.0);
	this->pidI.setMode(AUTO_MODE);
	this->pidD.setMode(AUTO_MODE);
	this->timer.attach(Carro::update,0.030);
}

void Carro::update(void){
	instancia->velsActuales[0] = instancia->velocidadI.getPulses();
	instancia->velocidadI.reset();
	instancia->velsActuales[1] = instancia->velocidadD.getPulses();
	instancia->velocidadD.reset();
	
	instancia->pidI.setProcessValue(instancia->velsActuales[0]);
	instancia->pidD.setProcessValue(instancia->velsActuales[1]);
	
	instancia->traccion.set(instancia->pidI.compute(),instancia->pidD.compute());
	
	/*
	instancia->traccion.get(instancia->pwmActuales);
	if(instancia->velI > instancia->velsActuales[0]){
		instancia->pwmActuales[0] += VEL_PERCENT;
	}else{
		if(instancia->velI < instancia->velsActuales[0]){
			instancia->pwmActuales[0] -= VEL_PERCENT;
		}
	}
	
	if(instancia->velD > instancia->velsActuales[1]){
		instancia->pwmActuales[1] += VEL_PERCENT;
	}else{
		if(instancia->velD < instancia->velsActuales[1]){
			instancia->pwmActuales[1] -= VEL_PERCENT;
		}
	}
	instancia->traccion.set(instancia->pwmActuales[0],instancia->pwmActuales[1]);*/
	
}

void Carro::setPIDConstants(float k,float i,float d){
	this->pidI.setTunings(k,i,d);
	this->pidD.setTunings(k,i,d);
}

void Carro::set(int16_t velI,int16_t velD){
	//this->velI = velI;
	//this->velD = velD;
	this->pidI.setSetPoint(velI);
	this->pidD.setSetPoint(velD);
}

int16_t Carro::get(uint8_t index){
	return this->velsActuales[index];
}

// Velocidad.cpp
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


#include "Velocidad.h"

extern DigitalIn* Velocidad::decAd;
extern InterruptIn* Velocidad::decA;
extern DigitalIn* Velocidad::decBd;
extern InterruptIn* Velocidad::decB;
extern float Velocidad::promA;
extern float Velocidad::promB;
extern volatile int16_t Velocidad::pasosDecA;
extern volatile int16_t Velocidad::pasosDecB;

Velocidad::Velocidad(
			PinName Ad, PinName a,
			PinName Bd, PinName b
			){
	Velocidad::decAd = new DigitalIn(Ad);
	Velocidad::decA = new InterruptIn(a);
	Velocidad::decBd = new DigitalIn(Bd);
	Velocidad::decB = new InterruptIn(b);
}

Velocidad::~Velocidad(){
		delete decAd;
		delete decA;
		delete decBd;
		delete decB;
}

void Velocidad::init(void){
	this->decA->rise(this->decAfr);
	this->decA->fall(this->decAff);

	this->decB->rise(this->decBfr);
	this->decB->fall(this->decBff);

	this->timer.attach(this->update,0.02);
}

void Velocidad::decAfr(void){
	if(decAd->read()){
		--pasosDecA;
	}else{
		++pasosDecA;
	}
}

void Velocidad::decAff(void){
	if(!decAd->read()){
		--pasosDecA;
	}else{
		++pasosDecA;
	}
}

void Velocidad::decBfr(void){
	if(decBd->read()){
		--pasosDecB;
	}else{
		++pasosDecB;
	}
}

void Velocidad::decBff(void){
	if(!decBd->read()){
		--pasosDecB;
	}else{
		++pasosDecB;
	}
}

void Velocidad::update(void){
	promA = pasosDecA*60.0/180.0;
	promB = pasosDecB*60.0/180.0;
	pasosDecA = 0;
	pasosDecB = 0;
}

float Velocidad::get(uint8_t i){
	switch(i){
		case 0:
			return promA;
		case 1:
			return promB;
		default:
			return 0.0/0.0;
	}
}

void Velocidad::getV(float resp[]){
	resp[0] = promA;
	resp[1] = promB;
}

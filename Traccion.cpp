// Traccion.cpp
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

#include "Traccion.h"
Traccion::Traccion(
            PinName iv, PinName id,
            PinName dv, PinName dd
            ):
                    dirI(id),velI(iv),
                    dirD(dd),velD(dv){
}

extern Serial pc;

Traccion::~Traccion(){}

void Traccion::init(void){
    this->velI.write(0);
    this->velD.write(0);
    this->dirI.write(0);
    this->dirD.write(0);
    this->stop();
}

void Traccion::stop(bool brute){
    if(brute){
        velI.write(0);
        velD.write(0);
        dirI.write(0);
        dirD.write(0);
    }else{
        //TODO
    }
}

void Traccion::set(float veli,float veld){
    pc.printf("veli:%f  veld:%f",veli,veld);
    this->velocidades[0] = veli;
    this->velocidades[1] = veld;
    if(veld<0.0f){
        this->dirD = 1;
        veld = 1.0f+veld;
    }else{
        this->dirD = 0;
    }
    if(veli<0.0f){
        this->dirI = 1;
        veli = 1.0f+veli;
    }else{
        this->dirI = 0;
    }
    this->velI = veli;
    this->velD = veld;
}

void Traccion::get(float pwm[]){
	pwm[0] = this->velocidades[0];
	pwm[1] = this->velocidades[1];
}

void Traccion::setPWMFrequency(uint16_t freq){
	uint32_t us =1000000UL/(uint32_t)freq;
	this->velI.pulsewidth_us(us);
	this->velD.pulsewidth_us(us);
}

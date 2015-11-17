// Traccion.h
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


#ifndef TRACCION_H
#define TRACCION_H

#include "mbed.h"
#include "PwmOut.h"
#include "DigitalOut.h"
    
class Traccion{
    public:
        Traccion(
            PinName iv, PinName id,
            PinName dv, PinName dd
        );
        void init(void);
        void stop(bool brute=true);
        void set(float veli,float veld);
        void setPWMFrequiency(uint16_t);
        ~Traccion(void);
            
    private:
        DigitalOut dirI;
        PwmOut velI;
        DigitalOut dirD;
        PwmOut velD;

};

#endif /* TRACCION_H */ 

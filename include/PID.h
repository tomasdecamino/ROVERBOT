// PID.h
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
// @section DESCRIPTION
// 
// A PID controller is a widely used feedback controller commonly found in
// industry.
//
// This library is a port of Brett Beauregard's Arduino PID library:
//
//  http://www.arduino.cc/playground/Code/PIDLibrary
//
// The wikipedia article on PID controllers is a good place to start on
// understanding how they work:
//
//  http://en.wikipedia.org/wiki/PID_controller
//


#ifndef PID_H
#define PID_H

#include "mbed.h"

#define MANUAL_MODE 0
#define AUTO_MODE   1

class PID {

public:

    PID(float Kc, float tauI, float tauD, float interval);

    void setInputLimits(float inMin , float inMax);
    void setOutputLimits(float outMin, float outMax);
    void setTunings(float Kc, float tauI, float tauD);
    void reset(void);
    void setMode(int mode);
    void setInterval(float interval);
    void setSetPoint(float sp);
    void setProcessValue(float pv);
    void setBias(float bias);
    float compute(void);

    //Getters.
    float getSetPint(void);
    float getInMin(void);
    float getInMax(void);
    float getOutMin(void);
    float getOutMax(void);
    float getInterval(void);
    float getP(void);
    float getI(void);
    float getD(void);

private:
    bool usingFeedForward;
    bool inAuto;

    float Kc;
    float tauR;
    float tauD;

    float pParam;
    float iParam;
    float dParam;
    float setPoint;         

    float processVariable;  
    float prevProcessVariable;

    float controllerOutput; 
    float prevControllerOutput;

    float inMin;
    float inMax;
    float inSpan;
    float outMin;
    float outMax;
    float outSpan;

    //Error.
    float accError;
    //BIAS.
    float bias;
    //Interval.
    float tSample;          
    //Output.
    volatile float realOutput;

};

#endif /* PID_H */

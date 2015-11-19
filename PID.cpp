// PID.cpp
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

#include "PID.h"

PID::PID(float Kc, float tauI, float tauD, float interval) {

    this->usingFeedForward = false;
    this->inAuto           = false;

    this->setInputLimits(0, 3.3);
    this->setOutputLimits(0, 1.0);

    this->tSample = interval;

    this->setTunings(Kc, tauI, tauD);

    this->setPoint             = 0.0;
    this->processVariable      = 0.0;
    this->prevProcessVariable  = 0.0;
    this->controllerOutput     = 0.0;
    this->prevControllerOutput = 0.0;

    this->accError = 0.0;
    this->bias     = 0.0;
    
    this->realOutput = 0.0;

}

void PID::setInputLimits(float inMin, float inMax) {

    //Make sure we haven't been given impossible values.
    if (inMin >= inMax) {
        return;
    }

    //Rescale the working variables to reflect the changes.
    this->prevProcessVariable *= (inMax - inMin) / this->inSpan;
    this->accError            *= (inMax - inMin) / this->inSpan;

    //Make sure the working variables are within the new limits.
    if (this->prevProcessVariable > 1) {
        this->prevProcessVariable = 1;
    } else if (this->prevProcessVariable < 0) {
        this->prevProcessVariable = 0;
    }

    this->inMin  = inMin;
    this->inMax  = inMax;
    this->inSpan = inMax - inMin;

}

void PID::setOutputLimits(float outMin, float outMax) {
    //Make sure we haven't been given impossible values.
    if (outMin >= outMax) {
        return;
    }

    //Rescale the working variables to reflect the changes.
    this->prevControllerOutput *= (outMax - outMin) / this->outSpan;
	//Make sure the working variables are within the new limits.
    if (this->prevControllerOutput > 1) {
        this->prevControllerOutput = 1;
    } else if (this->prevControllerOutput < 0) {
        this->prevControllerOutput = 0;
    }
    this->outMin  = outMin;
    this->outMax  = outMax;
    this->outSpan = outMax - outMin;

}

void PID::setTunings(float Kc, float tauI, float tauD) {

    //Verify that the tunings make sense.
    if (Kc == 0.0 || tauI < 0.0 || tauD < 0.0) {
        return;
    }

    //Store raw values to hand back to user on request.
    this->pParam = Kc;
    this->iParam = tauI;
    this->dParam = tauD;

    float tempTauR;

    if (tauI == 0.0) {
        tempTauR = 0.0;
    } else {
        tempTauR = (1.0 / tauI) * this->tSample;
    }

    //For "bumpless transfer" we need to rescale the accumulated error.
    if (inAuto) {
        if (tempTauR == 0.0) {
            this->accError = 0.0;
        } else {
            this->accError *= (this->Kc * this->tauR) / (Kc * tempTauR);
        }
    }

    this->Kc   = Kc;
    this->tauR = tempTauR;
    this->tauD = tauD / this->tSample;

}

void PID::reset(void) {
    float scaledBias = 0.0;
    if (this->usingFeedForward) {
        scaledBias = (this->bias - this->outMin) / this->outSpan;
    } else {
        scaledBias = (this->realOutput - this->outMin) / this->outSpan;
    }

    this->prevControllerOutput = scaledBias;
    this->prevProcessVariable  = (this->processVariable - this->inMin) / this->inSpan;

    //Clear any error in the integral.
    this->accError = 0;
}

void PID::setMode(int mode) {

    //We were in manual, and we just got set to auto.
    //Reset the controller internals.
    if (mode != 0 && !this->inAuto) {
        this->reset();
    }

    this->inAuto = (mode != 0);

}

void PID::setInterval(float interval) {
    if (interval > 0) {
        //Convert the time-based tunings to reflect this change.
        this->tauR     *= (interval / this->tSample);
        this->accError *= (this->tSample / interval);
        this->tauD     *= (interval / this->tSample);
        this->tSample   = interval;
    }
}

void PID::setSetPoint(float sp) {
    this->setPoint = sp;
}

void PID::setProcessValue(float pv) {
    this->processVariable = pv;
}

void PID::setBias(float bias){
    this->bias = bias;
    this->usingFeedForward = 1;
}

float PID::compute() {
    //Pull in the input and setpoint, and scale them into percent span.
    float scaledPV = (this->processVariable - this->inMin) / this->inSpan;

    if (scaledPV > 1.0) {
        scaledPV = 1.0;
    } else if (scaledPV < 0.0) {
        scaledPV = 0.0;
    }

    float scaledSP = (this->setPoint - this->inMin) / this->inSpan;
    if (scaledSP > 1.0) {
        scaledSP = 1;
    } else if (scaledSP < 0.0) {
        scaledSP = 0;
    }

    float error = scaledSP - scaledPV;

    //Check and see if the output is pegged at a limit and only
    //integrate if it is not. This is to prevent reset-windup.
    if (!(this->prevControllerOutput >= 1 && error > 0) && !(this->prevControllerOutput <= 0 && error < 0)) {
        this->accError += error;
    }

    //Compute the current slope of the input signal.
    float dMeas = (scaledPV - this->prevProcessVariable) / this->tSample;

    float scaledBias = 0.0;

    if (this->usingFeedForward) {
        scaledBias = (this->bias - this->outMin) / this->outSpan;
    }

    //Perform the PID calculation.
    this->controllerOutput = scaledBias + this->Kc * (error + (this->tauR * this->accError) - (this->tauD * dMeas));

    //Make sure the computed output is within output constraints.
    if (this->controllerOutput < 0.0) {
        this->controllerOutput = 0.0;
    } else if (this->controllerOutput > 1.0) {
        this->controllerOutput = 1.0;
    }

    //Remember this output for the windup check next time.
    this->prevControllerOutput = this->controllerOutput;
    //Remember the input for the derivative calculation next time.
    this->prevProcessVariable  = scaledPV;

    //Scale the output from percent span back out to a real world number.
    return ((this->controllerOutput * this->outSpan) + this->outMin);

}

float PID::getInMin() { return this->inMin; }

float PID::getInMax() { return this->inMax; }

float PID::getOutMin() { return this->outMin;}

float PID::getOutMax() { return this->outMax; }

float PID::getInterval() { return this->tSample; }

float PID::getP() { return this->pParam; }

float PID::getI() { return this->iParam; }

float PID::getD() { return this->dParam; }

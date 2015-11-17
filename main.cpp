// main.h
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
    #include "mbed.h"
    #include "Carro.h"
    #include "MPU6050.h"
    
    Carro carro(
			PC_2,PC_3,  //decoder izquierda
			PC_1,PC_0, //decoder derecha
			PB_13,PB_5,  //motor izquierda
			PA_10,PC_4   //motor derecha
			);
			
    float sum = 0;
    uint32_t sumCount = 0;

    MPU6050 mpu6050;

    Timer t;

    Serial pc(USBTX, USBRX); // tx, rx

    float anguloY;
    float anguloX;

int main(){
    pc.baud(9600);  
    carro.init();
    i2c.frequency(400000);
    t.start();
    wait(2);
    carro.set(2,2);
    while(1){
		pc.printf(" vels  %i , %i\n\r",carro.get(0),carro.get(1));
		wait(0.25);
    }

    uint8_t whoami = mpu6050.readByte(MPU6050_ADDRESS, WHO_AM_I_MPU6050);
    pc.printf("I AM 0x%x\n\r", whoami);
    pc.printf("I SHOULD BE 0x68\n\r");
    if (whoami == 0x68) {  
        pc.printf("MPU6050 is online...");
        wait(1);
        mpu6050.MPU6050SelfTest(SelfTest); // Start by performing self test and reporting values
        wait(1);
        if(SelfTest[0] < 1.0f && SelfTest[1] < 1.0f && SelfTest[2] < 1.0f && SelfTest[3] < 1.0f && SelfTest[4] < 1.0f && SelfTest[5] < 1.0f) {
            mpu6050.resetMPU6050();
            mpu6050.calibrateMPU6050(gyroBias, accelBias);
            mpu6050.initMPU6050();
            pc.printf("MPU6050 initialized for active data mode....\n\r");
            wait(2);
        }else{
            pc.printf("Device did not the pass self-test!\n\r");
        }
    }else{
        pc.printf("Could not connect to MPU6050: \n\r");
        pc.printf("%#x \n",  whoami);
        while(1) ;
    }

    while(1) {
        if(mpu6050.readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) {  // check if data ready interrupt
            mpu6050.readAccelData(accelCount);  // Read the x/y/z adc values
            mpu6050.getAres();
            ax = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
            ay = (float)accelCount[1]*aRes - accelBias[1];   
            az = (float)accelCount[2]*aRes - accelBias[2];  
            mpu6050.readGyroData(gyroCount);
            mpu6050.getGres();
            gx = (float)gyroCount[0]*gRes; // - gyroBias[0];  // get actual gyro value, this depends on scale being set
            gy = (float)gyroCount[1]*gRes;  
            gz = (float)gyroCount[2]*gRes;   
            tempCount = mpu6050.readTempData();  // Read the x/y/z adc values
            temperature = (tempCount) / 340. + 36.53; // Temperature in degrees Centigrade
        }  
        Now = t.read_us();
        deltat = (float)((Now - lastUpdate)/1000000.0f) ; // set integration time by time elapsed since last filter update
        lastUpdate = Now;
        sum += deltat;
        sumCount++;
        if(lastUpdate - firstUpdate > 10000000.0f) {
            beta = 0.04;  // decrease filter gain after stabilized
            zeta = 0.015; // increasey bias drift gain after stabilized
        }
        // Pass gyro rate as rad/s
        mpu6050.MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f);
        delt_t = t.read_ms() - count;
        if (delt_t > 500) { // update LCD once per half-second independent of read rate
            anguloX = atan(ax/sqrt(pow(ay,2)+pow(az,2)))*100;
            anguloY = atan(ay/sqrt(pow(ax,2)+pow(az,2)))*100;
            pc.printf("angulo x:%f angulo y:%f\n\r",anguloX,anguloY);
            myled= !myled;
            count = t.read_ms(); 
            sum = 0;
            sumCount = 0; 
        }
    } 
}

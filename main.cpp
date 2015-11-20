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
    #include "MMA7660.h"
    
    Carro carro(
			PC_2,PC_3,  //decoder izquierda
			PC_1,PC_0, //decoder derecha
			PB_13,PB_5,  //motor izquierda
			PA_10,PC_4   //motor derecha
			);

    Serial pc(USBTX, USBRX); // tx, rx
	Serial otro(PA_11,PA_12);
	MMA7660 MMA(I2C_SDA, I2C_SCL);
	DigitalIn espReady(PA_6);
int main(){
    pc.baud(9600);  
    otro.baud(115200);
    carro.init();
    wait(2);
    MMA.setSampleRate(120);
    carro.set(0,0);
    int vi=0;
    int vd=0;
    while(!espReady);
    while(1){
		otro.printf("%f,%f",MMA.x(),MMA.y());
		if(otro.readable()){
			otro.getc();
			otro.scanf(" %i,%i",&vi,&vd);
			carro.set(vi,vd);
		}
		wait(0.5);
    }

}

# ROVERBOT
Proyecto de robot usando: STM32F411, plataforma ROVER5, MBED, MPU6050

Para este proyecto se emplea una targeta STM32F411RET6(Nucleo F411), junto con MBED para una facil programacion ademad de algunos sensores,motores, puentes H, todo montado sobre una plataforma ROVER 5. 
# Requisitos
	 gcc-arm-embedded: https://launchpad.net/gcc-arm-embedded
	 make: apt-get install make o yum install make
	 st-link: https://github.com/texane/stlink

#Compilacion
	make

#Subir a la targeta
	st-flash --reset write Carro.bin 0x8000000


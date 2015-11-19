# ROVERBOT
Proyecto de robot usando: STM32F411, plataforma ROVER5, MBED, MPU6050

Para este proyecto se emplea una targeta STM32F411RET6(Nucleo F411), junto con MBED para una facil programacion ademad de algunos sensores,motores, puentes H, todo montado sobre una plataforma ROVER 5. 
# Requisitos
	 gcc-arm-embedded: https://launchpad.net/gcc-arm-embedded
	 make: apt-get install make o yum install make
	 st-link: https://github.com/texane/stlink

#Instalacion
En el archivo Makefile editar las lineas ST_LINK, GCC_BIN y colocar la direccion donde se encuentren los binarios correspondientes.
	ST_LINK hace referencia a donde esta instalado st-link(requisito)
	GCC_BIN hace refetencia a donde esta la carpeta bin del compilador gcc-arm-embedded	
En caso de que esten en agregados a la variable de sistema PATH, con el comando whereis o which desde la consola pueden optenerlos facilmente.
	whereis st-link
	which arm-none-eabi-gcc

#Compilacion
	make

#Subir a la targeta
	make upload

#Limpiar build
	make clean

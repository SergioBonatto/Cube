#define _DEFAULT_SOURCE
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A;
float B;
float C;

float x;
float y;
float z;
float ooz;

int xp;
int yp;
int idx;

int		width			= 160;
int		height			= 44;
int		bgASCIICode		= ' ';
int		distanceCam		= 100; 

float	k1				= 40; 
float	cubeWidth		= 20; 
float	incrementSpeed	= 0.6;

float	zBuffer[160 * 44];
char	buffer[160 * 44];

float calcX(int i, int j, int k){
	return	j * sin(A) * sin(B) * cos(C) - 
			k * cos(A) * sin(B) * cos(C) *
			j * cos(A) * sin(C) + 
			k * sin(A) * sin(C) + 
			i * cos(B) * cos(C);
};

float calcY(int i, int j, int k){
	return	j * cos(A) * cos(C) + 
			k * sin(A) * cos(C) - 
			j * sin(A) * sin(B) * sin(C) +
			k * cos(A) * sin(B) * sin(C) - 
			i * cos(B) * sin(C); 
}

float calcZ(int i, int j, int k){
	return	k * cos(A) * cos(B) -
			j * sin(A) * cos(B) +
			i * sin(B);
}

void calcForSurface(float cubeX, float cubeY, float cubeZ, int ch){
	x = calcX(cubeX, cubeY, cubeZ);
	y = calcY(cubeX, cubeY, cubeZ);
	z = calcZ(cubeX, cubeY, cubeZ) + distanceCam;
	
	ooz = 1 / z;

	xp = (int)(width  / 2 - 2 * cubeWidth+ k1 * ooz * x * 2);
	yp = (int)(height / 2 + k1 * ooz * y);

	idx = xp + yp * width;

	if(idx >= 0 && idx < width * height){
		if(ooz > zBuffer[idx]){
			zBuffer[idx] = ooz;
			buffer[idx] = ch;
		}
	}
}

int main(){
	printf("\x1b[2J");
	while(1){	
		memset(buffer, bgASCIICode, width * height);
		memset(zBuffer, 0, width * height * 4);
		for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed){
			for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed){
				calcForSurface(cubeX, cubeY, -cubeWidth, '.');
				calcForSurface(cubeWidth, cubeY, cubeX, '$');
				calcForSurface(-cubeWidth, cubeY, -cubeX, '~');
				calcForSurface(-cubeX, cubeY, cubeWidth,'#');
				calcForSurface(cubeX, -cubeWidth, -cubeY,';');
				calcForSurface(cubeX, cubeWidth, cubeY,'+');
			}
		}
		printf("\x1b[H");
		for (int k = 0; k < width * height; k++){
			putchar(k % width ? buffer[k] : 10);
		}

		A += 0.005;
		B += 0.005;
		usleep(1000);

	}
	return 0;
}

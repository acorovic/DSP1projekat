//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina: 2015
// *
// * Zadatak: Prijemnik DTMF signala
// * Autor:   Aleksa Corovic
// *                                                                          
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "print_number.h"
#include "math.h"
#include "Dsplib.h"
#include "window.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Velicina prozora za racunanje FFT-a */
#define FFT_SIZE 256

/* Prag */
#define TRESHOLD 1120138L

/* Trajanje (8000 odbiraka u 1s, 1ms = 8 odbiraka, 128ms = 1024 odbirka, ucitava se po 128 odbiraka, pa je potrebno 8 blokova ucitati*/
#define DELTA 8

/* Niz za smestanje odbiraka ulaznog signala */
#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];
Int16 oldValues[AUDIO_IO_SIZE];
Int16 newValues[AUDIO_IO_SIZE];
Int16 fftBuffer[FFT_SIZE];
Int16 importantComponents[8] = {33, 35, 38, 40, 47, 52, 57, 62};

Int32 componentPower[8];
char currentChar = 'E';
char previousChar = 'E';
int charLength = 0;
int printEnable = 1;

void main( void )
{   
	int i;

	for(i = 0; i < AUDIO_IO_SIZE; i++)
		oldValues[i] = 0;
	for(i = 0; i < 8; i++)
		componentPower[i] = 0;

	/* Inicijalizaija razvojne ploce */
	EZDSP5535_init( );

    /* Inicijalizacija LCD kontrolera */
	initPrintNumber();

	printf("\n DTMF Prijemnik\n");

    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();

    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    while(1)
	{
		aic3204_read_block(InputBufferL, InputBufferR);
		/* Preuzimanje odbiraka */
		for(i = 0; i < AUDIO_IO_SIZE; i++) {
			newValues[i] = InputBufferL[i];
		}
		/* Popunjavanje bafera za fft */
		int k = AUDIO_IO_SIZE;
		for(i = 0; i < AUDIO_IO_SIZE; i++, k++) {
			fftBuffer[i] = oldValues[i];
			fftBuffer[k] = newValues[i];
		}
		/* Prozoriranje */
		for(i = 0; i < FFT_SIZE; i++) {
			fftBuffer[i] = _smpy(fftBuffer[i], p_window[i]);
		}
		/* FFT */
		rfft(fftBuffer, FFT_SIZE, SCALE);
		/* Racunanje snaga */
		for(i = 0; i < 8; i++)
			componentPower[i] = (Int32)fftBuffer[2*importantComponents[i]]*fftBuffer[2*importantComponents[i]] + (Int32)fftBuffer[2*importantComponents[i] + 1]*fftBuffer[2*importantComponents[i] + 1]
								+ (Int32)fftBuffer[2*importantComponents[i]+2]*fftBuffer[2*importantComponents[i]+2] + (Int32)fftBuffer[2*importantComponents[i] + 3]*fftBuffer[2*importantComponents[i] + 3];
		/* Racunanje najjace i druge najjace snage radi provere sa pragom */
		Int32 biggestLow = componentPower[0];
		Int16 biggestLowIndex = 0;
		Int32 biggestHigh = componentPower[4];
		Int16 biggestHighIndex = 0;

		for(i = 1; i < 4; i++) {
			if(componentPower[i] > biggestLow) {
				biggestLow = componentPower[i];
				biggestLowIndex = i;
			}
		}

		for(i = 4; i < 8; i++) {
			if(componentPower[i] > biggestHigh) {
				biggestHigh = componentPower[i];
				biggestHighIndex = i - 4;
			}
		}

		Int32 secondBiggestLow;
		Int16 secondBiggestLowIndex;
		Int32 secondBiggestHigh;
		Int16 secondBiggestHighIndex;

		if(biggestLowIndex == 0) {
			secondBiggestLowIndex = 1;
			secondBiggestLow = componentPower[1];
			for(i = 2; i < 4; i++)
				if(componentPower[i] > secondBiggestLow) {
					secondBiggestLow = componentPower[i];
					secondBiggestLowIndex = i;
				}
		} else {
			secondBiggestLowIndex = 0;
			secondBiggestLow = componentPower[0];
			for(i = 1; i < 4; i++)
				if((componentPower[i] > secondBiggestLow) && (componentPower[i] != biggestLow)) {
					secondBiggestLowIndex = i;
					secondBiggestLow = componentPower[i];
				}
		}

		if(biggestHighIndex == 0) {
			secondBiggestHighIndex = 1;
			secondBiggestHigh = componentPower[5];
			for(i = 6; i < 8; i++)
				if(componentPower[i] > secondBiggestHigh) {
					secondBiggestHigh = componentPower[i];
					secondBiggestHighIndex = i - 4;
				}
		} else {
			secondBiggestHighIndex = 0;
			secondBiggestHigh = componentPower[4];
			for(i = 5; i < 8; i++)
				if((componentPower[i] > secondBiggestHigh) && (componentPower[i] != biggestHigh)) {
					secondBiggestHighIndex = i - 4;
					secondBiggestHigh = componentPower[i];
				}
		}

		Int32 razlikaDole = biggestLow - secondBiggestLow;
		Int32 razlikaGore = biggestHigh - secondBiggestHigh;
		/* Provera karaktera */
		if(TRESHOLD < razlikaDole && TRESHOLD < razlikaGore) {
			if(biggestHighIndex == 0 && biggestLowIndex == 0) {
				currentChar = '1';
			}
			if(biggestHighIndex == 1 && biggestLowIndex == 0) {
				currentChar = '2';
			}
			if(biggestHighIndex == 2 && biggestLowIndex == 0) {
				currentChar = '3';
			}
			if(biggestHighIndex == 3 && biggestLowIndex == 0) {
				currentChar = 'A';
			}
			if(biggestHighIndex == 0 && biggestLowIndex == 1) {
				currentChar = '4';
			}
			if(biggestHighIndex == 1 && biggestLowIndex == 1) {
				currentChar = '5';
			}
			if(biggestHighIndex == 2 && biggestLowIndex == 1) {
				currentChar = '6';
			}
			if(biggestHighIndex == 3 && biggestLowIndex == 1) {
				currentChar = 'B';
			}
			if(biggestHighIndex == 0 && biggestLowIndex == 2) {
				currentChar = '7';
			}
			if(biggestHighIndex == 1 && biggestLowIndex == 2) {
				currentChar = '8';
			}
			if(biggestHighIndex == 2 && biggestLowIndex == 2) {
				currentChar = '9';
			}
			if(biggestHighIndex == 3 && biggestLowIndex == 2) {
				currentChar = 'C';
			}
			if(biggestHighIndex == 0 && biggestLowIndex == 3) {
				currentChar = '*';
			}
			if(biggestHighIndex == 1 && biggestLowIndex == 3) {
				currentChar = '0';
			}
			if(biggestHighIndex == 2 && biggestLowIndex == 3) {
				currentChar = '#';
			}
			if(biggestHighIndex == 3 && biggestLowIndex == 3) {
				currentChar = 'D';
			}
			/* Dozvola za ispis ukuliko je dovoljno trajao signal na prijemu */
			if(currentChar != previousChar) {
				charLength = 0;
				printEnable = 1;
			}
			else {
				if((++charLength == DELTA) && (printEnable == 1)) {
					printChar(currentChar);
					printEnable = 0;
				}
			}

			for(i = 0; i < AUDIO_IO_SIZE; i++) {
				oldValues[i] = newValues[i];
			}

			//previousChar = currentChar;
		} else
			currentChar = 0;
		previousChar = currentChar;
	}
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}

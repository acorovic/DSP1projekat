//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 1
// * Godina:  2016
// *
// * Zadatak: Generator DTMF signala
// * Autor:   Aleksa Corovic RA 54/2014
// *                                                                          
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "gen_sinus.h"
#include "math.h"
#include "Dsplib.h"
#include "Dsplib_c.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L
/* Bazna frekvencija za generisanje */
#define FP 1040

#define NOISE_SCALE 0.2
#define AMPLITUDE_SCALE 0.8

#define PI 3.14159265

/* Niz za smestanje izlaznih odbiraka */
#pragma DATA_ALIGN(OutputBuffer,4)
Int16 OutputBuffer[AUDIO_IO_SIZE];
Int16 sineF1[AUDIO_IO_SIZE];
Int16 sineF2[AUDIO_IO_SIZE];
Int16 sineSum[AUDIO_IO_SIZE];
Int16 noise[AUDIO_IO_SIZE];
Int16 f1;
Int16 f2;
char buttons[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

float snr(Int16* signal, Int16* noise) {
	int i;
	Uint32 Ps = 0;
	Uint32 Pe = 0;
	for(i = 0; i < AUDIO_IO_SIZE; i++) {
		Ps += _smpy(signal[i], signal[i]);
		Pe += _smpy(noise[i], noise[i]);
	}

	Ps = Ps >> 7;
	Pe = Pe >> 7;

	return 10*log10(Ps/Pe);
}

void main( void )
{   
	int i;
	int j;
	int button = 0;
	/* Promenljive za pamcenje faznog pomeraja */
	int ph1 = 0;
	int ph2 = 0;

    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n DTMF Predajnik\n");
		
    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();
    
    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

	while(1)
	{
		/* Popunjavanje DMA bafera sa nulama (Ping i Pong) kako bi na izlazu bila emitovana tisina dok nije pritisnut taster SW2 */
		for(j=0; j < AUDIO_IO_SIZE; j++) {
			OutputBuffer[j] = 0;
		}

		for(i = 0; i < 2; i++) {
			aic3204_write_block(OutputBuffer, OutputBuffer);
		}

		int pressed;
		int pressedBefore = NoKey;
		/* Ukoliko nije pritisnut SW2, a pritisnut je SW1 menjanje karaktera */
		while((pressed = EZDSP5535_SAR_getKey()) != SW2) {
			if(pressed == SW1 && pressed != pressedBefore) {
				clearLCD();
				setWritePointerToFirstChar();
				printChar(buttons[button]);
				button++;
				button = button%16;
			}
			pressedBefore = pressed;
		}
		/* Generisanje DTMF dok je pritisnut SW2 */
		while((pressed = EZDSP5535_SAR_getKey()) == SW2) {
			switch(buttons[button-1]) {
			case '1':
				f1 = FP + 440;
				f2 = FP;
				break;
			case '2':
				f1 = FP + 600;
				f2 = FP;
				break;
			case '3':
				f1 = FP + 760;
				f2 = FP;
				break;
			case 'A':
				f1 = FP + 920;
				f2 = FP;
				break;
			case '4':
				f1 = FP + 440;
				f2 = FP + 80;
				break;
			case '5':
				f1 = FP + 600;
				f2 = FP + 80;
				break;
			case '6':
				f1 = FP + 760;
				f2 = FP + 80;
				break;
			case 'B':
				f1 = FP + 920;
				f2 = FP + 80;
				break;
			case '7':
				f1 = FP + 440;
				f2 = FP + 160;
				break;
			case '8':
				f1 = FP + 600;
				f2 = FP + 160;
				break;
			case '9':
				f1 = FP + 760;
				f2 = FP + 160;
				break;
			case 'C':
				f1 = FP + 920;
				f2 = FP + 160;
				break;
			case '*':
				f1 = FP + 440;
				f2 = FP + 240;
				break;
			case '0':
				f1 = FP + 600;
				f2 = FP + 240;
				break;
			case '#':
				f1 = FP + 760;
				f2 = FP + 240;
				break;
			case 'D':
				f1 = FP + 920;
				f2 = FP + 240;
				break;
			}
			/* Generisanje DTMF tonova u zavisnosti od karaktera */
			gen_sinus_table(AUDIO_IO_SIZE, AMPLITUDE_SCALE, (float)f1/SAMPLE_RATE, &ph1, sineF1);
			gen_sinus_table(AUDIO_IO_SIZE, AMPLITUDE_SCALE, (float)f2/SAMPLE_RATE, &ph2, sineF2);
			/* Generisanje suma */
			rand16init();
			rand16(noise, AUDIO_IO_SIZE);

			for(i = 0; i < AUDIO_IO_SIZE; i++) {
				noise[i] = noise[i] * NOISE_SCALE;
			}
			/* Sabiranje oba tona */
			for(i = 0; i < AUDIO_IO_SIZE; i++) {
				OutputBuffer[i] = ((Int32)sineF1[i] + sineF2[i]) >> 1;
			}

			float SNR;
			SNR = snr(OutputBuffer, noise);
			/* Dodavanje suma na izlaz */
			/*for(i = 0; i < AUDIO_IO_SIZE; i++) {
				OutputBuffer[i] += noise[i];
			}*/

			aic3204_write_block(OutputBuffer, OutputBuffer);
		}
	}
    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}



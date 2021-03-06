#include "tistdtypes.h"
#include "gen_sinus.h"
#include "sine_table.h"
#include "math.h"

#define PI 3.14159265

/********************************************************************
 * Generisanje sinusnog signala upotrebom tabele pretrazivanja
 *   n - broj odbiraka
 *   a - amplituda (u opsegu 0 - 1.0)
 *   f - frekvencija (normalizovana)
 *   ph - fazni pomeraj
 *   buffer - niz u kome ce biti smesten izlazni signal
 *********************************************************************/

void gen_sinus_table(Int16 n, float a, float f, int* ph, Int16 buffer[])
{
	int i = 0;
	int delta = f * 4* SINE_TABLE_SIZE;
	int k = *ph;
	//int k = (ph/(2*PI)*4* SINE_TABLE_SIZE);
	int mask = (4*SINE_TABLE_SIZE-1);
	for (i = 0; i < n; i++)
	{
		k = k & mask;
		if(k < SINE_TABLE_SIZE) {
			buffer[i] = a*p_sine_table[k];
		}
		else if((k >= SINE_TABLE_SIZE) && (k < SINE_TABLE_SIZE*2)) {
			buffer[i] = a*p_sine_table[SINE_TABLE_SIZE*2-k];
		}
		else if((k >= 2*SINE_TABLE_SIZE) && (k < SINE_TABLE_SIZE*3)) {
			buffer[i] = -a*p_sine_table[k-2*SINE_TABLE_SIZE];
		} else
			buffer[i] = -a*p_sine_table[4*SINE_TABLE_SIZE - k - 1];
		k += delta;
	}
	*ph = k;
}

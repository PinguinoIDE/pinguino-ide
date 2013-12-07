/*	----------------------------------------------------------------------------
	SOUND LIBRAIRY TO PLAY SOUND WITH PIEZO BUZZER VIA PWM
	----------------------------------------------------------------------------
	author:	R. Blanchot rblanchot@gmail.com
	----------------------------------------------------------------------------
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	--------------------------------------------------------------------------*/

#ifndef __SOUND_C
#define __SOUND_C

// TODO : add flat (bemol, bemolle) notes

// English Notes (s for sharp)
#define C		1
#define Cs		2
#define D		3
#define Ds		4
#define E		5
#define F		6
#define Fs		7
#define G		8
#define Gs		9
#define A		10
#define As		11
#define B		12
#define C_		13

// Romance Notes (d for diese, diesis, sostenido, ...)
#define DO		1
#define DOd		2
#define RE		3
#define REd		4
#define MI		5
#define FA		6
#define FAd		7
#define SOL		8
#define SOLd	9
#define LA		10
#define LAd		11
#define SI		12
#define DO_		13

// REST
#define R4		8	// long (or four-measure rest)
#define R2		7	// double whole rest / breve rest
#define R1		6	// whole rest / semibreve rest
#define R1_2	5	// half rest / minim rest
#define R1_4	4	// quarter rest / crotchet rest
#define R1_8	3	// eighth rest / quaver rest
#define R1_16 	2	// sixteenth rest / semiquaver rest
#define R1_32	1	// thirty-second rest / demisemiquaver rest
#define R1_64	0	// sixty-fourth rest / hemidemisemiquaver rest

#include <delay.c>
#include <digitalw.c>
#include <pwm.c>

void noTone(u8 pin)
{
	PWM_set_frequency(0);					// note (range 1..14)
	PWM_set_percent_dutycycle(pin, 0);		// silence
	//pinmode(pin, INPUT);
	//CCP1CON = 0;							// stop sound = stop pwm
	//T2CONbits.TMR2ON = OFF;				// disable Timer2 = stop pwm = stop sound
}

void Tone(u8 pin, u16 freq, u16 duration)
{
	PWM_set_frequency(freq);
	PWM_set_percent_dutycycle(pin, 50);
	Delayms(duration);						// length of sound in ms 
	noTone(pin);
}

/*	----------------------------------------------------------------------------
Sound
----------------------------------------------------------------------------
PWM Frequencies are computed for 48MHz internal clock
@param pin:			pin number where buzzer is connected
@param note:		Note to Play {DO,RE,MI,FA,SOL,LA,SI,DO_}
@param duration:	Duration in ms
@param vol:			Volume from 0 to 10
----------------------------------------------------------------------------
The purpose of the duty cycle in our case is the volume.
You can vary the volume of the sound by modifying the duty cycle.
0% will produce no sounds, 50% will be the max volume.
Between 50% and 100% is the same as between 0% and 50%.
----------------------------------------------------------------------------
TODO:				Using timer to replace the Delayms function
					vol is no more used
--------------------------------------------------------------------------*/

void Sound(u8 pin, u8 note, u16 duration, u8 vol)
{
	// Frequency table in French : http://fr.wikipedia.org/wiki/Note_de_musique
	// and in English http://liutaiomottola.com/formulae/freqtab.htm
	// freq. must be > 3000Hz so notes are taken from the 8th octave

	//const u16 freq[] = {2093,2217,2349,2489,2637,2793,2959,3135,3322,3520,3729,3951,4186};
	//const u16 freq[] = {4186,4434,4698,4978,5274,5587,5919,6271,6644,7040,7458,7902,8372};
	const u16 freq[] = {4189,4437,4687,4966,5281,5597,5905,6250,6637,7075,7425,7894,8333};

	if (vol > 10) vol = 10;					// max. volume is 10
	vol = vol * 5;							// max. volume is reach for 50% duty

	Tone(pin, freq[note-1], duration);
}

/*	----------------------------------------------------------------------------
Play
----------------------------------------------------------------------------
PWM Frequencies are computed for 48MHz internal clock
@param pin:			pin number where buzzer is connected
@param partition:	Notes to Play {DO,RE,MI,FA,SOL,LA,SI,DO_}
@param vol:			Volume from 0 to 10
--------------------------------------------------------------------------*/

void Play(u8 pin, const u8 *partition, u8 volume)
{
	u8 i = 0;
	u8 exp;
	while (partition[i] != 0)
	{
		exp = partition[i+1];
		Sound(pin, partition[i], (8 << exp), volume);
		i = i + 2;
	}		
}

#endif	/* __SOUND_C */


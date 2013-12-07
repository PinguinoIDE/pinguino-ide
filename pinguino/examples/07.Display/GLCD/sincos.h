/*
 * By Marcus Fazzi (Anunakin@ieee.org) GPLv2
 * http://vivaphp.net
 * 
 * sin() and cos() for use with analog clock system, in name of memory! :)
 * 
 */
 
//Integer sins => (sin(x*0.1045))
__code __at(0x7900) float sins[] = { 0,0.104528463,0.207911691,0.309016994,
			0.406736643,0.5,0.587785252,0.669130606,0.743144825,
			0.809016994,0.866025404,0.913545458,0.951056516,
			0.978147601,0.994521895,1 };

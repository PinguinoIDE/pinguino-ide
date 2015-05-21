/*
 * By Marcus Fazzi (Anunakin@ieee.org) GPLv2 2011
 * http://vivaphp.net
 * 
 * In name of memory! I create a custom sine and cosine for use with analog clocks! :P 
 * This free about 6k of flash used by math.h! :)
 * 
 * sin() and cos() for use with analog clock system, in name of memory! :)
 * 
 */

#include "sincos.h"

//x are minutes or seconds
float zino(unsigned char x){
	if(x<16){
		return(sins[x]);
	}
	if(x<30){
		return(sins[30-x]);
	}
	if(x<46){
		return(-sins[x-30]);
	}
	//x<60
	return(-sins[60-x]);
}

float cozi(unsigned char x){
	if(x<16){
		return(sins[15-x]);
	}
	if(x<30){
		return(-sins[x-15]);
	}
	if(x<46){
		return(-sins[45-x]);
	}
	//x<60
	return(sins[x-45]);
}

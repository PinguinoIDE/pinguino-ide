/*
 * By Marcus Fazzi (Anunakin@ieee.org) GPLv2 2011
 * http://fazzi.eng.br
 * 
 * In name of memory! I create a custom sine and cosine for use with analog clocks! :P 
 * This free about 20k of flash used by math.h! :)
 * 
 * Integer sin() and cos(), in name of memory! :)
 * 
 */
#ifndef INTEGER_MATH_c
#define INTEGER_MATH_c

#include "integer_math.h"

//Integer SIN
returntype sini(unsigned int x){
    x = x - 360*((int)(x/360));
    
	if(x<=90){
		return(sins[x]);
	}
	if(x<180){
		return(sins[180-x]);
	}
	if(x<270){
		return(-sins[x-180]);
	}
	//x<360
	return(-sins[360-x]);
}

//Integer COS
returntype cosi(unsigned int x){
    x = x - 360*((int)(x/360));
    
	if(x<=90){
		return(sins[90-x]);
	}
	if(x<180){
		return(-sins[x-90]);
	}
	if(x<=270){
		return(-sins[270-x]);
	}
	//x<60
	return(sins[x-270]);
}

returntype tani(unsigned int x){
    return (sini(x)/cosi(x));
}

returntype powi(int base, int num){
    int i, to;
    float res = 1;
    
    to = num > 0? num: num * (-1);

    for(i = 0; i<to; i++){
        res *= base;
    }
    
    return (num>0?res:(1/res));
    
}

#endif

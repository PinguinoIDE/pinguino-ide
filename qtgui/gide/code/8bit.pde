//-----------------------------------------------------
//Author:  Yeison Cardona --<yeison.eng@gmail.com>
//First release: 05/03/2012
//Last release: 12/04/2013
//Description: Control de la tarjeta pinguino mediante
//comandos transparentes enviados por CDC.
//-----------------------------------------------------
  
#include <string.h>
#include <stdlib.h>
#define TotalPines 21
   
char lectura[21], *bloque;
int par1, valor, cont, i;

char *strtok(char *str, char *control) {
//Written By -  Sandeep Dutta . sandeep.dutta@usa.net (1999)
static  char * s; register char * s1; if (str)s = str; if (!s)return NULL;
while (*s) { if (strchr(control,*s))s++; else break;}
s1 = s ; while (*s) { if (strchr(control,*s)) {*s++ = '\0'; return s1 ;} s++ ;}
s = NULL;  if (*s1)return s1; else return NULL; }
   
void setup() {
 }
  
void leer_cadena() {
    unsigned char valor_leido[21];
    unsigned char receivedbyte;
    int cont=1;
    while (cont){    
        receivedbyte=CDC.read(valor_leido);
        valor_leido[receivedbyte]=0;
        if (receivedbyte>0) cont=0;}
    strcpy(lectura,valor_leido);}
    
void loop() {
   
leer_cadena();
cont=0;
   
if (strncmp(lectura, "pinMode", 7)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ",") ){ 
 switch (cont){
 case 0: break;
 case 1:
  par1=atoi(bloque);
  break;
 case 2:
  if (strncmp(bloque, "INPUT", 5)==0) pinMode(par1,INPUT);
  else if (strncmp(bloque, "OUTPUT", 6)==0) pinMode(par1,OUTPUT);
  break;
 } 
 cont+=1;
 }
}
   
else if (strncmp(lectura, "digitalWrite", 12)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ",") ){
 switch (cont){
 case 0: break;
 case 1:
  par1=atoi(bloque);
  break;
 case 2:
  if (strncmp(bloque, "HIGH", 4)==0) digitalWrite(par1,HIGH);
  else if (strncmp(bloque, "LOW", 3)==0) digitalWrite(par1,LOW);
  break;
 } 
 cont+=1;
 }
}
   
else if (strncmp(lectura, "analogWrite", 11)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ",") ){
 switch (cont){
 case 0: break;
 case 1:
  par1=atoi(bloque);
  break;
 case 2:
  analogWrite(par1,atoi(bloque));
  break;
 } 
 cont+=1;
 }
}
   
else if (strncmp(lectura, "digitalRead", 11)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ")") ){
 switch (cont){
 case 0: break;
 case 1:
  CDC.printf("%d\n",digitalRead(atoi(bloque)));
  break;
 } 
 cont+=1;
 }
}
   
else if (strncmp(lectura, "analogRead", 10)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ")") ){
 switch (cont){
 case 0: break;
 case 1:
  CDC.printf("%d\n",analogRead(atoi(bloque)));
  break;
 } 
 cont+=1;
 }
}
  
else if (strncmp(lectura, "eepromRead", 10)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ")") ){
 switch (cont){
 case 0: break;
 case 1:
  CDC.printf("%d\n",EEPROM.read(atoi(bloque)));
  break;
 } 
 cont+=1;
 }
}
  
else if (strncmp(lectura, "eepromWrite", 11)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ",") ){
 switch (cont){
 case 0: break;
 case 1:
  par1=atoi(bloque);
  break;
 case 2:
  bloque = strtok(bloque,")");
  EEPROM.write(par1, atoi(bloque));
  break;
 } 
 cont+=1;
 }
}
  
else if (strncmp(lectura, "delay", 5)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ")") ){
 switch (cont){
 case 0: break;
 case 1:
  delay(atoi(bloque));
  break;
 } 
 cont+=1;
 }
}
    
else if (strncmp(lectura, "delayMicroseconds", 17)==0){
 for (bloque = strtok(lectura,"("); bloque != NULL; bloque = strtok(NULL, ")") ){
 switch (cont){
 case 0: break;
 case 1:
  delayMicroseconds(atoi(bloque));
  CDC.printf("Done\n");
  break;
 } 
 cont+=1;
 }
}
    
else if (strcmp(lectura, "allOutput")==0){
 for (i=0;i<=TotalPines;i++){
  pinMode(i,OUTPUT);
  digitalWrite(i,LOW);
 }
}
  
else if (strcmp(lectura, "allInput")==0){
 for (i=0;i<=TotalPines;i++)
  pinMode(i,INPUT);
}
  
else if (strcmp(lectura, "allHigh")==0){
 for (i=0;i<=TotalPines;i++){
  pinMode(i,OUTPUT);
  digitalWrite(i,HIGH);
 }
}
  
else if (strcmp(lectura, "allLow")==0){
 for (i=0;i<=TotalPines;i++){
  pinMode(i,OUTPUT);
  digitalWrite(i,LOW);
 }
}
    
else if (strcmp(lectura, "reset")==0)
 reset();
    
}

/**
   * Fonts for display Text
   * If you get error: multiple sections using address 0x6500 
   * on load more then one truetype font,  try modify font.h file to load at another address, AT YOUR RISC!
   * "The whole range of flash memory 0 to 8000H . Be careful, bootloader is from 0 to 2000H and user program is from 2000H to ??
   *   So for security, i use flash starting at 6000H, with this configuration you can store 8192 bytes."
   *
   *   Fixed Fonts starts at 0x6000
   *   True Type Fonts at 0x6500
   *   Bitmaps at 0x7500
   *   Sine and Cosine table at 0x7900
   */

//
#define PIC18F4550

//
#define USEDATAD()
//
//

//
#define MODE 2
#define SET 3

//
#define SENSOR 13

//
#define RSEC 16
#define RMIN 12
#define RHOUR 8

//
#define I2C_address 0x68



/* 
 * Hera a custom sine and cosine for use with analog clocks! :P 
 * This free about 6k of flash used by math.h! :)
 */


//




//
//
unsigned char I2C_setParameters[8]={0x00, 0x00, 0x00,0x00,0x00,0x00,0x00,0x00};
//
unsigned char I2C_readRegister[2]={0x00, 0x00};

//
unsigned char I2C_OK;
unsigned char strtmp[3]={0x00, 0x00, 0x00};

//
unsigned char mode = 0;

unsigned char secAnt = 0;

//
unsigned char decToBcd(unsigned char val){
	return ( (val/10*16) + (val%10) );
}

//
unsigned char bcdToDec(unsigned char val){
	return ( (val/16*10) + (val%16) );
}

//
void printDec(unsigned char val){
	strtmp[0] = (val/10) + '0';
	strtmp[1] = (val%10) + '0';
}

void setup(){
	unsigned char i;
	
	Delayms(500);
	
//
//
	TRISBbits.TRISB0=1;
	TRISBbits.TRISB1=1;
	i2c_open(I2C_MASTER, I2C_SLEW_OFF, 19);

//
	pinmode(MODE, INPUT);
	pinmode(SET, INPUT);	
	
//
	pinmode(SENSOR, INPUT);

//
	for (i=0;i<8;i++)
		i2c_buffer[i]=0;
	
//
	Init(NON_INVERTED);   // initialise the library
	ClearScreenX();  // clear the screen
	DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	Delayms(3000);
	ClearScreenX();
}

void loop(){ // run over and over again	
	char x, y;	
	unsigned char hour = 0;
	unsigned char min  = 0;
	unsigned char sec  = 0;
	unsigned char sem  = 0;
	unsigned char dia  = 0;
	unsigned char mes  = 0;
	unsigned char ano  = 0;
	
//
	I2C_OK = I2C_write(I2C_address,I2C_readRegister,1);
	I2C_STOP();		
	if (I2C_OK == 1 ){
//
		I2C_OK=I2C_read(I2C_address,7);
		I2C_STOP();
			
		if (I2C_OK == 1 ){				
//
			sec  = bcdToDec(i2c_buffer[0] & 0x7f);
			min  = bcdToDec(i2c_buffer[1]);			
			hour = bcdToDec(i2c_buffer[2] & 0x3f);
			sem  = bcdToDec(i2c_buffer[3]);
			dia  = bcdToDec(i2c_buffer[4]);
			mes  = bcdToDec(i2c_buffer[5]); //Mes 0=> Janeiro
			ano  = bcdToDec(i2c_buffer[6]);
		}
	}
	
	if(sec!=secAnt){
		ClearScreenX();         // clear the screen
		secAnt = sec;		
	}
	
//
	if(!(digitalread(MODE))){
		if(++mode>7)
			mode = 0; //no setup/normal mode
			
		ClearScreenX();
	}
	
	if(!(digitalread(SET)) && mode != 0){
		ano = ano>99?0:ano;
		mes = mes>11?0:mes;
		sem = sem>6?0:sem;
		dia = dia>31?1:dia;
		hour= hour>23?0:hour;
		min = min>59?0:min;
		sec = sec>59?0:sec;
		
		I2C_setParameters[0] = decToBcd(0); //Start clock
		
		I2C_setParameters[1] = decToBcd((sec+(mode==1?1:0))); //Segundos
		I2C_setParameters[2] = decToBcd(min+(mode==2?1:0)); //Minutos
		I2C_setParameters[3] = decToBcd(hour+(mode==3?1:0));//Hora
		I2C_setParameters[4] = decToBcd(sem+(mode==4?1:0)); //Dia da Semana; 0 => domingo
		I2C_setParameters[5] = decToBcd(dia+(mode==5?1:0)); //Dia do Mes
		I2C_setParameters[6] = decToBcd(mes+(mode==6?1:0)); //Mes; 0 => janeiro
		I2C_setParameters[7] = decToBcd(ano+(mode==7?1:0)); //Ano
	
//
		I2C_OK = I2C_write(I2C_address,I2C_setParameters,9);
		I2C_STOP();
		
		ClearScreenX();
	}

	SelectFont(VerdanaNumbers, BLACK);
	GotoXY(5,0);	

	if (hour < 10)
		PutChar('0');
	PrintNumber(hour);
	
	PutChar(':');
	if (min < 10)
		PutChar('0');
	PrintNumber(min);
	
	PutChar(':');
	if (sec < 10)
		PutChar('0');
	PrintNumber(sec);
				
//
	DrawCircle(21,43,18,BLACK);
//
	x = RSEC * zino(sec);
	y = RSEC * cozi(sec);
	DrawLine(21,43,21+x,43-y, BLACK);
//
	x = RMIN * zino(min);
	y = RMIN * cozi(min);
	DrawLine(21,43,21+x,43-y, BLACK);
//
	y = min/15;
	x = RHOUR * zino((hour>11?hour-11:hour)*5+y);
	y = RHOUR * cozi((hour>11?hour-11:hour)*5+y);
	DrawLine(21,43,21+x,43-y, BLACK);
	
//
	if(mode==0){
//
//
		GotoXY(75,40);
//
		x = (analogRead(SENSOR)>>1)-1; //We need more memory!!!!
		PrintNumber(x);	
		Puts(";");
	
//
		DrawRoundRect(120, 39, 5, 22, 2, BLACK);
		x /= 3; //22*3 => 66 max temperature on graph
		FillRect(122, 61 - x, 1, x, BLACK); //61=39+22
	}
//
	GotoXY(38,25);
	SelectFont(System5x7, BLACK);
	
//
	switch(sem){
		case 1:
			Puts("DOM");
			break;
		case 2:
			Puts("SEG");
			break;
		case 3:
			Puts("TER");
			break;
		case 4:
			Puts("QUA");
			break;
		case 5:
			Puts("QUI");
			break;
		case 6:
			Puts("SEX");
			break;
		default:
			Puts("SAB");
	}
	Puts(" ");
	
	if(dia<10) Puts("0");
	PrintNumber(dia);
	Puts("/");
	if(mes<10) Puts("0");
	PrintNumber(mes);
	Puts("/20");
	if(ano<10) Puts("0");
	PrintNumber(ano); //Ano

//
	GotoXY(38,56);
	switch (mode){
			case 1:
				Puts("set Segundos");
				break;
			case 2:
				Puts("set Minutos");
				break;
			case 3:
				Puts("set Hora");
				break;
			case 4:
				Puts("set Dia Semana");
				break;
			case 5:
				Puts("set Dia");
				break;
			case 6:
				Puts("set Mes");
				break;
			case 7:
				Puts("set Ano");
				break;
	}

	Delayms(100); //just a fine tune
 }


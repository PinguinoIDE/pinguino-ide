/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53 & Pinguino 32MX250
        Output:	262K-color graphic TFT-LCD with ST7735 controller

        2 modes available :
        - Hardware SPI
            . default mode
            . SPI operations are handled by the CPU
            . pins have to be the CPU SPI pins
            . PINGUINO 32 have up to 4 SPI module (SPI1 to SPI4)
            . PINGUINO 8  have only one SPI module (SPI1)
        - Software SPI
            . SPISW
            . SPI operations are handled by the SPI library
            . pins can be any digital pin
        
        Wiring :
        
        ST7735    PINGUINO
        ---------------------------------------
        LED       VSS (backlight on)
        SCK       SCK
        SDA       SDO
        A0 (DC)   can be connected to any digital pin
        RESET     VSS
        CS        can be connected to any digital pin
        GND       GND
        VSS       VSS (+5V or +3.3V)
**/

// Load one or more fonts and active them with ST7735.setFont()
#include <fonts/font6x8.h>
#define SPIMODULE SPI2
#define NBPOINTS 8

typedef struct { float x, y, z, xy; } point3D;
typedef struct { float x, y;        } point2D;

point3D Sommet[NBPOINTS];   // les sommets du cube
point3D Point3D[NBPOINTS];  // les sommets apres rotation
point2D Point2D[NBPOINTS];  // les sommets apres projection

float matrice[3][3];          // 3*3 rotation matrix

u16 fps=0, maxfps=0;         // frame per second and stats
u16 xa=0, ya=0, za=0;        // angles de rotation
u16 Xoff, Yoff, Zoff;       // position de l'observateur

///
/// Effectue la rotation des points Sommet -> Point3D
///

void Rotation(u16 Xa, u16 Ya, u16 Za)
{
    u8 i;
    float a[3];

    // Calcul de la matrice de rotation 3*3

    matrice[0][0] = cosr(Za) * cosr(Ya);
    matrice[1][0] = sinr(Za) * cosr(Ya);
    matrice[2][0] = - sinr(Ya);

    matrice[0][1] = cosr(Za) * sinr(Ya) * sinr(Xa) - sinr(Za) * cosr(Xa);
    matrice[1][1] = sinr(Za) * sinr(Ya) * sinr(Xa) + cosr(Xa) * cosr(Za);
    matrice[2][1] = sinr(Xa) * cosr(Ya);

    matrice[0][2] = cosr(Za) * sinr(Ya) * cosr(Xa) + sinr(Za) * sinr(Xa);
    matrice[1][2] = sinr(Za) * sinr(Ya) * cosr(Xa) - cosr(Za) * sinr(Xa);
    matrice[2][2] = cosr(Xa) * cosr(Ya);

    a[0] = - (matrice[0][0] * matrice[0][1]);
    a[1] = - (matrice[1][0] * matrice[1][1]);
    a[2] = - (matrice[2][0] * matrice[2][1]);

    // Rotation des sommets de l'objet

    for (i = 0; i < NBPOINTS; i++)
    {
        Point3D[i].x =	((matrice[0][1] + Sommet[i].x) * (matrice[0][0] + Sommet[i].y)
            + a[0] + Sommet[i].xy
            + matrice[0][2] * Sommet[i].z);

        Point3D[i].y = ((matrice[1][1] + Sommet[i].x) * (matrice[1][0] + Sommet[i].y)
            + a[1] + Sommet[i].xy
            + matrice[1][2] * Sommet[i].z);

        Point3D[i].z =	((matrice[2][1] + Sommet[i].x) * (matrice[2][0] + Sommet[i].y)
            + a[2] + Sommet[i].xy
            + matrice[2][2] * Sommet[i].z);
    /*
        Point3D[i].x =	matrice[0][0]*Sommet[i].x + matrice[1][0]*Sommet[i].y + matrice[2][0]*Sommet[i].z;
        Point3D[i].y =	matrice[0][1]*Sommet[i].x + matrice[1][1]*Sommet[i].y + matrice[2][1]*Sommet[i].z;
        Point3D[i].z =	matrice[0][2]*Sommet[i].x + matrice[1][2]*Sommet[i].y + matrice[2][2]*Sommet[i].z;
     */
    }
}

///
/// Projette en perspective les points apres rotation.
/// 256 = distance (oeil - écran)

void Projection(void)
{
    u8 i;

    for (i = 0; i < NBPOINTS; i++)
    {
        Point2D[i].x = ( Point3D[i].x * 256.0f ) / ( Point3D[i].z + Zoff ) + Xoff;
        Point2D[i].y = ( Point3D[i].y * 256.0f ) / ( Point3D[i].z + Zoff ) + Yoff;
    }
}

///
/// Initialise les coordonnees des sommets du cube
///

void initCube()
{
    u8 i;

    Sommet[0].x = -100.0f;  Sommet[0].y = -100.0f;  Sommet[0].z = -100.0f;
    Sommet[1].x =  100.0f;  Sommet[1].y = -100.0f;  Sommet[1].z = -100.0f;
    Sommet[2].x =  100.0f;  Sommet[2].y =  100.0f;  Sommet[2].z = -100.0f;
    Sommet[3].x = -100.0f;  Sommet[3].y =  100.0f;  Sommet[3].z = -100.0f;
    
    Sommet[4].x =  100.0f;  Sommet[4].y = -100.0f;  Sommet[4].z =  100.0f;
    Sommet[5].x = -100.0f;  Sommet[5].y = -100.0f;  Sommet[5].z =  100.0f;
    Sommet[6].x = -100.0f;  Sommet[6].y =  100.0f;  Sommet[6].z =  100.0f;
    Sommet[7].x =  100.0f;  Sommet[7].y =  100.0f;  Sommet[7].z =  100.0f;

    for (i = 0; i < NBPOINTS; i++)
        Sommet[i].xy = - Sommet[i].x * Sommet[i].y;
}

///
/// Trace un segment
///

void ligne(u8 a, u8 b)
{
    ST7735.drawLine(SPIMODULE, Point2D[a].x, Point2D[a].y,
                               Point2D[b].x, Point2D[b].y);
}

///
/// Trace le cube
///

void drawCube()
{
    ligne(0,1); ligne(1,2); ligne(2,3); ligne(3,0);
    ligne(4,5); ligne(5,6); ligne(6,7); ligne(7,4);
    ligne(0,5); ligne(1,4); ligne(2,7); ligne(3,6);
}

///
/// Initialisation
///

void setup()
{
    // Optimization for Pinguino 32 only
    // 1/ Compile the program with MIPS32 option instead of MIPS16 (+30% faster)
    // 2/ Use of optimized Sine routine (cosr and sinr)
    // 3/ Overclock your processor if possible (Pinguino 32MX270 only)
    // 4/ Set Fspi to 30MHz :
    //    Fspi max = Fpb max / 2 with Fpb max = Fosc max
    //    Fspi max for ST7735 is 30 MHz so Fpb max = 60 MHz
    //    Adjust the following according to your processor
    //System.setCpuFrequency(60000000);
    //System.setPeripheralFrequency(60000000);
    ST7735.init(SPIMODULE, 6, 5, 0, 0);  	// CS, DC
    ST7735.setFont(SPIMODULE, font6x8);
    ST7735.setBackgroundColor(SPIMODULE, ST7735_BLUE);
    ST7735.setColor(SPIMODULE, ST7735_WHITE);
    ST7735.clearScreen(SPIMODULE);
    
    // Position du cube
    Xoff = ST7735[SPIMODULE].screen.width  / 2;
    Yoff = ST7735[SPIMODULE].screen.height / 2 + 9;
    Zoff = 1800;

    // create 3D cube
    initCube();
}

///
/// Boucle principale
///

void loop()
{
    u16 frame=0;
    u32 timeEnd = millis() + 1000; // 1000 ms = 1 sec
    
    while (millis() < timeEnd)
    {
        // calculations
        Rotation(xa, ya, za);
        Projection();
        
        // display
        ST7735.clearScreen(SPIMODULE);
        ST7735.printf(SPIMODULE, "%u fps (max. %u)", fps, maxfps);
        drawCube();

        // update angles
        xa = (xa + 1) % 360;
        ya = (ya + 3) % 360;
        za = (za + 1) % 360;
        
        // one frame done !
        frame++;
    }
    
    fps = frame;
    if (fps > maxfps) maxfps = fps;
}

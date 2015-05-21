/**
        Author: 	Régis Blanchot (Mar. 2014)
        Tested on:	Pinguino 47J53A & Pinguino 32MX250
        Output:	Oled 0.96" with SSD1306 Controller

        2 size available : SSD1306_128X64 or SSD1306_128X32
        
        Wiring :
        
        if SSD1306_6800
            if SSD1306_PMP
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D3)
                OLED D/C#   connected to Pinguino PMA1 (D4)
                OLED W/R#  connected to Pinguino PMRD/PMWR (D13)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino PMD[7:0] (D[31:24])
            else
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D0)
                OLED D/C#   connected to any GPIO (D1)
                OLED W/R#  connected to any GPIO (D2)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino D[31:24]
        if SSD1306_8080 
            if SSD1306_PMP
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D3)
                OLED D/C#   connected to Pinguino PMA1 (D4)
                OLED W/R#  connected to Pinguino PMWR (D14)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino PMD[7:0]
            else
                OLED CS#     connected to GND
                OLED RES#   connected to any GPIO (D0)
                OLED D/C#   connected to any GPIO (D1)
                OLED W/R#  connected to any GPIO (D2)
                OLED E/RD# connected to GND
                OLED D[7:0]  connected to Pinguino D[31:24]
        if SSD1306_I2C
        if SSD1306_SPI3
        if SSD1306_SPI4
**/

#define DISPLAY (SSD1306_PMP | SSD1306_6800 | SSD1306_128X64)
//#define DISPLAY (SSD1306_6800 | SSD1306_128X64)
//#define DISPLAY (SSD1306_8080 | SSD1306_128X64)
//#define DISPLAY (SSD1306_I2C  | SSD1306_128X64)
//#define DISPLAY (SSD1306_SPI3 | SSD1306_128X64)
//#define DISPLAY (SSD1306_SPI4 | SSD1306_128X64)

/**
    Load one or more fonts and active them with SSD1306.setFont()
**/

#include <fonts/font6x8.h>
//#include <fonts/font8x8.h>          // wrong direction
//#include <fonts/font10x14.h>        // ???
//#include <fonts/font12x8.h>         // wrong direction
//#include <fonts/font16x8.h>         // wrong direction
//#include <fonts/font16x16.h>        // ???

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

    /* Calcul de la matrice de rotation 3*3 */

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

    /* Rotation des sommets de l'objet */

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
    SSD1306_drawLine( Point2D[a].x, Point2D[a].y,
                      Point2D[b].x, Point2D[b].y	);
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
    SSD1306.init(3, 4);  	// init. LCD
    SSD1306.clearScreen();
    SSD1306.setFont(font6x8);
    
    // Position du cube
    Xoff = SSD1306.screen.width  / 2;
    Yoff = SSD1306.screen.height / 2 + 9;
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
        SSD1306.clearScreen();
        SSD1306.printf("%u fps (max. %u)", fps, maxfps);
        drawCube();
        SSD1306.refresh();

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

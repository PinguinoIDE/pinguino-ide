/*  --------------------------------------------------------------------
    FILE:           sound.h
    PROJECT:        pinguino
    PURPOSE:        note frequencies to play sound
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  15 Feb. 2015
    LAST RELEASE:   15 Feb. 2015
    --------------------------------------------------------------------
    CHANGELOG:
    --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

#ifndef __AUDIO_H
    #define __AUDIO_H
    
    // NOTES
    #define NOTE_B0  31
    #define NOTE_C1  33                 // 1rst octave
    #define NOTE_CS1 35
    #define NOTE_D1  37
    #define NOTE_DS1 39
    #define NOTE_E1  41
    #define NOTE_F1  44
    #define NOTE_FS1 46
    #define NOTE_G1  49
    #define NOTE_GS1 52
    #define NOTE_A1  55
    #define NOTE_AS1 58
    #define NOTE_B1  62
    #define NOTE_C2  65                 // 2nd octave
    #define NOTE_CS2 69
    #define NOTE_D2  73
    #define NOTE_DS2 78
    #define NOTE_E2  82
    #define NOTE_F2  87
    #define NOTE_FS2 93
    #define NOTE_G2  98
    #define NOTE_GS2 104
    #define NOTE_A2  110
    #define NOTE_AS2 117
    #define NOTE_B2  123
    #define NOTE_C3  131                 // 3rd octave
    #define NOTE_CS3 139
    #define NOTE_D3  147
    #define NOTE_DS3 156
    #define NOTE_E3  165
    #define NOTE_F3  175
    #define NOTE_FS3 185
    #define NOTE_G3  196
    #define NOTE_GS3 208
    #define NOTE_A3  220
    #define NOTE_AS3 233
    #define NOTE_B3  247
    #define NOTE_C4  262                 // 4th octave
    #define NOTE_CS4 277
    #define NOTE_D4  294
    #define NOTE_DS4 311
    #define NOTE_E4  330
    #define NOTE_F4  349
    #define NOTE_FS4 370
    #define NOTE_G4  392
    #define NOTE_GS4 415
    #define NOTE_A4  440
    #define NOTE_AS4 466
    #define NOTE_B4  494
    #define NOTE_C5  523                 // 5th octave
    #define NOTE_CS5 554
    #define NOTE_D5  587
    #define NOTE_DS5 622
    #define NOTE_E5  659
    #define NOTE_F5  698
    #define NOTE_FS5 740
    #define NOTE_G5  784
    #define NOTE_GS5 831
    #define NOTE_A5  880
    #define NOTE_AS5 932
    #define NOTE_B5  988
    #define NOTE_C6  1047                 // 6th octave
    #define NOTE_CS6 1109
    #define NOTE_D6  1175
    #define NOTE_DS6 1245
    #define NOTE_E6  1319
    #define NOTE_F6  1397
    #define NOTE_FS6 1480
    #define NOTE_G6  1568
    #define NOTE_GS6 1661
    #define NOTE_A6  1760
    #define NOTE_AS6 1865
    #define NOTE_B6  1976
    #define NOTE_C7  2093                 // 7th octave
    #define NOTE_CS7 2217
    #define NOTE_D7  2349
    #define NOTE_DS7 2489
    #define NOTE_E7  2637
    #define NOTE_F7  2794
    #define NOTE_FS7 2960
    #define NOTE_G7  3136
    #define NOTE_GS7 3322
    #define NOTE_A7  3520
    #define NOTE_AS7 3729
    #define NOTE_B7  3951
    #define NOTE_C8  4186                 // 8th octave
    #define NOTE_CS8 4435
    #define NOTE_D8  4699
    #define NOTE_DS8 4978

    // RESTS
    #define R4          8               // long (or four-measure rest)
    #define R2          7               // double whole rest / breve rest
    #define R1          6               // whole rest / semibreve rest
    #define R1_2        5               // half rest / minim rest
    #define R1_4        4               // quarter rest / crotchet rest
    #define R1_8        3               // eighth rest / quaver rest
    #define R1_16       2               // sixteenth rest / semiquaver rest
    #define R1_32       1               // thirty-second rest / demisemiquaver rest
    #define R1_64       0               // sixty-fourth rest / hemidemisemiquaver rest

    typedef struct
    {
            u8          RIFF[4];        // RIFF Header  
            u32         ChunkSize;      // RIFF Chunk Size
            u8          WAVE[4];        // WAVE Header
            u8          fmt[4];         // FMT header  
            u32         Subchunk1Size;  // Size of the fmt chunk (Nombre d'octets du bloc - 16  (0x10))
            u16         AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
            u16         NumOfChan;      // Number of channels 1=Mono 2=Stereo  
            u32         SamplesPerSec;  // Sampling Frequency in Hz (11025, 22050, 44100, ...)
            u32         bytesPerSec;    // bytes per second (i.e., Frequence * BytePerBloc)
            u16         blockAlign;     // 2=16-bit mono, 4=16-bit stereo
            u16         bitsPerSample;  // Number of bits per sample
            u8          Subchunk2ID[4]; // "data"  string = file size - 44
            u32         Subchunk2Size;  // Sampled data length
    } Wav_File_Header;

#endif // __AUDIO_H


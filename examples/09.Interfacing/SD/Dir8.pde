/*      ---------------------------------------------------------------------------
        List files on SD card
        2011 Regis Blanchot
        <rblanchot@gmail.com>
        http://www.pinguino.cc

        circuit:
        * SD card attached to SPI bus as follows:
        ** MOSI - pin SDO
        ** MISO - pin SDI
        ** CLK  - pin SCK
        ** CS   - pin 8

        output:
        (GNU/linux) sudo minicom -o -D /dev/ttyACM0
        press <return> to start
        ---------------------------------------------------------------------------*/

void setup()
{
}

void loop()
{
        #define MAXFILE 32

        char i, found;
        const char ext[]="TXT";
        Dir dir[MAXFILE];

        // wait for RETURN key to start
        CDC.println("Press RETURN to start.");
        while (CDC.getKey() != '\r');
        CDC.println(" ");
        
        // mount FAT16 file system
        // card select is on pin 8
        if (SD.mount(8))
        {
                CDC.println("SD successfully mounted");
                found = SD.list(dir, MAXFILE, ext);
                CDC.println("%-8s %-8s %-8s", "name", "ext", "bytes");
                CDC.println("-----------------------");
                for (i=0; i<found; i++)
                        CDC.printf("%-8s %-8s %-8d\r\n", dir[i].filename, dir[i].ext, dir[i].size);
                CDC.println("found %d file(s)", found);
                SD.unmount();
        }
        else
        {
                CDC.println("mount failed!");
        }
}
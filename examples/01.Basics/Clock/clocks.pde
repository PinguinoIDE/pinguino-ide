/*
 * This applications shows how to read and change Pinguino clock settings
 */

u32 cpuFrequency;
u32 peripheralFrequency;

void setup()
{                                       
    CDC.printf("CDC: Hello\r\n");
}

void loop()
{
    // Show current settings

    cpuFrequency = System.getCpuFrequency();
    CDC.printf("CPU       : %d MHz\r\n", cpuFrequency / 1000 / 1000);

    peripheralFrequency = System.getPeripheralFrequency();
    CDC.printf("Peripheral: %d MHz\r\n", peripheralFrequency / 1000 / 1000);

    // Change current register settings 
    // NB : On 32-bit Pinguino CPU and Peripheral frequency are independant
    // but on 8-bit Pinguino Peripheral freq. = CPU freq. / 4 
    // So System.setPeripheralFrequency(2000000); set CPU freq. to 8MHz
    // and System.setCpuFrequency(8000000); set Peripheral freq. to 2MHz
    // Be careful, not all frequencies are allowed, please take a look at datasheets

    //System.setCpuFrequency(8000000);          
    //System.setPeripheralFrequency(2000000);

    // Show new current settings

    //cpuFrequency = System.getCpuFrequency();
    //CDC.printf("\nCPU       : %d MHz\r\n", cpuFrequency / 1000 / 1000);

    //peripheralFrequency = System.getPeripheralFrequency();
    //CDC.printf("Peripheral: %d MHz\r\n", peripheralFrequency / 1000 / 1000);
    
    delay(1000);
}

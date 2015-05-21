#ifdef SERIAL_PRINT
  #include <serial.c>
#endif
#ifdef CDC_PRINT
  #include <__cdc.c>
  #include <delayus.c>
#endif

/********************************************************************
 * Function:        void PrintSectorData( BYTE* data )
 *
 * PreCondition:    None
 *
 * Input:           Pointer to a 512 byte buffer
 *
 * Output:          Humen readable data
 *
 * Side Effects:    None
 *
 * Overview:        Data is outputed in groups of 16 bytes per row
 *
 * Note:            None
 *******************************************************************/
#ifdef SERIAL_PRINT
 void disk_printp_serial( u8* datx )
{
	u16 k, px;

	for(k = 0; k < 512; k++)
	{
		Serial_printf("%2X ",datx[k]);

		if( ((k + 1) % 16) == 0)
		{
			Serial_printf("  ");

			for(px = (k - 15); px <= k; px++)
			{
				if( ((datx[px] > 33) && (datx[px] < 126)) || (datx[px] == 0x20) )
				{
					Serial_printf("%c ",datx[px]);
				}
				else
				{
					Serial_printf(".");
				}
			}

			Serial_printf("\n\r");
		}
	}

	return;
}
#endif

#ifdef CDC_PRINT
void disk_printp_CDC( u8* datx )
{
	u16 k, px;

	for(k = 0; k < 512; k++)
	{
	CDCprintf("%2X ",datx[k]); Delayus(50);
	

		if( ((k + 1) % 16) == 0)
		{
			CDCprintf("  "); Delayus(50);

			for(px = (k - 15); px <= k; px++)
			{
				if( ((datx[px] > 33) && (datx[px] < 126)) || (datx[px] == 0x20) )
				{
					CDCprintf("%c ",datx[px]); Delayus(100);
				}
				else
				{
					CDCprintf("."); Delayus(50);
				}
			}

			CDCprintf("\n\r"); Delayus(50);

		}
	}

	return;
}
#endif

void disk_printp( u8* datx )
{
#ifdef SERIAL_PRINT
  disk_printp_serial( datx);
#endif
#ifdef CDC_PRINT
  disk_printp_CDC( datx);
#endif
}

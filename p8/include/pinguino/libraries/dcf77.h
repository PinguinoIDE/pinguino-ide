/*******************************************************************************
 * DCF time format struct
*******************************************************************************/

typedef struct
{
	u8  seconds;
	u8	minutes;
	u8	hours;
	u8  dayofweek;
	u8	dayofmonth;
	u8	month;
	u8	year;
	u8	nosync;		// nosync shows minutes
} Time_Date_Format;	// record is not updated by DCF77
					// Reset to 0 when record is updated by DCF77 module
Time_Date_Format RTClock, DCF77;

/*******************************************************************************
* Prototypes
*******************************************************************************/

void DCF77_start(u8);

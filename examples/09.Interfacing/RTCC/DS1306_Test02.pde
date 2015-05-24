/*
 * File			DS1306_Test02.pde
 *
 * Description		Full test program for DS1306 devices. 
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 * 			Original author Chris Bearman
 *			Original project https://github.com/cjbearman/ds1306-arduino
 *
 * Instructions		Connect USB port, open a terminal emulator and hit a key to start phase.
 *
 *   18/06/2013 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/

/* You should ONLY define __CHARGING_SUPPORTED is you have a chargable battery or super-cap
   properly connected, per DS1306 spec sheet */
   
/* #define __CHARGING_SUPPORTED */

//RTC IO
#define RTC_E          4
#define SCK            5
#define SDO            6
#define SDI            7

char buf[100];

// This test will test both the 24 and 12 hour write/read modes for the clock
// using these two objects

// Convert a 12 hour + ampm to a 24 hour  
unsigned char convert_to_24(unsigned char hours12, char ampm)
{
  if (ampm == 'A') {
    if (hours12 == 12) return 0;
    return hours12;
  } else if (ampm == 'P') {
    if (hours12 == 12) return 12;
    return hours12+12;
  } else {
    return 255;  // Error signal because of missing ampm
  }
}

// Convert a 24 hour to a 12 hour format
void convert_to_12(unsigned char hours24, unsigned char *hours12, char *ampm)
{
  if (hours24 == 0) {
    *hours12 = 12;
    *ampm = 'A';
  } else if (hours24 == 12) {
    *hours12 = 12;
    *ampm = 'P';
  } else if (hours24 < 12) {
    *hours12 = hours24;
    *ampm = 'A';
  } else {
    *hours12 = hours24 - 12;
    *ampm = 'P';
  }
} 

char bf[100],bf1[30];
// Compare two bytes, return true if the same, FALSE otherwise.
// Output error to serial on FALSE
BOOL compare(u8 in, u8 out, char *desc, BOOL *passflag, int aschar)
{
 if (in != out)
  {
   bf[0] = 0;
   if (*passflag)
    {
     *passflag = FALSE;
     strcat(bf,"Fail\r\n");
    }
     strcat(bf,"..Failure comparing %s - in=");
   if (aschar)
    Sprintf(bf1,"%c",(char) in);
   else
    Sprintf(bf1,"%d",in);
   strcat(bf,bf1);
   strcat(bf,", out=");
   if (aschar)
    Sprintf(bf1,"%c\r\n",(char) out);
   else
    Sprintf(bf1,"%d\r\n",out);
   strcat(bf,bf1);
  }
 CDC.printf("%s",bf);
}

// Routine to write and read the clock using 24 hour format
// and check the two values against each other
BOOL read_write_compare_clock24(char *tc, unsigned char year, unsigned char month, unsigned char day, unsigned char hours, unsigned char mins, unsigned char secs, unsigned char dow)
{
  CDC.printf("%s",tc);
  CDC.print(" Read/Write/Compare Main Clock 24hr - ");

  ds1306time ts_in, ts_out;
  ts_in.year = year;
  ts_in.month = month;
  ts_in.day = day;
  ts_in.dow=dow;
  ts_in.hours=hours;
  ts_in.minutes=mins;
  ts_in.seconds=secs;
  
  DS1306.setTime(&ts_in);
  DS1306.getTime(&ts_out);

  BOOL pass = true;
  
  unsigned char hours12;
  char ampm;
  
  convert_to_12(ts_in.hours, &hours12, &ampm);

  compare(ts_in.year, ts_out.year, "Year", &pass,FALSE);
  compare(ts_in.month, ts_out.month, "Month", &pass,FALSE);
  compare(ts_in.day, ts_out.day, "Day", &pass,FALSE);
  compare(ts_in.hours, ts_out.hours, "Hours", &pass,FALSE);
  compare(hours12, ts_out.hours12, "Hours (12)", &pass,FALSE);
  compare(ampm, ts_out.ampm, "AMPM Flag", &pass, TRUE);
  compare(ts_in.minutes, ts_out.minutes, "Minutes", &pass,FALSE);
  compare(ts_in.seconds, ts_out.seconds, "Seconds", &pass,FALSE);
  compare(ts_in.dow, ts_out.dow, "DOW", &pass,FALSE);
  
  if (pass) CDC.print("Pass\r\n");
  
  return pass; 
}


// Routine to write and read and alarm using 24 hour format
// and check the two values against each other
BOOL read_write_compare_alarm24(char *tc, int alarm, unsigned char hours, unsigned char mins, unsigned char secs, unsigned char dow)
{
  CDC.printf("%s",tc);
  CDC.print(" Read/Write/Compare Alarm 24hr - ");

  ds1306alarm ts_in, ts_out;
  ts_in.dow=dow;
  ts_in.hours=hours;
  ts_in.minutes=mins;
  ts_in.seconds=secs;
  
  DS1306.setAlarm(alarm, &ts_in);
  DS1306.getAlarm(alarm, &ts_out);

  BOOL pass = true;
  
  unsigned char hours12;
  char ampm;
  
  convert_to_12(ts_in.hours, &hours12, &ampm);

  compare(ts_in.hours, ts_out.hours, "Hours", &pass,FALSE);
  if (ts_in.hours != DS1306_ANY) compare(hours12, ts_out.hours12, "Hours (12)", &pass,FALSE);
  if (ts_in.hours != DS1306_ANY) compare(ampm, ts_out.ampm, "AMPM Flag", &pass, TRUE);
  compare(ts_in.minutes, ts_out.minutes, "Minutes", &pass,FALSE);
  compare(ts_in.seconds, ts_out.seconds, "Seconds", &pass,FALSE);
  compare(ts_in.dow, ts_out.dow, "DOW", &pass,FALSE);
  
  if (pass) CDC.println("Pass");
  
  return pass; 
}

// Routine to write and read and alarm using 12 hour format
// and check the two values against each other
BOOL read_write_compare_alarm12(char *tc, int alarm, unsigned char hours, char ampm, unsigned char mins, unsigned char secs, unsigned char dow)
{
  CDC.printf("%s",tc);
  CDC.print(" Read/Write/Compare Alarm 12hr - ");

  ds1306alarm ts_in, ts_out;
  ts_in.dow=dow;
  ts_in.hours12=hours;
  ts_in.ampm=ampm;
  ts_in.minutes=mins;
  ts_in.seconds=secs;
  
  DS1306.Hours24(FALSE);
  DS1306.setAlarm(alarm, &ts_in);
  DS1306.getAlarm(alarm, &ts_out);

  BOOL pass = true;
  
  unsigned char hours24;
  
  hours24 = convert_to_24(ts_in.hours12, ampm);
  compare(ts_in.hours12, ts_out.hours12, "Hours", &pass,FALSE);
  if (ts_in.hours12 != DS1306_ANY) compare(ts_in.ampm, ts_out.ampm, "AM PM Flag", &pass,FALSE);
  if (ts_in.hours12 != DS1306_ANY) compare(hours24, ts_out.hours, "Hours (24)", &pass,FALSE);
  compare(ts_in.minutes, ts_out.minutes, "Minutes", &pass,FALSE);
  compare(ts_in.seconds, ts_out.seconds, "Seconds", &pass,FALSE);
  compare(ts_in.dow, ts_out.dow, "DOW", &pass,FALSE);
  
  if (pass) CDC.print("Pass\r\n");
  
  return pass; 
}

// Validate that the two alarms do not match each other
BOOL check_alarm_diff(char *tc)
{
  ds1306alarm a0, a1;

  CDC.printf("%s",tc);  
  CDC.print(" Compare alarms are different - ");
  DS1306.getAlarm(0, &a0);
  DS1306.getAlarm(1, &a1);
  
  if (a0.hours == a1.hours &&
      a0.minutes == a1.minutes &&
      a0.seconds == a1.seconds &&
      a0.dow == a1.dow) {
    CDC.print("Fail\r\n");
    return FALSE;
  } else {    
    CDC.print("Pass\r\n");
    return true;
  }
}  

// Run tests related to the primary clock
int clocktests()
{
  int failures = 0;
  // RWC24 series tests
  // Write clock (24 hour mode), Read clock, compare written timestamp with read timestamp
  
  // RWC24.01 - Lower bound (lowest supportable values)
  if (!read_write_compare_clock24("RWC24.01", 0, 1, 1, 0, 0, 0, DS1306_SUNDAY)) failures++;

  // RWC24.02 - Upper bound (maximum supportable values)
  if (!read_write_compare_clock24("RWC24.02", 99, 12, 31, 23, 59, 59, DS1306_SATURDAY)) failures++;
  
  // RWC24.03 - Check operation of 12th hour for PM decode, unique values for ALL fields
  if (!read_write_compare_clock24("RWC24.03", 1, 2, 3, 12, 4, 5, DS1306_SATURDAY)) failures++;
  
  // RWC12 series tests
  // Write clock (12 hour mode), Read clock, compare written timestamp with read timestamp
  
  return failures;
}

// Run tests related to primary alarm setting / retrieval
int alarmtests1()
{  
  int failures = 0;
  // ALM24 series tests
  // Write alarm (1 or 2), Read back, compare written and read values
  
  // ALM24.01 - Lower bound (lowest supportable values)
  if (!read_write_compare_alarm24("ALM24.01", 0, 0, 0, 0, DS1306_SUNDAY)) failures++;
  
  // ALM24.02 - Upper bound (highest supportable values)
  if (!read_write_compare_alarm24("ALM24.02", 0, 23, 59, 59, DS1306_SATURDAY)) failures++;
  
  // ALM24.03 - Check operation of 12th hour for PM decode, unique values for ALL fields
  if (!read_write_compare_alarm24("ALM24.03", 0, 12, 2, 3, DS1306_FRIDAY)) failures++;

  // Repeat previous 3 tests for second alarm (alarm 1)
  if (!read_write_compare_alarm24("ALM24.11", 1, 0, 0, 0, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm24("ALM24.12", 1, 23, 59, 59, DS1306_SATURDAY)) failures++;
  if (!read_write_compare_alarm24("ALM24.13", 1, 12, 2, 3, DS1306_FRIDAY)) failures++;

  // Do some testing of the ANY indicator in all fields, alternating alarm 1 and alarm 2
  // to ensure that both alarms end up on different values for a further test
  if (!read_write_compare_alarm24("ALM24.21", 0, DS1306_ANY, 1, 2, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm24("ALM24.22", 1, 1, DS1306_ANY, 2, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm24("ALM24.24", 0, 1, 2, DS1306_ANY, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm24("ALM24.23", 1, 1, 2, 3, DS1306_ANY)) failures++;

  // ALM12 series tests
  // Write alarm (1 or 2), Read back, compare written and read values
  
  // ALM12.01 - Lower bound (lowest supportable values)
  if (!read_write_compare_alarm12("ALM12.01", 0, 12, 'A', 0, 0, DS1306_SUNDAY)) failures++;
  
  // ALM12.02 - Upper bound (highest supportable values)
  if (!read_write_compare_alarm12("ALM12.02", 0, 11, 'P', 59, 59, DS1306_SATURDAY)) failures++;
  
  // ALM12.03 - Check operation of 12th hour for PM decode, unique values for ALL fields
  if (!read_write_compare_alarm12("ALM12.03", 0, 12, 'P', 2, 3, DS1306_FRIDAY)) failures++;

  // Repeat previous 3 tests for second alarm (alarm 1)
  if (!read_write_compare_alarm12("ALM12.11", 1, 0, 'A', 0, 0, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm12("ALM12.12", 1, 11, 'P', 59, 59, DS1306_SATURDAY)) failures++;
  if (!read_write_compare_alarm12("ALM12.13", 1, 12, 'A', 2, 3, DS1306_FRIDAY)) failures++;

  // Do some testing of the ANY indicator in all fields, alternating alarm 1 and alarm 2
  // to ensure that both alarms end up on different values for a further test
  if (!read_write_compare_alarm12("ALM12.21", 0, DS1306_ANY, '\0', 1, 2, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm12("ALM12.22", 1, 1, 'P', DS1306_ANY, 2, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm12("ALM12.24", 0, 1, 'A', 2, DS1306_ANY, DS1306_SUNDAY)) failures++;
  if (!read_write_compare_alarm12("ALM12.23", 1, 1, 'P', 2, 3, DS1306_ANY)) failures++;

  DS1306.Hours24(TRUE);

  // Make sure that the alarms are set differently
  if (!check_alarm_diff("ALMXX.01")) failures++;

  return failures;
}

// Run tests related to alarm enable / disable / state and triggering
int runalarmtests2(int alarmnumber, char *prefix)
{
  int failures = 0;
  
  // Start off by setting alarm as on
  CDC.printf("%s",prefix);
  CDC.print(".01 Set alarm to disabled - " );
  DS1306.enableAlarm(alarmnumber);
  if (DS1306.getAlarmEnabled(alarmnumber)) {
    CDC.print("Pass\r\n");
  } else {
    CDC.print("Fail\r\n");
    failures++;
  }
  
  // Now set alarm as off
  CDC.printf("%s",prefix);
  CDC.print(".02 Set alarm to enabled - ");
  DS1306.disableAlarm(alarmnumber);
  if (!DS1306.getAlarmEnabled(alarmnumber)) {
    CDC.print("Pass\r\n");
  } else {
    CDC.print("Fail\r\n");
    failures++;
  }
  
  CDC.printf("%s",prefix);
  CDC.print(".03 Trigger alarm - ");
  ds1306alarm a;
  a.dow = DS1306_ANY;
  a.hours = DS1306_ANY;
  a.minutes = DS1306_ANY;
  a.seconds = DS1306_ANY;
  
  DS1306.clearAlarmState(alarmnumber);
  if (DS1306.getAlarmState(alarmnumber)) {
    CDC.print("Fail\r\n");
    CDC.print("..Alarm did not clear\r\n");
    failures++;
  } else {
    DS1306.setAlarm(alarmnumber, &a);
    delay(2000);
    if (!DS1306.getAlarmState(alarmnumber)) {
      CDC.print("Fail\r\n");
      CDC.print("..Alarm did not trigger\r\n");
      failures++;
    } else {
      CDC.print("Pass\r\n");
    }
  }
  DS1306.disableAlarm(alarmnumber);
  
  return failures;
}
  
  
// Run functional alarm tests
int alarmtests2()
{
  int failures = 0;

  failures += runalarmtests2(0, "AL0FN");  
  //Cannot run alarm test 2 for 1306 like we can for the 1305 because
  //it only works when the clock is running on backup
  //failures += runalarmtests2(1, "AL1FN");
  return failures;
}

// Run user memory tests
int usermemtests()
{
  int failures = 0;
  char outbuf[128];
  char inbuf[128];
  int i;
  BOOL fail;
  
  CDC.print("USMEM.01 Read/Write Sequential Pattern - ""...");
  
  memset(inbuf, 0, 128);
  memset(outbuf, 0, 128);
  
  // Write an obvious test pattern into buf
  for (i = 0 ; i < 96 ; i ++) {
    outbuf[i] = i;
  }
  
  DS1306.writeUser(DS1306_USER_START, outbuf, 96);
  DS1306.readUser(DS1306_USER_START, inbuf, 96);
  
  fail = FALSE;
  for (i = 0 ; i < 96 ; i++) {
    if (inbuf[i] != outbuf[i]) {
      fail = true;
      break;
    }
  }
  
  if (fail) {
    CDC.print("Fail\r\n");
    CDC.print("..Offset = 0x");
    CDC.printNumber(i, HEX);
    failures++;
  } else {
    CDC.print("Pass\r\n");
  }
  
    CDC.print("USMEM.02 Read/Write Random Pattern - ");
  
  memset(inbuf, 0, 128);
  memset(outbuf, 0, 128);
  
  // Write a (pseudo) random value test pattern into buf
  for (i = 0 ; i < 96 ; i ++) {
    outbuf[i] = rand() % 0x100;
  }
  
  DS1306.writeUser(DS1306_USER_START, outbuf, 96);
  DS1306.readUser(DS1306_USER_START, inbuf, 96);
  
  fail = FALSE;
  for (i = 0 ; i < 96 ; i++) {
    if (inbuf[i] != outbuf[i]) {
      fail = true;
      break;
    }
  }
  
  if (fail) {
    CDC.print("Fail\r\n");
    CDC.print("..Offset = 0x");
    CDC.printNumber(i, HEX);
    failures++;
  } else {
    CDC.print("Pass\r\n");
  }
  
  return failures;
}

// Run control register tests
int crtests()
{
  int failures = 0;
  
  CDC.print("CRRWP.01 Write Protect Clock - ");
  DS1306.setWriteProtection(true);
  if (DS1306.isWriteProtected()) {
    CDC.print("Pass\r\n");
  } else {
    failures++;
    CDC.print("Fail\r\n");
  }
  
  CDC.print("CRRWP.02 Write Unprotect Clock - ");
  DS1306.setWriteProtection(FALSE);
  if (!DS1306.isWriteProtected()) {
    CDC.print("Pass\r\n");
  } else {
    failures++;
    CDC.print("Fail\r\n");
  }
  
#ifdef __CHARGING_SUPPORTED
  CDC.print("TCREN.01 Enable Trickle Charge - ");
  DS1306.enableTrickleCharge(2, 8);
  unsigned char diodes;
  unsigned char resistance;
  BOOL en = DS1306.getTrickleChargeState(&diodes, &resistance);
  if (diodes != 2 || resistance != 8 || en == FALSE) {
    CDC.print("Fail\r\n");
    if (diodes != 2) {
      CDC.print("..Expected 2 diodes, got ");
      CDC.printNumber(diodes, DEC);
    }
    if (resistance != 8) {
      CDC.print("..Expected resistance of 8, got ");
      CDC.printNumber(resistance, DEC);
    }
    if (en != true) {
      CDC.print("..Got disabled indication\r\n");
    }
    failures++;
    
  } else {
    CDC.print("Pass\r\n");
  }
  
  CDC.print("TCREN.02 Disable Trickle Charge - ");
  DS1306.disableTrickleCharge();
  en = DS1306.getTrickleChargeState(&diodes, &resistance);
  if (diodes != 0 || resistance != 0 || en == true) {
    CDC.print("Fail\r\n");
    if (diodes != 0) {
      CDC.print("..Expected 0 diodes, got ");
      CDC.printNumber(diodes, DEC);
    }
    if (resistance != 0) {
      CDC.print("..Expected resistance of 0, got ");
      CDC.printNumber(resistance, DEC);
    }
    if (en == true) {
      CDC.println("..Got disabled indication");
    }
    failures++;
    
  } else {
    CDC.print("Pass\r\n");
  }
#else
  CDC.println("TCREN.01 Enable Trickle Charge - Skipped");
  CDC.println("TCREN.02 Disable Trickle Charge - Skipped");
#endif

  CDC.print("TCREN.03 Enable 1KHZ Signal - ");
  DS1306.set1HzState(true);
  if (!DS1306.get1HzState()) {
    CDC.print("Fail\r\n");
    failures++;
  } else {
    CDC.print("Pass\r\n");
  }

  CDC.print("TCREN.04 Disable 1KHZ Signal - ");
  DS1306.set1HzState(FALSE);
  if (DS1306.get1HzState()) {
    CDC.print("Fail\r\n");
    failures++;
  } else {
    CDC.print("Pass\r\n");
  }

  return failures;
}
  
// Setup routine
void setup()
{
 pinMode(USERLED, OUTPUT); 
 digitalWrite(USERLED,LOW); 
// randomSeed(12);
 srand(123);
 DS1306.init(0,RTC_E,SCK,SDO,SDI); 
 CDC.getKey();
}


// Main run loop
void loop()
{
  int failures = 0; 
  int i;

  digitalWrite(USERLED,HIGH); 
  // Provide a countdown to start
  for (i = 5 ; i > 0; i--)
   {
    CDC.printf("Test will start in %d ...\r\n",i);
    delay(500);
   }

   
  // Run all tests
  CDC.println("Test starting");
  failures+=clocktests();
  failures+=alarmtests1();
  failures+=alarmtests2();
  failures+=usermemtests();
  failures+=crtests();

  // Report on outcome
  CDC.println("");
  CDC.println("");
  CDC.print("Final result - ");
  CDC.println(failures == 0 ? "Pass" : "Fail");
  if (failures) {
    CDC.printNumber(failures, DEC);
    CDC.println(" total failures");
  }
    
  while(true) { };
}



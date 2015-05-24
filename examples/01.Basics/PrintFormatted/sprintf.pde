/*	----------------------------------------------------------------------------
	sprintf - Régis Blanchot
 	Choose Serial or CDC output
	--------------------------------------------------------------------------*/

void setup()
{
	Serial.begin(9600);		// initialize UART
}

void loop()
{
	char buf[80];

	// Serial Output
	Sprintf(buf, "|justif: \"%-10s\"|\n", "left");	Serial.printf("%s", buf);
	Sprintf(buf, "|justif: \"%10s\"|\n", "right");	Serial.printf("%s", buf);
	Sprintf(buf, "| 3: %04d zero padded|\n", 3);	Serial.printf("%s", buf);
	Sprintf(buf, "| 3: %-4d left justif.|\n", 3);	Serial.printf("%s", buf);
	Sprintf(buf, "| 3: %4d right justif.|\n", 3);	Serial.printf("%s", buf);
	Sprintf(buf, "|-3: %04d zero padded|\n", -3);	Serial.printf("%s", buf);
	Sprintf(buf, "|-3: %-4d left justif.|\n", -3);	Serial.printf("%s", buf);
	Sprintf(buf, "|-3: %4d right justif.|\n", -3);	Serial.printf("%s", buf);

	// CDC Output
/*
	Sprintf(buf, "|justif: \"%-10s\"|\n", "left");	CDC.printf("%s", buf);
	Sprintf(buf, "|justif: \"%10s\"|\n", "right");	CDC.printf("%s", buf);
	Sprintf(buf, "| 3: %04d zero padded|\n", 3);	CDC.printf("%s", buf);
	Sprintf(buf, "| 3: %-4d left justif.|\n", 3);	CDC.printf("%s", buf);
	Sprintf(buf, "| 3: %4d right justif.|\n", 3);	CDC.printf("%s", buf);
	Sprintf(buf, "|-3: %04d zero padded|\n", -3);	CDC.printf("%s", buf);
	Sprintf(buf, "|-3: %-4d left justif.|\n", -3);	CDC.printf("%s", buf);
	Sprintf(buf, "|-3: %4d right justif.|\n", -3);	CDC.printf("%s", buf);
*/
}

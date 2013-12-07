void setup()
{
    int seed;
    seed = millis();
    randomSeed(seed);
}

void loop()
{
    int r;
    r = random(1, 10);
    CDC.printf("random=%d\r\n", r);
    delay(1000);
}

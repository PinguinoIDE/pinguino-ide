/********************************************************
* PID Basic Example
* Reading analog input 0 to control analog PWM output 3
********************************************************/

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

void setup()
{
    //initialize the variables we're linked to
    Input = analogRead(0);
    Setpoint = 100;

    //Specify the links and initial tuning parameters
    PID.init(&Input, &Output, &Setpoint,2,5,1, DIRECT);

    //turn the PID on
    PID.setMode(AUTOMATIC);
}

void loop()
{
    Input = analogRead(0);
    PID.compute();
    analogWrite(3,Output);
}



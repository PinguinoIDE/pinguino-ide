/**********************************************************************************************
* Arduino PID Library - Version 1.0.1
* by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
* adapted to Pinguino by Regis Blanchot (2012 Jul 10)
* This Library is licensed under a GPLv3 License
**********************************************************************************************/

#ifndef __PID_C
#define __PID_C

#include <typedef.h>
#include <pid.h>
#include <millis.c>

/*Constructor (...)*********************************************************
* The parameters specified here are those for for which we can't set up
* reliable defaults, so we need to have the user set them.
***************************************************************************/
void PID_init(double* Input, double* Output, double* Setpoint, double Kp, double Ki, double Kd, int ControllerDirection)
{
    PID_SetOutputLimits(0, 255); //default output limit corresponds to the arduino pwm limits

    SampleTime = 100; //default Controller Sample Time is 0.1 seconds

    PID_SetControllerDirection(ControllerDirection);
    PID_SetTunings(Kp, Ki, Kd);

    lastTime = millis()-SampleTime;
    inAuto = false;
    myOutput = Output;
    myInput = Input;
    mySetpoint = Setpoint;
}
 
/* Compute() **********************************************************************
* This, as they say, is where the magic happens. this function should be called
* every time "void loop()" executes. the function will decide for itself whether a new
* pid Output needs to be computed
**********************************************************************************/
void PID_Compute()
{
    unsigned long now, timeChange;
    double input, error, dInput, output;

    if(!inAuto) return;

    now = millis();
    timeChange = (now - lastTime);
    
    if(timeChange>=SampleTime)
    {
        /*Compute all the working error variables*/
        input = *myInput;
        error = *mySetpoint - input;
        ITerm+= (ki * error);
        if(ITerm > outMax) ITerm= outMax;
        else if(ITerm < outMin) ITerm= outMin;
        dInput = (input - lastInput);

        /*Compute PID Output*/
        output = kp * error + ITerm- kd * dInput;

        if(output > outMax) output = outMax;
          else if(output < outMin) output = outMin;
        *myOutput = output;

        /*Remember some variables for next time*/
        lastInput = input;
        lastTime = now;
    }
}


/* SetTunings(...)*************************************************************
* This function allows the controller's dynamic performance to be adjusted.
* it's called automatically from the constructor, but tunings can also
* be adjusted on the fly during normal operation
******************************************************************************/
void PID_SetTunings(double Kp, double Ki, double Kd)
{
    double SampleTimeInSec;

    if (Kp<0 || Ki<0 || Kd<0) return;

    dispKp = Kp; dispKi = Ki; dispKd = Kd;

    SampleTimeInSec = ((double)SampleTime)/1000;
    kp = Kp;
    ki = Ki * SampleTimeInSec;
    kd = Kd / SampleTimeInSec;

    if(controllerDirection ==REVERSE)
    {
        kp = (0 - kp);
        ki = (0 - ki);
        kd = (0 - kd);
    }
}
  
/* SetSampleTime(...) *********************************************************
* sets the period, in Milliseconds, at which the calculation is performed
******************************************************************************/
void PID_SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio = (double)NewSampleTime
                      / (double)SampleTime;
      ki *= ratio;
      kd /= ratio;
      SampleTime = (unsigned long)NewSampleTime;
   }
}
 
/* SetOutputLimits(...)****************************************************
* This function will be used far more often than SetInputLimits. while
* the input to the controller will generally be in the 0-1023 range (which is
* the default already,) the output will be a little different. maybe they'll
* be doing a time window and will need 0-8000 or something. or maybe they'll
* want to clamp it from 0-125. who knows. at any rate, that can all be done
* here.
**************************************************************************/
void PID_SetOutputLimits(double Min, double Max)
{
   if(Min >= Max) return;
   outMin = Min;
   outMax = Max;
 
   if(inAuto)
   {
if(*myOutput > outMax) *myOutput = outMax;
else if(*myOutput < outMin) *myOutput = outMin;

if(ITerm > outMax) ITerm= outMax;
else if(ITerm < outMin) ITerm= outMin;
   }
}

/* SetMode(...)****************************************************************
* Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
* when the transition from manual to auto occurs, the controller is
* automatically initialized
******************************************************************************/
void PID_SetMode(int Mode)
{
    u8 newAuto = (Mode == AUTOMATIC);
    if(newAuto == !inAuto)
    { /*we just went from manual to auto*/
        PID_Initialize();
    }
    inAuto = newAuto;
}
 
/* Initialize()****************************************************************
* does all the things that need to happen to ensure a bumpless transfer
* from manual to automatic mode.
******************************************************************************/
void PID_Initialize()
{
   ITerm = *myOutput;
   lastInput = *myInput;
   if(ITerm > outMax) ITerm = outMax;
   else if(ITerm < outMin) ITerm = outMin;
}

/* SetControllerDirection(...)*************************************************
* The PID will either be connected to a DIRECT acting process (+Output leads
* to +Input) or a REVERSE acting process(+Output leads to -Input.) we need to
* know which one, because otherwise we may increase the output when we should
* be decreasing. This is called from the constructor.
******************************************************************************/
void PID_SetControllerDirection(int Direction)
{
   if(inAuto && Direction !=controllerDirection)
   {
kp = (0 - kp);
      ki = (0 - ki);
      kd = (0 - kd);
   }
   controllerDirection = Direction;
}

/* Status Funcions*************************************************************
* Just because you set the Kp=-1 doesn't mean it actually happened. these
* functions query the internal state of the PID. they're here for display
* purposes. this are the functions the PID Front-end uses for example
******************************************************************************/
double PID_GetKp(){ return dispKp; }
double PID_GetKi(){ return dispKi;}
double PID_GetKd(){ return dispKd;}
int PID_GetMode(){ return inAuto ? AUTOMATIC : MANUAL;}
int PID_GetDirection(){ return controllerDirection;}

#endif


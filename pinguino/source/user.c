









int __i__ = 0;

setup(){
    pinmode(13, OUTPUT);
    }

loop(){
    for(__i__ = 0;__i__ < 10;__i__ = __i__+1)
    {
        toggle(13);
        Delayms(100);
        }
    Delayms(1000);
    }




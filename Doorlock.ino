/*
  Copyright (C) 2014 DaKnOb <daknob.mac@gmail.com>
  Code for ToLABaki <saloni@tolabaki.gr> http://tolabaki.gr/
*/

/* Configuration, must match the configuration in doorlockd.py */
static const int StepsToRotate = 700;  /* How many steps back and FoRTH to move */
static const int SerialOpen    = 0x05; /* Serial byte to open the door at */
static const int SerialAck     = 0x06; /* Serial byte to send on packet receipt */
static const int SerialNack    = 0x15; /* Serial byte to send on packet decoding issue */
static const int SerialOk      = 0x11; /* Serial byte to send upon success */
static const int SerialNo      = 0x12; /* Serial byte to send upon failure */

/* Pins */
static const int Step   = 2; /* The pin to send a step pulse */
static const int Dir    = 3; /* The pin to send a direction logical value */
static const int Enable = 4; /* The pin to NOT enable the chip */
static const int Error  = 5; /* The pin to send a blink pattern on error */
static const int Switch = 6; /* The pin for the switch inside ToLABaki */

/*********************************************
 * No editing required below this line here. *
 *********************************************/

/* Define some easy names */
#define CLOCKWISE  LOW

/* Declare all variables */
int sBuff = 0;       /* Serial Buffer */

/* Set appropriate pin modes */
void setup(){

    pinMode(Step, OUTPUT);
    pinMode(Dir, OUTPUT);
    pinMode(Error, OUTPUT);
    pinMode(Switch, INPUT);
    pinMode(Enable, OUTPUT);
    /* start with the stepper driver disabled */
    digitalWrite(Enable, HIGH);
    digitalWrite(Dir, CLOCKWISE);
    /* Initialize Serial connection with a computer */
    Serial.begin(9600);
    delay(100);

}

/* Run the actual code to use sensors for open and close */
void loop(){

    if (Serial.available() > 0){ /* Full Sensoring */
        sBuff = Serial.read();   /* Read one byte from the Serial port */
        if(sBuff == SerialOpen){
            openDoor();
            Serial.write(SerialOk);
        }else{
            Serial.write(SerialNack);
        }
    }

    /* Open the door using the internal switch */
    if(digitalRead(Switch) == HIGH){
        openDoor();
    }

}

/* Opens door if not open */
void openDoor(){

    /* Enable the stepper motor driver */
    digitalWrite(Enable, LOW);
    /* give it some time to boot */
    delay(1700); /* Normally it takes 1600, but
                  * just to be on the safe side */

    /* We turn the gear clockwise, so the other gear spins
     * counterclockwise */
    sendRotatePulse();

    /* keep it open for one second */
    delay(1000);
    /* Disable the stepper motor driver, the handle spring should
     * bring it to it's normal state. */
    digitalWrite(Enable, HIGH);

}

/* Display an 1 second error blink on the Error LED */
void displayError(){
    int i;

    for(i=1; i<=10; i++) {
        digitalWrite(Error, HIGH);
        delay(100);
        digitalWrite(Error, LOW);
        delay(100);
    }

}

/* Spin the motor a known amount of times */
void sendRotatePulse(){
    int i;

    for(i=1; i<=StepsToRotate; i++) {
        digitalWrite(Step, HIGH);
        delay(1);
        digitalWrite(Step, LOW);
        delay(1);
    }

}

/* Local Variables: */
/* tab-width: 4 */
/* c-basic-offset: 4 */
/* indent-tabs-mode: nil */
/* End: */

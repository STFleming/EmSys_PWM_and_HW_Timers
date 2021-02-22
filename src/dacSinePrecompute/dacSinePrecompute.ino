// dacSin -- Write a sin wave to the ESP32 DAC pins with the arduino library
// author: stf

const unsigned int dacPIN = 25;

uint8_t sine_lookup[200] = {100,103,106,109,112,115,118,121,124,127,130,133,136,139,142,145,148,150,153,156,158,161,163,166,168,170,172,175,177,179,180,182,184,186,187,189,190,191,192,194,195,196,196,197,198,198,199,199,199,199,200,199,199,199,199,198,198,197,196,196,195,194,192,191,190,189,187,186,184,182,180,179,177,175,172,170,168,166,163,161,158,156,153,150,148,145,142,139,136,133,130,127,124,121,118,115,112,109,106,103,99,96,93,90,87,84,81,78,75,72,69,66,63,60,57,54,51,49,46,43,41,38,36,33,31,29,27,24,22,20,19,17,15,13,12,10,9,8,7,5,4,3,3,2,1,1,0,0,0,0,0,0,0,0,0,1,1,2,3,3,4,5,7,8,9,10,12,13,15,17,19,20,22,24,27,29,31,33,36,38,41,43,46,49,51,54,57,60,63,66,69,72,75,78,81,84,87,90,93,96};

void setup() {

}

uint8_t idx = 0;

void loop() {
        idx = idx + 1;
        if(idx >= 200)
                idx = 0;
        dacWrite(dacPIN, sine_lookup[idx]); 
}
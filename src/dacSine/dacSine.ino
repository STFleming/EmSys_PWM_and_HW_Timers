// dacSin -- Write a sin wave to the ESP32 DAC pins with the arduino library
// author: stf

const unsigned int dacPIN = 25;

uint8_t sine_lookup[200];
const float conversionFactor = (2*PI)/200;

void setup() {
        float radAngle;
        for(unsigned int angle=0; angle<200; angle++){
                radAngle = angle*conversionFactor;
                sine_lookup[angle] = (sin(radAngle)*100) + 100;
        }
}

uint8_t idx = 0;

void loop() {
        idx = idx + 1;
        if(idx >= 200)
                idx = 0;
        dacWrite(dacPIN, sine_lookup[idx]); 
        //dacWrite(dacPIN, (sin(idx*conversionFactor)*100) + 100); 
}

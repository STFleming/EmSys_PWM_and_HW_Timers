// dacWrite -- Write to the ESP32 DAC pins with the arduino library
// author: stf

const unsigned int dacPIN = 25;

void setup() {

}

uint8_t value = 0;

void loop() {
        value = value + 1;
        if(value >= 200)
                value = 0;
        dacWrite(dacPIN, value); 
}

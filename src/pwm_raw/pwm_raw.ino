// Code to play about with the PWM using direct register reads and writes

const unsigned int pwmPin = 15;

// setting PWM properties
const unsigned int freq = 5000;
const unsigned int pwmChannel = 0;
const unsigned int resolution = 8;

unsigned int * LEDC_HSCH0_DUTY_REG = (unsigned int *)(0x3FF59008);
unsigned int * LEDC_HSCH0_CONF0_REG = (unsigned int *)(0x3FF59000);
unsigned int * LEDC_HSCH0_CONF1_REG = (unsigned int *)(0x3FF5900C);

void setup() {
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(pwmPin, pwmChannel);
}


void loop() {
       
       //ledcWrite(pwmChannel, 128);
       *LEDC_HSCH0_CONF0_REG = (1 << 2);

       for(int i=0; i<255; i++) {
           *LEDC_HSCH0_DUTY_REG = (i << 4);
           *LEDC_HSCH0_CONF1_REG = (1 << 31);
           delayMicroseconds(100);
           //delay(3);
       }

}

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

       *LEDC_HSCH0_DUTY_REG = (0 << 4);
       *LEDC_HSCH0_CONF1_REG = (1 << 31);

       delay(1000);

       for(int i=0; i<50; i++) {
           *LEDC_HSCH0_DUTY_REG = (i << 4);
           *LEDC_HSCH0_CONF1_REG = (1 << 31);
            delay(500);
        }

       for(int i=50; i>0; i--) {
           *LEDC_HSCH0_DUTY_REG = (i << 4);
           *LEDC_HSCH0_CONF1_REG = (1 << 31);
            delay(500);
        }

       *LEDC_HSCH0_DUTY_REG = (0 << 4);
       *LEDC_HSCH0_CONF1_REG = (1 << 31);

       delay(1000);

}

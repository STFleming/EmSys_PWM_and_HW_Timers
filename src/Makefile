PORT=/dev/ttyUSB0
EMSYS_COMP=~/EmSys/arduino-cli compile --fqbn esp32:esp32:tinypico
EMSYS_UPLOAD=~/EmSys/arduino-cli upload --fqbn esp32:esp32:tinypico

.PHONY: dacWrite dacSine dacSinePrecompute

dacWrite: dacWrite/dacWrite.ino
		$(EMSYS_COMP) dacWrite 
		$(EMSYS_UPLOAD) -p $(PORT) dacWrite 

dacSine: dacSine/dacSine.ino
		$(EMSYS_COMP) dacSine 
		$(EMSYS_UPLOAD) -p $(PORT) dacSine 

dacSinePrecompute: dacSinePrecompute/main.cpp dacSinePrecompute/dacSinePrecompute.ino
		g++ -o dacSinePrecompute/precompute dacSinePrecompute/main.cpp
		$(EMSYS_COMP) dacSinePrecompute 
		$(EMSYS_UPLOAD) -p $(PORT) dacSinePrecompute 

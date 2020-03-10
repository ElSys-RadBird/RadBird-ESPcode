
const bool debug = true;	// Boolean for setting the program in its debug mode

int radarPin = 13;			// Pin connected to radar sensor
// int ledPin = LED_BUILTIN;	// Debug LED
volatile bool activity;		// Is the radar reading anything?
bool bird = false;			// Is there a bird present? (boolean for external communication)
bool lastBirdState = false;
const int birdTimeLimit = 1500;		// Calm time needed to log a bird event
volatile unsigned long timeCounter = 0;		// Counter for time having elapsed, stopping when there is no bird
volatile int radarTrigs = 0;

/*
* TODO:
* Make the bird boolean so that it sends a bird event on FALLING
*/

void setup() {
	Serial.begin(115200);			// Set up serial communication
	// pinMode(ledPin, OUTPUT);	// Set debug LED as an output
	attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);
}

void loop () {
	// On activity, set time to the current millis() value
	if (activity) {
		timeCounter = millis();
		activity = false;
	}

	bird = millis() - timeCounter < birdTimeLimit;

	// If less than birdTimeLimit milliseconds have passed since last radar activity
	// Set the bird true
	if (bird != lastBirdState) {
		if (radarTrigs > 2) {
			// if (debug) digitalWrite(ledPin, HIGH);	// Sets debug LED HIGH
			if (debug) Serial.println("BIRD!");		// Prints serial debug
		}
		if (!bird) radarTrigs = 0;
		lastBirdState = bird;
	}

}

void radarEvent() {
	// activity = true;
	timeCounter = millis();
	radarTrigs++;
}
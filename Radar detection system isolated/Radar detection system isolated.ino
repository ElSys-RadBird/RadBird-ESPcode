

const bool debug = true;	// Boolean for setting the program in its debug mode

int radarPin = 2;			// Pin connected to radar sensor
int ledPin = LED_BUILTIN;	// Debug LED
volatile bool activity;		// Is the radar reading anything?
bool bird = false;			// Is there a bird present? (boolean for external communication)
const int birdTimeLimit = 1000;		// Calm time needed to log a bird event
unsigned long time = 0;		// Counter for time having elapsed, stopping when there is no bird

/*
* TODO:
* Make the bird boolean so that it sends a bird event on FALLING
* Check potential for overflow error on the millis() I do not think this is an issue, considering the subtraction will underflow
*/

void setup() {
	Serial.begin(9600);			// Set up serial communication
	pinMode(ledPin, OUTPUT);	// Set debug LED as an output
	attachInterrupt(digitalPinToInterrupt(radarPin), radarEvent, RISING);
}

void loop () {
	// On activity, set time to the current millis() value
	if (activity) {
		time = millis();
		activity = false;
	}

	// If less than birdTimeLimit milliseconds have passed since last radar activity
	// Set the bird true
	if (millis() - time > birdTimeLimit) {
		bird = true;							// Sets bird true
		if (debug) digitalWrite(ledPin, HIGH);	// Sets debug LED HIGH
		if (debug) Serial.println("BIRD!");		// Prints serial debug
	}
	else {
		bird = false;							// Sets bird false
		if (debug) digitalWrite(ledPin, LOW);	// Sets debug LED LOW
		if (debug) Serial.println("Not bird");	// Prints serial debug
	}

}

void radarEvent() {
	activity = true;
}
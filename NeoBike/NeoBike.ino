#include <v4pins.h>
#include <Snooze.h>
#include <Wire.h>
#include <MAX17043.h>
#include "FastLED.h"

// Debounce times - SW4 is the rear wheel reed switch
#define SW1_DEBOUNCE_MILLIS 250
#define SW2_DEBOUNCE_MILLIS 250
#define SW3_DEBOUNCE_MILLIS 250
#define SW4_DEBOUNCE_MILLIS 100

#define SW1_DWELL_MILLIS 10
#define SW2_DWELL_MILLIS 10
#define SW3_DWELL_MILLIS 10
#define SW4_DWELL_MILLIS 10

#define SW1_HELD_MILLIS 700
#define SW2_HELD_MILLIS 700
#define SW3_HELD_MILLIS 700

// Snooze config
SnoozeBlock config;
volatile int who;

// Fuel gauge
MAX17043 fuelGauge(20, 22);
elapsedMillis fuelPrint;

// State counter
volatile uint8_t awakeCount;
volatile uint8_t v5Count;

// Switch variables
volatile unsigned long SW1intMillis, SW2intMillis, SW3intMillis, SW4intMillis;
volatile unsigned long SW1resetMillis, SW2resetMillis, SW3resetMillis, SW4resetMillis;
volatile int SW1state, SW2state, SW3state, SW4state;
int SW1pressType, SW2pressType, SW3pressType, SW4pressType;

// LED setup
CRGB bikeLeds[TOTAL_LED_NUM];
uint8_t brightness = 50;
IntervalTimer ledTimer;
volatile uint8_t ledFrame;
uint8_t ledState;
uint8_t voltageState;

void setup() {
  Serial1.begin(115200);
  Serial1.println("Starting...");

  fuelGauge.begin();

  // Enable 5V output and HCUR
  pinMode(V5ENB, OUTPUT);
  pinMode(HCUR, OUTPUT);

  // Debug LEDS
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWriteFast(LED1, HIGH);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  config.pinMode(SW1, INPUT, FALLING);
  config.pinMode(SW2, INPUT, FALLING);
  config.pinMode(SW3, INPUT, FALLING);
  config.pinMode(SW4, INPUT, FALLING);

  attachInterrupt(SW1, SW1ISR, FALLING);
  attachInterrupt(SW2, SW2ISR, FALLING);
  attachInterrupt(SW3, SW3ISR, FALLING);
  attachInterrupt(SW4, SW4ISR, FALLING);

  // Define LED groups
  FastLED.addLeds<WS2812B, NEO1_PIN, GRB>(bikeLeds, FRONT_LED_NUM);
  FastLED.addLeds<WS2812B, NEO2_PIN, GRB>(bikeLeds, FRONT_LED_NUM, CENTER_LED_NUM);  
  FastLED.addLeds<WS2812B, NEO3_PIN, GRB>(bikeLeds, FRONT_LED_NUM + CENTER_LED_NUM, REAR_LED_NUM);

  // LED timer
  ledTimer.begin(ledTimerFunc, 10000);
}

void ledTimerFunc() {
	ledFrame++;
}

void loop() {
  sleep();
  handleInputController();
  actionInputs();

  if (v5Count > 0) {
  	digitalWriteFast(V5ENB, HIGH);
  } else {
  	digitalWriteFast(V5ENB, LOW);
  }

  actionLeds();
  actionRainbow();

	if (ledFrame >= 1) {
		FastLED.show();
		ledFrame = 0;
	}

  if (fuelPrint > 10000) {
  	fuelPrint = 0;
  	printVoltage();
  }
}

uint8_t hue, voltageI;
unsigned long voltageMillis;
elapsedMillis voltageTimer;
uint8_t voltageInterval1 = 70;
uint8_t voltageInterval2 = 10;
void actionRainbow() {
	switch (voltageState) {
		case 0: // Idle
			rainbowI = FRONT_LED_NUM;
			hue = 0;
			break;
		case 1: awakeCount++; voltageState++; v5Count++; voltageTimer = 0; break;
		case 2: {// Fading up
			if (voltageTimer >= voltageInterval1) {
				voltageTimer -= voltageInterval1;
				uint8_t percent = fuelGauge.getBatteryPercentage();

				bikeLeds[voltageI] = CHSV(hue, 255, 255);
				hue += 4;
				rainbowI++;

				if (hue >= percent) {
					voltageMillis = millis();
					voltageState++;
					voltageTimer = 0;
				}
			}
		}
			break;
		case 3: // Flash last LED
			if (voltageTimer >= 250) {
				voltageTimer -= 250;
				if (!bikeLeds[voltageI]) {
					bikeLeds[voltageI] = CHSV(hue, 255, 255);
				} else {
					bikeLeds[voltageI] = CHSV(0, 0, 0);
				}
			}
			if ((millis() - voltageMillis) > 2750) {
				voltageTimer = 0;
				voltageState++;
			}
			break;
		case 4: // Fade down
			if (voltageTimer >= voltageInterval2) {
				voltageTimer -= voltageInterval2;
				for (uint8_t i = FRONT_LED_NUM; i <= FRONT_LED_NUM + 25; i++) {
					bikeLeds[i].fadeToBlackBy(10);
				}
				if (!bikeLeds[voltageI]) {
					voltageState = 0;
					awakeCount--;
					v5Count--;
				}
			}
			break;
	}
}

void actionLeds() {
	switch (ledState) {
		case 0:
			break;
		case 1:
			awakeCount++;
			ledState++;
			v5Count++;
			break;
		case 2:
			halfOn();
			break;
		case 3:
			fullOn();
			break;
		case 4:
			fadeOff();
			// Probably needs work, waits until first LED is black
			// then turns all LEDs off
			if (!bikeLeds[0]) {
				allOff();
				ledState = 0;
				awakeCount--;
				v5Count--;
			}
			break;
	}
}

void actionInputs() {
	// Switch input 1
	if (SW1pressType == 1) { // Basic press
		SW1pressType = 0;
		Serial1.print(millis());
		Serial1.println(" SW1 pressed");
		ledState++;
	} else if (SW1pressType == 2) { // Held
		SW1pressType = 0;
		Serial1.println("SW1 held");
		voltageState = 1;
	}

	// Switch input 2
	if (SW2pressType == 1) { // Basic press
		SW2pressType = 0;
		Serial1.print(millis());
		Serial1.println(" SW2 pressed");
	} else if (SW2pressType == 2) { // Held
		SW2pressType = 0;
		Serial1.println("SW2 held");
	}

	// Switch input 3
	if (SW3pressType == 1) { // Basic press
		SW3pressType = 0;
		Serial1.print(millis());
		Serial1.println(" SW3 pressed");
	} else if (SW3pressType == 2) { // Held
		SW3pressType = 0;
		Serial1.println("SW4 held");
	}

	// Switch input 4 - reed switch
	if (SW4pressType == 1) { // Basic press
		SW4pressType = 0;
		Serial1.print(millis());
		Serial1.println(" SW4 pressed");
		digitalWriteFast(LED2, !digitalRead(LED2));
	} // There is no hold event for this switch
}

void sleep() {
	if (awakeCount == 0) {
		//Serial1.println("-");
		Serial1.flush();

		// Detach all of the interrupts
		detachInterrupt(SW1);
		detachInterrupt(SW2);
		detachInterrupt(SW3);
		detachInterrupt(SW4);

		digitalWriteFast(LED1, LOW);
		who = Snooze.deepSleep(config);
		digitalWriteFast(LED1, HIGH);

		// Attach the interrupts again
		attachInterrupt(SW1, SW1ISR, FALLING);
		attachInterrupt(SW2, SW2ISR, FALLING);
		attachInterrupt(SW3, SW3ISR, FALLING);
		attachInterrupt(SW4, SW4ISR, FALLING);

		// Determine what woke us up
		determineWakeup();
		
		//Serial1.print(millis());
		//Serial1.println(" +");
	}
}

void determineWakeup() {
	switch (who) {
		case SW1: {
			SW1ISR();
			//Serial1.print(millis());
			//Serial1.println(" SW1 wake");
			break;	
		}
		case SW2: {
			SW2ISR();
			//Serial1.print(millis());
			//Serial1.println(" SW2 wake");
			break;
		}
		case SW3: {
			SW3ISR();
			//Serial1.print(millis());
			//Serial1.println(" SW3 wake");
			break;
		}
		case SW4: {
			SW4ISR();
			//Serial1.print(millis());
			//Serial1.println(" SW4 wake");
			break;
		}
	}
}

void SW1ISR() {
	SW1state = 1;
	SW1intMillis = millis();
	detachInterrupt(SW1);
}

void SW2ISR() {
	SW2state = 1;
	SW2intMillis = millis();
	detachInterrupt(SW2);
}

void SW3ISR() {
	SW3state = 1;
	SW3intMillis = millis();
	detachInterrupt(SW3);
}

void SW4ISR() {
	SW4state = 1;
	SW4intMillis = millis();
	detachInterrupt(SW4);
}

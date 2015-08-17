void fadeOff() {
	for (uint8_t i = 0; i <= TOTAL_LED_NUM; i++) {
		bikeLeds[i].fadeToBlackBy(1);
	}
	analogWrite(HCUR, bikeLeds[0].red);
	FastLED.show();
}

void allOff() {
	// Turn all LEDs off, then disable 5V and HCUR output
	for (uint8_t i = 0; i <= TOTAL_LED_NUM; i++) {
		bikeLeds[i] = CHSV(0, 0, 0);
	}
	FastLED.show();
	digitalWriteFast(HCUR, LOW);
}

void halfOn() {
	for (uint8_t i = 0; i < FRONT_LED_NUM; i++) {
		bikeLeds[i] = CHSV(0, 0, random(10, 50));
	}
	for (uint8_t i = FRONT_LED_NUM + CENTER_LED_NUM; i <= TOTAL_LED_NUM; i++) {
		bikeLeds[i] = CHSV(0, 255, random(10, 50));
	}
}

void fullOn() {
	// Flicker LEDS and enable HCUR output
	for (uint8_t i = 0; i < FRONT_LED_NUM; i++) {
		bikeLeds[i] = CHSV(0, 0, random(80, 100));
	}
	for (uint8_t i = FRONT_LED_NUM + CENTER_LED_NUM; i <= TOTAL_LED_NUM; i++) {
		bikeLeds[i] = CHSV(0, 255, random(80, 100));
	}
	digitalWriteFast(HCUR, HIGH);
}
void handleInputController() {
  handleSW1();
  handleSW2();
  handleSW3();
  handleSW4();
}

void handleSW1() {
  switch (SW1state) {
    case 0: break; // Idle
    case 1: { SW1state = 2; awakeCount++; } break;
    case 2: {
      if (digitalRead(SW1) == HIGH && (millis() - SW1intMillis) > SW1_DWELL_MILLIS) {
        SW1pressType = 1;
        // Go to wait for high and reset
        SW1state = 3;
      } else if (digitalRead(SW1) == LOW && (millis() - SW1intMillis) > SW1_HELD_MILLIS) {
        SW1pressType = 2;
        // Go to wait for high and reset
        SW1state = 3;
      }
    }
    break;
    case 3:
      if (digitalRead(SW1) == HIGH) {
        SW1resetMillis = millis();
        SW1state = 4;
      }
      break;
    case 4:
      if (millis() > (SW1resetMillis + SW1_DEBOUNCE_MILLIS)) {
        attachInterrupt(SW1, SW1ISR, FALLING);
        awakeCount--;
        SW1intMillis = 0;
        SW1resetMillis = 0;
        SW1state = 0;
      }
      break;
  }
}

void handleSW2() {
  switch (SW2state) {
    case 0: break; // Idle
    case 1: { SW2state = 2; awakeCount++; } break;
    case 2: {
      if (digitalRead(SW2) == HIGH && (millis() - SW2intMillis) > SW2_DWELL_MILLIS) {
        SW2pressType = 1;
        // Go to wait for high and reset
        SW2state = 3;
      } else if (digitalRead(SW2) == LOW && (millis() - SW2intMillis) > SW2_HELD_MILLIS) {
        SW2pressType = 2;
        // Go to wait for high and reset
        SW2state = 3;
      }
    }
    break;
    case 3:
      if (digitalRead(SW2) == HIGH) {
        SW2resetMillis = millis();
        SW2state = 4;
      }
      break;
    case 4:
      if (millis() > (SW2resetMillis + SW2_DEBOUNCE_MILLIS)) {
        attachInterrupt(SW2, SW2ISR, FALLING);
        awakeCount--;
        SW2intMillis = 0;
        SW2resetMillis = 0;
        SW2state = 0;
      }
      break;
  }
}

void handleSW3() {
  switch (SW3state) {
    case 0: break; // Idle
    case 1: { SW3state = 2; awakeCount++; } break;
    case 2: {
      if (digitalRead(SW3) == HIGH && (millis() - SW3intMillis) > SW3_DWELL_MILLIS) {
        SW3pressType = 1;
        // Go to wait for high and reset
        SW3state = 3;
      } else if (digitalRead(SW3) == LOW && (millis() - SW3intMillis) > SW3_HELD_MILLIS) {
        SW3pressType = 2;
        // Go to wait for high and reset
        SW3state = 3;
      }
    }
    break;
    case 3:
      if (digitalRead(SW3) == HIGH) {
        SW3resetMillis = millis();
        SW3state = 4;
      }
      break;
    case 4:
      if (millis() > (SW3resetMillis + SW3_DEBOUNCE_MILLIS)) {
        attachInterrupt(SW3, SW3ISR, FALLING);
        awakeCount--;
        SW3intMillis = 0;
        SW3resetMillis = 0;
        SW3state = 0;
      }
      break;
  }
}

void handleSW4() {
  switch (SW4state) {
    case 0: break; // Idle
    case 1: { SW4state = 2; awakeCount++; } break;
    case 2: {
      if (digitalRead(SW4) == HIGH && (millis() - SW4intMillis) > SW4_DWELL_MILLIS) {
        SW4pressType = 1;
        // Go to wait for high and reset
        SW4state = 3;
      }
    }
    break;
    case 3:
      if (digitalRead(SW4) == HIGH) {
        SW4resetMillis = millis();
        SW4state = 4;
      }
      break;
    case 4:
      if (millis() > (SW4resetMillis + SW4_DEBOUNCE_MILLIS)) {
        attachInterrupt(SW4, SW4ISR, FALLING);
        awakeCount--;
        SW4intMillis = 0;
        SW4resetMillis = 0;
        SW4state = 0;
      }
      break;
  }
}

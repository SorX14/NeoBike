#ifndef neobikepins_h
#define neobikepins_h

// Teensy pins
// BTLE
#define NRF_RQN 17 		// 8
#define NRF_RDY 21		// 2
#define NRF_RST 15		// 9
#define NRF_ACT 10		// A3

// Accelerometer and voltage
#define ACC_ALRT 16		// 16
#define VCC_ALRT 22		// 5

// NeoPixel outputs and high current output
#define NEO1_PIN 9		// 5
#define NEO2_PIN 8		// 8
#define NEO3_PIN 5		// 9
#define HCUR 3			// 3

// Debug LEDs
#define LED1 23			// 17
#define LED2 20			// 15
#define LED3 0			// 13

// Enable 5V output
#define V5ENB 14		// 14

// Input switches
#define SW1 4
#define SW2 6
#define SW3 7
#define SW4 2

#define FRONT_LED_NUM 50
#define CENTER_LED_NUM 28
#define REAR_LED_NUM 60

#define FRONT_AVAILABLE_LED_NUM FRONT_LED_NUM / 2
#define CENTER_AVAILABLE_LED_NUM CENTER_LED_NUM
#define REAR_AVAILABLE_LED_NUM REAR_LED_NUM / 2

#define TOTAL_LED_NUM FRONT_LED_NUM + CENTER_LED_NUM + REAR_LED_NUM
#define TOTAL_AVAILABLE_LED_NUM (FRONT_LED_NUM / 2) + CENTER_LED_NUM + (REAR_LED_NUM / 2)

#endif
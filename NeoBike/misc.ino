// Basic debugging; outputing the current voltage
void printVoltage() {
  Serial1.print("Voltage: ");
  Serial1.print(fuelGauge.getBatteryVoltage());
  Serial1.print(" (");
  Serial1.print(fuelGauge.getBatteryPercentage());
  Serial1.println(")");
}

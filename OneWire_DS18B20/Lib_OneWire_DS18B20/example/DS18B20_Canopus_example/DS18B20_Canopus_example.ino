/*
 * Example sketch to show using configuration commands on the DS18B20.
 * On Canopus_RAK3172, connect the DS18B20 to GPIO pins PA11 : I2C_SDA
*/
#include <OneWire_DS18B20.h>

void setup() {
  Serial.begin(115200);
  Serial.println("RAK3172_Canopus -  OneWire Protocol");
  Serial.println("------------------------------------------------------");
  Serial.println("This library support DS18B20");
  Serial.println("------------------------------------------------------");
  init_io(); //Enable I/O
  enable_Vss3();
  startTime = millis();
}
void loop() {
  float temperature = read_temperature();
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);  
  Serial.println(" *C");
  delay(100); 
}

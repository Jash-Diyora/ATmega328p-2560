#include<MPU6050.h>
MPU6050 mpu;

void setup() 
{
  mpu.setZGyroOffset(19);
  Serial.begin(9600);
}
float Gz; 
double current_time = 0, loop_time = 0, previous_time = 0, G_rate,AG;
void loop() 
{
  mpu.initialize();
 Gz=mpu.getRotationZ();
 current_time = millis();
 loop_time = current_time - previous_time;
 previous_time = current_time;
 G_rate = map(Gz, -32760, 32780, -70, 70);
 AG += (G_rate*loop_time)/25;
 Serial.print(millis());
 Serial.print("\t \t");
Serial.println(G_rate);
}

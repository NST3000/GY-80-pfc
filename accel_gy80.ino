// Programa : Teste acelerometro ADXL345
// Adaptacoes : Arduino e Cia
// Read the 3Axis Inertial sensors from the GY-80 using I2C


#include <Wire.h>

#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37

// Endereco I2C do sensor : 83 em decimal ou 0x53
int ADXAddress = 0x53;  // the default 7-bit slave address
int reading = 0;
int val=0;
int x_acc,x1_acc,x_acc_out;
int y_acc,y1_acc,y_acc_out;
int z1_acc,z_acc,z_acc_out;
float x_acc_final,y_acc_final,z_acc_final;
float pitch_acc,roll_acc;

void setup()
{
  Wire.begin();                
  Serial.begin(9600);    
  delay(100);
  // enable to measute g data
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting
   pinMode(53, OUTPUT);
  
}

void loop()
{
  //digitalWrite(53, LOW);   
  
  //--------------X
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    x_acc = Wire.read();
    x1_acc = Wire.read(); 
    x1_acc=x1_acc<<8;
    x_acc_out=x_acc+x1_acc;   
  }

  //------------------Y
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    y_acc = Wire.read();
    y1_acc = Wire.read(); 
    y1_acc=y1_acc<<8;
    y_acc_out=y_acc+y1_acc;
  }
  //------------------Z
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    z_acc = Wire.read();
    z1_acc = Wire.read(); 
    z1_acc=z1_acc<<8;
    z_acc_out=z_acc+z1_acc;
  }
  //
  x_acc_final=x_acc_out/256.0;
  y_acc_final=y_acc_out/256.0;
  z_acc_final=z_acc_out/256.0;
// Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  pitch_acc = -atan(y_acc_final / sqrt(pow(x_acc_final, 2) + pow(z_acc_final, 2))) * 180 / PI;
  roll_acc = atan(-1 * x_acc_final / sqrt(pow(y_acc_final, 2) + pow(z_acc_final, 2))) * 180 / PI;

 
  Serial.print("X= ");
  Serial.print(x_acc_final);
  Serial.print("       ");
  Serial.print("Y= ");
  Serial.print(y_acc_final);
  Serial.print("       ");
  Serial.print("Z= ");
  Serial.print(z_acc_final);
  Serial.print("  ");
  Serial.print("Roll&Pitch= ");
  Serial.print("/");
  Serial.print(roll_acc);
  //Serial.print("       ");
  //Serial.print("Pitch= ");
  Serial.print("/");
  Serial.println(pitch_acc);
   digitalWrite(53, LOW);
  if (roll_acc>45 || -45>roll_acc || pitch_acc>45 || -45> pitch_acc){
  digitalWrite(53, HIGH);   
    }
  delay(200);
}

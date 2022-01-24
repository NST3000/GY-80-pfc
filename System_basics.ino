#include <Wire.h>

//Declaro acelerómetro
#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37

// Declaro gyroscopio
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24


//Variables acelerómetro
int ADXAddress = 0x53;  // the default 7-bit slave address
int reading = 0;
int val=0;
int x_acc,x1_acc,x_acc_out;
int y_acc,y1_acc,y_acc_out;
int z1_acc,z_acc,z_acc_out;
float x_acc_final,y_acc_final,z_acc_final;
float pitch_acc,roll_acc;

// variables giroscopio
int L3G4200D_Address = 105;

int x_gyro;
int y_gyro;
int z_gyro;
float x1_gyro,y1_gyro,z1_gyro;
float roll_gyro, pitch_gyro;
float roll, pitch;
void setup()
{
  Wire.begin();                
  Serial.begin(9600);    
  

  
  // enable to measute g data
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting

  setupL3G4200D(2000); 

  
   pinMode(53, OUTPUT); //------------------------ Pin del motor como output
   delay(100);
}

void loop()
{
 
  //----------------------------------------------------------X accel
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

  //----------------------------------------------------------Y accel
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
  //----------------------------------------------------------Z accel
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

  //digitalWrite(53, LOW);   
   getGyroValues();  
   
   x1_gyro=x_gyro+9,328;
   y1_gyro=y_gyro-25,171;
   z1_gyro=z_gyro-6,421;
  
 
  roll_gyro+=(-y1_gyro/100);
  pitch_gyro+=(x1_gyro/100);
  
  //delay(200);
  pitch= pitch_gyro*0.95 +pitch_acc*0.05;
  roll= roll_gyro*0.95 + roll_acc*0.05;
  //imprimo valores

 //PRINT ACCEL
 /*
  Serial.print("Xaccel= ");
  Serial.print(-x_acc_final);
  Serial.print("       ");
  Serial.print("Yaccel= ");
  Serial.print(-y_acc_final);
  Serial.print("       ");
  Serial.print("Zaccel= ");
  Serial.print(-z_acc_final);
  Serial.print("  ");

  Serial.print("Roll_accel & Pitch_accel= ");
  Serial.print("/");
  Serial.print(roll_acc);
  Serial.print("/");
  
  Serial.println(pitch_acc);
*/
  //PRINT GYRO
 /* 
  Serial.print("X_gyro:");
  Serial.print(-y1_gyro);

  Serial.print(" Y_gyro:");
  Serial.print(x1_gyro);

  Serial.print(" Z_gyro:");
  Serial.print(z1_gyro);
Serial.print("     ");
  Serial.print("Roll_gyro & Pitch_gyro= ");
  Serial.print("/");
  Serial.print(roll_gyro);
  Serial.print("/");
  Serial.println(pitch_gyro);  

*/
 // FILTRO COMPLEMENTARIO

  Serial.print("Roll & Pitch= ");
  Serial.print("/");
  Serial.print(roll);
  Serial.print("/");
  Serial.println(pitch);  
 
   //digitalWrite(53, LOW);
  //if (roll>45 || -45>roll || pitch>45 || -45> pitch){
  //digitalWrite(53, HIGH);   //---------------------------------------------------------- activo motor
  //  }
  delay(100);
}
void getGyroValues()
{
  // Rotina para leitura dos valores de X, Y e Z
  byte xMSB = readRegister(L3G4200D_Address, 0x29);
  byte xLSB = readRegister(L3G4200D_Address, 0x28);
  x_gyro = ((xMSB << 8) | xLSB);

  byte yMSB = readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = readRegister(L3G4200D_Address, 0x2A);
  y_gyro = ((yMSB << 8) | yLSB);

  byte zMSB = readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = readRegister(L3G4200D_Address, 0x2C);
  z_gyro = ((zMSB << 8) | zLSB);
}

int setupL3G4200D(int scale)
{
  //From  Jim Lindblom of Sparkfun's code

  // Enable x, y, z and turn off power down:
  writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

  // CTRL_REG4 controls the full-scale range, among other things:
  if(scale == 250){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
  }else{
    writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
  }

  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) 
{
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address)
{
    int v;
    Wire.beginTransmission(deviceAddress);
    Wire.write(address); // register to read
    Wire.endTransmission();

    Wire.requestFrom(deviceAddress, 1); // read a byte

    while(!Wire.available()) 
    {
        // waiting
    }
    v = Wire.read();
    return v;
}

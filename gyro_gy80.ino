// Programa : Teste Giroscopio L3G4200D
// Adaptacoes : Arduino e Cia
// 
// Read the Gyro3 Axes of the GY-80 sensor using the Wire Library (I2C)


#include <Wire.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

//Endereco I2C do L3G4200D
int L3G4200D_Address = 105;


int x_gyro;
int y_gyro;
int z_gyro;
float x1_gyro,y1_gyro,z1_gyro;
float roll_gyro, pitch_gyro;

void setup()
{
  Wire.begin();
  Serial.begin(9600);

  //Serial.println("Inicializando o L3G4200D");
  Serial.println("x,y,z");
  // Configura o L3G4200 para 200, 500 ou 2000 graus/seg
  setupL3G4200D(2000); 

  // Aguarda a resposta do sensor
  delay(1500); 
}

void loop()
{
  // Atualiza os valores de X, Y e Z
  getGyroValues();  

  x1_gyro=x_gyro+9,328;
  y1_gyro=y_gyro-25,171;
  z1_gyro=z_gyro-6,421;
  
  

  roll_gyro+=(-y1_gyro/100);
  pitch_gyro+=(x1_gyro/100);
  
  // Mostra os valores no serial monitor
  
  Serial.print("X_gyro:");
  Serial.print(-y1_gyro);

  Serial.print(" Y_gyro:");
  Serial.print(x1_gyro);

  Serial.print(" Z_gyro:");
  Serial.print(z1_gyro);
  Serial.print("    ");
  Serial.print("Roll_gyro & Pitch_gyro= ");
  Serial.print("/");
  Serial.print(roll_gyro);
  Serial.print("/");
  Serial.println(pitch_gyro);  
  // Aguarda 100ms e reinicia o processo
  delay(150);
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

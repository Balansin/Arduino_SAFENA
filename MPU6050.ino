//------------ I2C ---------------------------------------------------------
#include <Wire.h>

//------------ MPU6050 Measurement & Filtering Range ----------------------
#define AFS_SEL 2  // Accelerometer Configuration Settings  AFS_SEL=2, Full Scale Range
#define DLPF_SEL 0 // DLPF Configuration Settings Accel BW 260Hz, Delay 0ms / Gyro Enabled

//------------ Variables for gravity ---------------------------------------
const int MPU_ADDR = 0x68;  // I2C address of the MPU-6050
int AcX, AcY, AcZ;          // Accelerometer values
long Cal_AcX, Cal_AcY, Cal_AcZ;  // Calibration values

float GAcX, GAcY, GAcZ;  // Convert accelerometer to gravity value
float Min_GAcX = 0, Max_GAcX, Min_GAcY = 0, Max_GAcY = 0, Min_GAcZ = 0, Max_GAcZ = 0;
float Min = 0, Max = 0;  // Initial value of Min, Max
int cnt;  // Count of calibration process
float Grvt_unit;  // Gravity value unit
long period, prev_time;  // Period of calculation

void setup() {
    Wire.begin();
    init_MPU6050();
    Serial.begin(115200);
    Gravity_Range_Option();
    Calib_MPU6050();  // Calculating calibration value
}

void loop() {
    ReadDate_MPU6050();
    Calc_Grvt();
    Display_Grvt();
}

void init_MPU6050(){
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0x03);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1C);
    if(AFS_SEL == 0) Wire.write(0x00);
    else if(AFS_SEL == 1) Wire.write(0x08);
    else if(AFS_SEL == 2) Wire.write(0x10);
    else Wire.write(0x18);
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x1A);
    if(DLPF_SEL == 0) Wire.write(0x00);
    else if(DLPF_SEL == 1) Wire.write(0x01);
    else if(DLPF_SEL == 2) Wire.write(0x02);
    else if(DLPF_SEL == 3) Wire.write(0x03);
    else if(DLPF_SEL == 4) Wire.write(0x04);
    else if(DLPF_SEL == 5) Wire.write(0x05);
    else Wire.write(0x06);
    Wire.endTransmission(true);
}

void Gravity_Range_Option(){
    switch(AFS_SEL) {
        case 0: Grvt_unit = 16384; break;
        case 1: Grvt_unit = 8192; break;
        case 2: Grvt_unit = 4096; break;
        case 3: Grvt_unit = 3276.8; break;
    }
}

void Calib_MPU6050() {
    for(int i = 0; i < 2000; i++) {
        ReadDate_MPU6050();
        delay(10);
        Cal_AcX += AcX;
        Cal_AcY += AcY;
        Cal_AcZ += AcZ;
    }
    Cal_AcX /= 2000;
    Cal_AcY /= 2000;
    Cal_AcZ /= 2000;

    Serial.println("End of Calculation");
    Serial.print("Cal_AcX = "); Serial.println(Cal_AcX);
    Serial.print(" | Cal_AcY = "); Serial.println(Cal_AcY);
    Serial.print(" | Cal_AcZ = "); Serial.println(Cal_AcZ);
}

void ReadDate_MPU6050() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);

    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
    AcZ = Wire.read() << 8 | Wire.read();
}

void Calc_Grvt() {
    AcX = (AcX - Cal_AcX);
    AcY = (AcY - Cal_AcY);
    AcZ = (AcZ - Cal_AcZ);

    GAcX = AcX / Grvt_unit;
    GAcY = AcY / Grvt_unit;
    GAcZ = AcZ / Grvt_unit;

    Min_GAcX = min(Min_GAcX, GAcX);
    Max_GAcX = max(Max_GAcX, GAcX);
    PtoP_GAcX = Max_GAcX - Min_GAcX;

    Min_GAcY = min(Min_GAcY, GAcY);
    Max_GAcY = max(Max_GAcY, GAcY);
    PtoP_GAcY = Max_GAcY - Min_GAcY;

    Min_GAcZ = min(Min_GAcZ, GAcZ);
    Max_GAcZ = max(Max_GAcZ, GAcZ);
    PtoP_GAcZ = Max_GAcZ - Min_GAcZ;
}

void Display_Grvt() {
    Serial.print("AcX= " + String(AcX));
    Serial.print(" | AcY= " + String(AcY));
    Serial.println(" | AcZ= " + String(AcZ));

    period = millis() - prev_time;
    if(period > 1000) {
        prev_time = millis();
        Serial.print("PeakToPeak X|Y|Z: ");
        Serial.print(PtoP_GAcX);
        Serial.print(" | ");
        Serial.print(PtoP_GAcY);
        Serial.print(" | ");
        Serial.println(PtoP_GAcZ);

        Min_GAcX = 0;
        Max_GAcX = 0;
        Min_GAcY = 0;
        Max_GAcY = 0;
        Min_GAcZ = 0;
        Max_GAcZ = 0;
    }
}

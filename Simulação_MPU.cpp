#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

#ifndef NOMINMAX
    #define NOMINMAX  // Evita conflitos no Windows
#endif

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

float custom_min(float a, float b) { return (a < b) ? a : b; }
float custom_max(float a, float b) { return (a > b) ? a : b; }

// Simulated MPU6050 variables
int AcX, AcY, AcZ;
long Cal_AcX = 0, Cal_AcY = 0, Cal_AcZ = 0;
float GAcX, GAcY, GAcZ;
float Min_GAcX = 0, Max_GAcX = 0, Min_GAcY = 0, Max_GAcY = 0, Min_GAcZ = 0, Max_GAcZ = 0;
float PtoP_GAcX, PtoP_GAcY, PtoP_GAcZ;
float Grvt_unit = 4096;

void ReadDate_MPU6050() {
    AcX = rand() % 32768 - 16384;
    AcY = rand() % 32768 - 16384;
    AcZ = rand() % 32768 - 16384;
}

void Calc_Grvt() {
    AcX -= Cal_AcX;
    AcY -= Cal_AcY;
    AcZ -= Cal_AcZ;

    GAcX = AcX / Grvt_unit;
    GAcY = AcY / Grvt_unit;
    GAcZ = AcZ / Grvt_unit;

    Min_GAcX = custom_min(Min_GAcX, GAcX);
    Max_GAcX = custom_max(Max_GAcX, GAcX);
    PtoP_GAcX = Max_GAcX - Min_GAcX;

    Min_GAcY = custom_min(Min_GAcY, GAcY);
    Max_GAcY = custom_max(Max_GAcY, GAcY);
    PtoP_GAcY = Max_GAcY - Min_GAcY;

    Min_GAcZ = custom_min(Min_GAcZ, GAcZ);
    Max_GAcZ = custom_max(Max_GAcZ, GAcZ);
    PtoP_GAcZ = Max_GAcZ - Min_GAcZ;
}

void Display_Grvt() {
    std::cout << "AcX= " << AcX << " | AcY= " << AcY << " | AcZ= " << AcZ << std::endl;
    std::cout << "PeakToPeak X|Y|Z: " << PtoP_GAcX << " | " << PtoP_GAcY << " | " << PtoP_GAcZ << std::endl;
}

int main() {
    srand(time(0));
    while (true) {
        ReadDate_MPU6050();
        Calc_Grvt();
        Display_Grvt();
        SLEEP(5000);  // Espera 1 segundo entre as medições
    }
    return 0;
}

#include <iostream>
#include <wiringPiSPI.h>
#include <wiringPi.h>

using namespace std;

int main()
{
    int s;
    s=wiringPiSPISetup(0,50000);
    if(s<0)
    {
        return 1;
    }
    unsigned char i=0x43;
    wiringPiSPIDataRW(0,&i,1);
    return 0;
}


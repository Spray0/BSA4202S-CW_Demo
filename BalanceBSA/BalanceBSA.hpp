/*  
    Sartorius BSA Balances driver code
    Debug device: Sartorius BSA4202S-CW
    Refernece: Description of the Interface for Entris, ED, GK and GW Balances/Scales.pdf
    
    Spray0 
    Copyright © 2021 Spray0,All Rights Reserved.
    <javen.chan@outlook.com>
    2021-2 Ver1.0 Changzhou.China
*/
#ifndef BALANCE_BSA_H
#define BALANCE_BSA_H
#include <iostream>
#include <cstdlib>
#include <string>
#include "./serialport/serialport.h"


class BALANCE_BSA{
private:
    pthread_t listen_thread;
    pthread_t timeout_thread;
public:
    unsigned char data_buf[22]{0};
    unsigned char data_cnt=0;
    unsigned int time_cnt=0;
    std::string INFO_Model{"BSA4202S-CW"};
    SerialPort UART;
    bool ONLINE {false};
    float VAL   {0.0f};
    int Init(void);
    int GetInfo(void);
    void Do_InternalCalibration(void);
    void Do_Tare(void);
    void Do_BlockKeys(void);
    void Do_UnBlockKeys(void);
    void Do_Restart(void);
};

#endif
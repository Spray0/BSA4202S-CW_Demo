/*  
    Sartorius BSA Balances driver code
    Debug device: Sartorius BSA4202S-CW
    Refernece: Description of the Interface for Entris, ED, GK and GW Balances/Scales.pdf
    
    Spray0 
    Copyright © 2021 Spray0,All Rights Reserved.
    <javen.chan@outlook.com>
    2021-2 Ver1.0 Changzhou.China
*/
#include "BalanceBSA.hpp"
void* Timeout(void *arg){
    BALANCE_BSA *BSA= (BALANCE_BSA*) arg;
    while(true){
        sleep(1);
        if(BSA->time_cnt<999)BSA->time_cnt++;
        if( BSA->time_cnt==3)BSA->ONLINE=false;
    }
}
void* Listen(void *arg) {
	int get;
	BALANCE_BSA *BSA= (BALANCE_BSA*) arg;
	std::vector<unsigned char> RX_buf(128);
	while (1) {
		get = read(BSA->UART.fd, &RX_buf[0], 128);
		if (get > 0) {
			std::vector<unsigned char> RX_data;
			for (int c = 0; c < get; ++c)
				RX_data.push_back(RX_buf[c]);
                for (auto c : RX_data)
                {
                    //byte process here
                    if((BSA->data_cnt==0)&&(c=='N')){
                        BSA->data_buf[BSA->data_cnt]=c;
                        BSA->data_cnt++;
                    }else if (BSA->data_cnt>0){
                        BSA->data_buf[BSA->data_cnt]=c;
                        BSA->data_cnt++;
                        if(BSA->data_cnt==22){
                            BSA->data_cnt=0;
                            std::string output;
                            for(unsigned char cnt=0;cnt<10;cnt++) output.push_back(BSA->data_buf[7+cnt]);
                            BSA->VAL=atof(output.c_str());
                            if(BSA->data_buf[6]=='-')BSA->VAL=-BSA->VAL;
                            BSA->time_cnt=0;
                            BSA->ONLINE=true;
                        }
                    }
                }
			RX_data.clear();
		}
	}
	return NULL;
}
int BALANCE_BSA::Init(void){

    this->UART.Config.BaudRate=SerialPort::BR1200;
    this->UART.Config.DataBits=SerialPort::DataBits7;
    this->UART.Config.StopBits=SerialPort::StopBits1;
    this->UART.Config.Parity=SerialPort::ParityOdd;
    this->UART.Config.DevicePath=(char*)&"/dev/ttyUSB0";

    if(this->UART.Open()==false)return 1;
    if(this->UART.LoadConfig()==false)return 1;

	pthread_create(&this->listen_thread, NULL, Listen, this);
	pthread_detach(this->listen_thread);
    pthread_create(&this->timeout_thread, NULL, Timeout, this);
	pthread_detach(this->timeout_thread);

    return 0;
}

void BALANCE_BSA::Do_InternalCalibration(void){
    this->UART<<(std::vector<unsigned char>){0x1b,'Z',0x0d,0x0a};
}
void BALANCE_BSA::Do_Tare(void){
    this->UART<<(std::vector<unsigned char>){0x1b,'T',0x0d,0x0a};
}
void BALANCE_BSA::Do_BlockKeys(void){
    this->UART<<(std::vector<unsigned char>){0x1b,'O',0x0d,0x0a};
}
void BALANCE_BSA::Do_UnBlockKeys(void){
    this->UART<<(std::vector<unsigned char>){0x1b,'R',0x0d,0x0a};
}
void BALANCE_BSA::Do_Restart(void){
    this->UART<<(std::vector<unsigned char>){0x1b,'S',0x0d,0x0a};
}

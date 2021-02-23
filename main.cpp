/*  
    Sartorius BSA Balances driver code
    Debug device: Sartorius BSA4202S-CW
    Refernece: Description of the Interface for Entris, ED, GK and GW Balances/Scales.pdf
    
    Spray0 
    Copyright © 2021 Spray0,All Rights Reserved.
    <javen.chan@outlook.com>
    2021-2 Ver1.0 Changzhou.China
*/

#include "./BalanceBSA/BalanceBSA.hpp"
BALANCE_BSA balance;


int main(int argc, char **argv) {


    printf("Sartorius BSA Balances Driver Demo.\r\n");
    printf("Device: BSA4202S-CW\r\n\r\n");

    if(balance.Init()==1){
        printf("ERR\r\n");
    }

    //balance.Do_Restart();

    while(!balance.ONLINE);
    while(balance.ONLINE){
        sleep(1);
        printf("%.2f\r\n",balance.VAL);
    }
    printf("offline!!!\r\n");
    return 0;
}
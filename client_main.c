//********************** Socket communication - Client ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : main.c
// Summary  : Socket commuication client side
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 13/09/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "types.h"
#include "client.h"

//***************************** Local Constants *******************************
#define READ_BUF_LEN        (200)
#define SEND_MES_TIME_DIF   (5)

//****************************** Local Functions ******************************
static bool exceedTime(uint32 lTimeDif, time_t* lPrevTime);

//************************************ main ***********************************
//Purpose : Socket communication client side
//Inputs  : Nil
//Outputs : Nil
//Return  : Nil
//Notes   : Client send a message to server every 5 seconds.
//*****************************************************************************
void main()
{
    bool blStatus = false;
    bool blReadStatus = false;
    uint8 pucRecieveBuffer[READ_BUF_LEN] = {0};
    time_t lPrevTime = 0;
    bool blSendState = false;
    bool blMesState = false;

    blStatus = clientStartCon();
    
    if (blStatus == true)
    {
        while (1)
        {
            // Read message from the server
            blReadStatus = clientReadMessage(pucRecieveBuffer);

            if (blReadStatus == true)
            {
                printf("Message from server: %s\n", pucRecieveBuffer);
            }

            blSendState = exceedTime(SEND_MES_TIME_DIF, &lPrevTime);

            if (blSendState == true)
            {
                uint8* message = "Hi!!";
                
                // Send a message to server in every 5 seconds
                blMesState = clientSendMessage(message);
            }
        }
    }
}

//********************************* exceedTime ********************************
//Purpose : Checks the current time exceeds the previous time and by a 
//          specified time difference and updates the previous time 
//          accordingly.
//Inputs  : lTimeDif
//Outputs : lPrevTime
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
static bool exceedTime(uint32 lTimeDif, time_t* lPrevTime)
{
    bool blStatus = false;
    time_t CurTime = 0;

    time(&CurTime);

    if ((CurTime - *lPrevTime) > lTimeDif)
    {
        printf("Current time : %ld\n", CurTime);
        blStatus = true;
        *lPrevTime = CurTime;
    }

    return blStatus;
}

// EOF

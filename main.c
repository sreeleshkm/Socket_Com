//********************** Socket communication ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : main.c
// Summary  : Main file of the server communication
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 20/09/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <stdbool.h>
#include "types.h"

#if (SOCKET_COM == SOC_CLI)
#include <time.h>
#include "client.h"
#elif (SOCKET_COM == SOC_SER)
#include "server.h"
#endif

//***************************** Local Constants *******************************
#define REC_MSG_BUF_LEN     (200)
#if (SOCKET_COM == SOC_CLI)
#define SEND_MES_TIME_DIF   (5)
#endif

//****************************** Local Functions ******************************
#if (SOCKET_COM == SOC_CLI)
static bool exceedTime(uint32 lTimeDif, time_t* lPrevTime);
#endif

//*********************************** main ************************************
//Purpose : Socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void main()
{
    bool blStatus = false;
    bool blReadStatus = false;
    uint8 pucRecieveBuffer[REC_MSG_BUF_LEN] = {0};
    bool blMesState = false;

#if (SOCKET_COM == SOC_CLI)
    time_t lPrevTime = 0;
    bool blSendState = false;
#elif (SOCKET_COM == SOC_SER)
    uint8* pucMessage = "Recieved!!";
#endif

#if (SOCKET_COM == SOC_CLI)
    blStatus = clientStartCon();
#elif (SOCKET_COM == SOC_SER)
    blStatus = serverStartCon();
#endif

    if (blStatus == true)
    {
        while (1)
        {

#if (SOCKET_COM == SOC_CLI)
            // Read message from the server
            blReadStatus = clientReadMessage(pucRecieveBuffer);
#elif (SOCKET_COM == SOC_SER)
            // Read message from the client
            blReadStatus = serverReadMessage(pucRecieveBuffer);
#endif

            if (blReadStatus == true)
            {
#if (SOCKET_COM == SOC_CLI)
                printf("Message from server: %s\n", pucRecieveBuffer);
#elif (SOCKET_COM == SOC_SER)
                printf("Message from client: %s\n", pucRecieveBuffer);
                
                // Send recieved message to client
                blMesState = serverSendMessage(pucMessage);
            
                if (blMesState == false)
                {
                    printf("Message not send\n");
                }
#endif
            }

#if (SOCKET_COM == SOC_CLI)

            blSendState = exceedTime(SEND_MES_TIME_DIF, &lPrevTime);

            if (blSendState == true)
            {
                uint8* message = "Hi!!";
                
                // Send a message to server in every 5 seconds
                blMesState = clientSendMessage(message);
            }

#endif
        }
    }
}

#if (SOCKET_COM == SOC_CLI)
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
#endif

// EOF

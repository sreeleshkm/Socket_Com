//********************** Socket communication - Server ************************
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
#include "server.h"

//***************************** Local Constants *******************************
#define READ_BUF_LEN (200)

//*********************************** main ************************************
//Purpose : Socket communication server side
//Inputs  : Nil
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
void main()
{
    bool blStatus = false;
    bool blReadStatus = false;
    uint8 pucRecieveBuffer[READ_BUF_LEN] = {0};
    bool blMesState = false;
    uint8* pucMessage = "Recieved!!";

    blStatus = serverStartCon();

    if (blStatus == true)
    {
        while (1)
        {
            // Read message from the client
            blReadStatus = serverReadMessage(pucRecieveBuffer);

            if (blReadStatus == true)
            {
                printf("Message from server: %s\n", pucRecieveBuffer);
                
                // Send recieved message to client
                blMesState = serverSendMessage(pucMessage);
            
                if (blMesState == false)
                {
                    printf("Message not send\n");
                }
            }
        }
    }
}

// EOF

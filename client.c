//********************** Socket communication - Client ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.c
// Summary  : This file includes the client functionalities of the socket
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 13/09/2024
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "client.h"

//***************************** Local Constants *******************************
#define CONNECTION_PORT         (3500)
#define CONNECTION_FAILED       (0)
#define CONNECTION_SUCCESS      (1)
#define NULL_CHARACTER          ('\0')
#define MESSAGE_BUF_LEN         (100)

//***************************** Local Variables *******************************
static uint32 sgulSocketDescriptor = 0;

//****************************** Local Functions ******************************
static bool openConnection(struct sockaddr_in* pstServerAdd);
static bool connectToSocket(struct sockaddr_in pstServerAdd);
static uint32 getSocketDescriptor(void);
static void setSocketDescriptor(uint32 ulSocDes);

//****************************** startClientCon *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool clientStartCon(void)
{
    bool blConState = false;
    bool blMesState = false;
    // Structure to represent the address
    struct sockaddr_in stServerAddress = {0};

    blConState = openConnection(&stServerAddress);

    if (blConState == true)
    {
        blConState = connectToSocket(stServerAddress);
        
        if (blConState == true)
        {
            printf("Client connetcted to the Server\n");
        }
        else
        {
            printf("Can not connect to the server\n");
        }
    }
    else
    {
        printf("Can not connect to the socket\n");
    }

    return blConState;
}

//****************************** openConnection ******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : pstServerAdd
//Return  : Return socket creation state
//Notes   : Nil
//*****************************************************************************
static bool openConnection(struct sockaddr_in* pstServerAdd)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    setSocketDescriptor(ulSocketDescriptor);

    if(ulSocketDescriptor < 0)
    {
        blStatus = false;
    }
    else
    {
        // Initialize address structure for binding
        pstServerAdd->sin_family = AF_INET;
        pstServerAdd->sin_port = htons(CONNECTION_PORT);
        // Set address to any address available
        pstServerAdd->sin_addr.s_addr = INADDR_ANY;
        pstServerAdd->sin_zero[8] = NULL_CHARACTER ;
    }

    return blStatus;
}

//****************************** connectToSocket ******************************
//Purpose : Try to connect to the server
//Inputs  : pstServerAdd
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
static bool connectToSocket(struct sockaddr_in pstServerAdd)
{
    bool blStatus = true;
    int32 lConState = CONNECTION_SUCCESS;
    uint32 ulSocketDescriptor = 0;
    
    ulSocketDescriptor = getSocketDescriptor();

    // Connect to the server
    lConState = connect(ulSocketDescriptor,
                        (struct sockaddr*)& pstServerAdd,
                        sizeof(pstServerAdd));

    if(lConState < CONNECTION_FAILED)
    {
        blStatus = false;
    }

    // Return connection status
    return blStatus;
}

//******************************** readMessage ********************************
//Purpose : Read message from server
//Inputs  : Nil
//Outputs : pucRecieveBuffer
//Return  : Return read message status
//Notes   : Nil
//*****************************************************************************
bool clientReadMessage(uint8* pucRecieveBuffer)
{
    bool blStatus = false;
    int32 lReadMesLen = 0;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescriptor();

    //Receive a message from the server
    lReadMesLen = recv(ulSocketDescriptor, pucRecieveBuffer, MESSAGE_BUF_LEN, MSG_DONTWAIT);

    if (lReadMesLen > 0)
    {
        blStatus = true;
    }

    return blStatus;
}

//******************************** writeMessage *******************************
//Purpose : Send message to server
//Inputs  : message
//Outputs : Nil
//Return  : Return send message status
//Notes   : Nil
//*****************************************************************************
bool clientSendMessage(uint8* pucMessage)
{
    bool blStatus = true;
    uint32 ulTransferMsgLen = 0;
    uint32 ulMsgLen = 0;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescriptor();

    ulMsgLen = strlen(pucMessage);
    // Send message to the server
    ulTransferMsgLen = send(ulSocketDescriptor, pucMessage, ulMsgLen, 0);

    if (ulTransferMsgLen != ulMsgLen)
    {
        blStatus = false;
    }

    return blStatus;
}

//**************************** closeClientConnection **************************
//Purpose : Close the socket communication
//Inputs  : ulSocketDescriptor
//Outputs : Nil
//Return  : Return the connection close status
//Notes   : Nil
//*****************************************************************************
bool clientCloseCon(void)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescriptor();

    // Terminate the socket connection
    close(ulSocketDescriptor);

    return blStatus;
}

//***************************** getSocketDescriptor ***************************
//Purpose : Read the socket descriptor value
//Inputs  : Nil
//Outputs : Nil
//Return  : Return the socket descriptor value
//Notes   : Nil
//*****************************************************************************
static uint32 getSocketDescriptor(void)
{
    return sgulSocketDescriptor;
}

//***************************** setSocketDescriptor ***************************
//Purpose : Set the socket descriptor value
//Inputs  : ulSocDes
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
static void setSocketDescriptor(uint32 ulSocDes)
{
    sgulSocketDescriptor = ulSocDes;
}

// EOF

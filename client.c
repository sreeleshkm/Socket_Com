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
#define CONENCTION_FAILED       (0)
#define CONNECTION_SUCCESS      (1)
#define NULL_CHARACTER          ('\0')
#define MESSAGE_BUF_LEN         (100)

//***************************** Local Variables *******************************
static uint32 sgulSocketDescreptor = 0;

//****************************** Local Functions ******************************
static bool openConnection(struct sockaddr_in* stServerAdd);
static bool connectToSocket(struct sockaddr_in stServerAdd);
static uint32 getSocketDescreptor(void);
static void setSocketDescreptor(uint32 ulSocDes);

//****************************** startClientCon *******************************
//Purpose : Open the client socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool startClientCon(void)
{
    bool blConState = false;
    bool blMesState = false;
    // Structure to represent the address
    struct sockaddr_in stServerAddress;

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
//Outputs : stServerAdd
//Return  : Return socket creation state
//Notes   : Nil
//*****************************************************************************
static bool openConnection(struct sockaddr_in* stServerAdd)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    setSocketDescreptor(ulSocketDescriptor);

    if(ulSocketDescriptor < 0)
    {
        blStatus = false;
    }
    else
    {
        // Initialize address structure for binding
        stServerAdd->sin_family = AF_INET;
        stServerAdd->sin_port = htons(CONNECTION_PORT);
        // Set address to any address available
        stServerAdd->sin_addr.s_addr = INADDR_ANY;
        stServerAdd->sin_zero[8] = NULL_CHARACTER ;
    }

    return blStatus;
}

//****************************** connectToSocket ******************************
//Purpose : Try to connect to the server
//Inputs  : stServerAdd
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
static bool connectToSocket(struct sockaddr_in stServerAdd)
{
    bool blStatus = true;
    uint32 ulConState = CONNECTION_SUCCESS;
    uint32 ulSocketDescriptor = 0;
    
    ulSocketDescriptor = getSocketDescreptor();

    // Connect to the server
    ulConState = connect(ulSocketDescriptor,
                        (struct sockaddr*)& stServerAdd,
                        sizeof(stServerAdd));

    if(ulConState < CONENCTION_FAILED)
    {
        blStatus = false;
    }

    // Return connection status
    return blStatus;
}

//******************************** readMessage ********************************
//Purpose : Read message from server
//Inputs  : Nil
//Outputs : RecieveBuffer
//Return  : Return read message status
//Notes   : Nil
//*****************************************************************************
bool readMessage(uint8* pucRecieveBuffer)
{
    bool blStatus = false;
    int32 lReadState = 0;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescreptor();

    //Receive a message from the server
    lReadState = recv(ulSocketDescriptor, pucRecieveBuffer, MESSAGE_BUF_LEN, MSG_DONTWAIT);

    if (lReadState > 0)
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
bool sendMessage(uint8* pucMessage)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescreptor();

    // Send message to the server
    write(ulSocketDescriptor, pucMessage, strlen(pucMessage));

    return blStatus;
}

//**************************** closeClientConnection **************************
//Purpose : Close the socket communication
//Inputs  : ulSocketDescriptor
//Outputs : Nil
//Return  : Return the connection close status
//Notes   : Nil
//*****************************************************************************
bool closeClientConnection(void)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;

    ulSocketDescriptor = getSocketDescreptor();

    // Terminate the socket connection
    close(ulSocketDescriptor);

    return blStatus;
}

//***************************** getSocketDescreptor ***************************
//Purpose : Read the socket descreptor value
//Inputs  : Nil
//Outputs : Nil
//Return  : Return the socket descreptor value
//Notes   : Nil
//*****************************************************************************
static uint32 getSocketDescreptor(void)
{
    return sgulSocketDescreptor;
}

//***************************** setSocketDescreptor ***************************
//Purpose : Set the socket descreptor value
//Inputs  : ulSocDes
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
static void setSocketDescreptor(uint32 ulSocDes)
{
    sgulSocketDescreptor = ulSocDes;
}

// EOF

//********************** Socket communication - Server ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.c
// Summary  : This file includes the client functionalities of the socket
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 20/09/2024
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
#include "server.h"

//***************************** Local Constants *******************************
#define CONNECTION_PORT         (3500)
#define CONENCTION_FAILED       (0)
#define CONNECTION_SUCCESS      (1)
#define NULL_CHARACTER          ('\0')
#define MESSAGE_BUF_LEN         (100)

//***************************** Local Variables *******************************
static uint32 gulSocketDescreptor = 0;
static uint32 gulClientSocket = 0;

//****************************** Local Functions ******************************
static bool openConnection(void);
static bool connectToSocket(void);
static uint32 getSocketDescreptor(void);
static void setSocketDescreptor(uint32 lSocDes);
static uint32 getClientSocket(void);
static void setClientSocket(uint32 lCliSoc);

//****************************** startServerCon *******************************
//Purpose : Open the server socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket connection state
//Notes   : Nil
//*****************************************************************************
bool startServerCon(void)
{
    bool blConState = false;
    bool blMesState = false;

    blConState = openConnection();

    if (blConState == true)
    {
        printf("Server connetction opened\n");
        blConState = connectToSocket();

        if (blConState == true)
        {
            printf("Server connected to client\n");
            uint8* message = "Hello!!";
            
            // Message send to server
            blMesState = sendMessage(message);
        }
    }

    return blConState;
}

//***************************** OpenCommunication *****************************
//Purpose : Create socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return socket creation status
//Notes   : Nil
//*****************************************************************************
static bool openConnection(void)
{
    bool blStatus = true;
    uint8 ucConState = 0;
    uint32 ulSocketDescriptor = 0;
    // Option value for respective option_name
    uint32 ulOptionValue = 1;
    // Structure to represent the address
    struct sockaddr_in stServerAddress;

    // Creating the socket with IPv4 domain and TCP protocol
    ulSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    setSocketDescreptor(ulSocketDescriptor);

    // Check if the socket is created successfully
    if(ulSocketDescriptor < 0)
    {
        printf("Socket creation failed");
        blStatus = false;
    }
    else
    {
        // Set options for the socket
        ucConState = setsockopt(ulSocketDescriptor, SOL_SOCKET, SO_REUSEADDR,
                        &ulOptionValue, sizeof(ulOptionValue));

        // Check if options are set successfully
        if(ucConState < 0)
        {
            printf("Couldn't set options\n");
            blStatus = false;
        }
        else
        {
            // Initializing structure elements for address
            stServerAddress.sin_family = AF_INET;
            // Convert port to network byte order using htons
            stServerAddress.sin_port = htons(CONNECTION_PORT);
            // Any address available
            stServerAddress.sin_addr.s_addr = INADDR_ANY;
            // Assigning null character to the last index of the character array
            stServerAddress.sin_zero[8] = NULL_CHARACTER;
            // Bind the socket with the values address and port from the 
            // sockaddr_in structure
            ucConState = bind(ulSocketDescriptor, (struct sockaddr*)&stServerAddress, 
                        sizeof(struct sockaddr));

            // Check if the binding was successful
            if(ucConState < 0)
            {
                printf("Couldn't bind socket\n");
                blStatus = false;
            }
        }
    }

    // Return comnnection status
    return blStatus;
}

//***************************** OpenCommunication *****************************
//Purpose : Listen and accpet the socket connetion
//Inputs  : Nil
//Outputs : Nil
//Return  : Return connection status
//Notes   : Nil
//*****************************************************************************
static bool connectToSocket(void)
{
    bool blStatus = true;
    uint8 ucConState = 0;
    struct sockaddr_in stConnectionAddress = {0};
    uint32 ulLengthOfAddress = 0;
    uint32 ulSocketDescriptor = 0;
    uint32 ulClientSocket = 0;

    ulSocketDescriptor = getSocketDescreptor();

    // Listen on specified port with a maximum of 4 requests
    ucConState = listen(ulSocketDescriptor, 4);

    // Check if the socket is listening successfully
    if(ucConState < 0)
    {
        printf("Couldn't listen for connections");
        blStatus = false;
    }

    ulLengthOfAddress = sizeof(stConnectionAddress);
    // Accept connection signals from the client
    ulClientSocket = accept(ulSocketDescriptor, 
                    (struct sockaddr*)&stConnectionAddress, &ulLengthOfAddress);
    setClientSocket(ulClientSocket);

    // Check if the server is accepting the signals from the client
    if(ulClientSocket < 0)
    {
        printf("Couldn't establish connection with client");
        blStatus = false;
    }

    // Return the connection status.
    return blStatus;
}

//******************************** readMessage ********************************
//Purpose : Read message from the client
//Inputs  : Nil
//Outputs : pucRecieveBuffer
//Return  : Return read message status
//Notes   : Nil
//*****************************************************************************
bool readMessage(uint8* pucRecieveBuffer)
{
    bool blStatus = false;
    int32 lReadState = 0;
    uint32 ulClientSocket = 0;

    ulClientSocket = getClientSocket();

    // Receive data sent by the client
    lReadState = recv(ulClientSocket, pucRecieveBuffer, MESSAGE_BUF_LEN, MSG_DONTWAIT);

    if (lReadState > 0)
    {
        blStatus = true;
    }

    return blStatus;
}

//******************************** sendMessage ********************************
//Purpose : Send message to the client
//Inputs  : pucMessage
//Outputs : Nil
//Return  : Return send message status
//Notes   : Nil
//*****************************************************************************
bool sendMessage(uint8* pucMessage)
{
    bool blStatus = true;
    uint32 ulClientSocket = 0;

    ulClientSocket = getClientSocket();

    // Send data to the client
    send(ulClientSocket, pucMessage, strlen(pucMessage), 0);

    return blStatus;
}

//******************************* closeConnection *****************************
//Purpose : Close the socket communication
//Inputs  : Nil
//Outputs : Nil
//Return  : Return the connection close status
//Notes   : Nil
//*****************************************************************************
bool closeConnection(void)
{
    bool blStatus = true;
    uint32 ulSocketDescriptor = 0;
    uint32 ulClientSocket = 0;

    ulClientSocket = getClientSocket();
    ulSocketDescriptor = getSocketDescreptor();

    // Close all the sockets created 
    close(ulSocketDescriptor);
    close(ulClientSocket);

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
    return gulSocketDescreptor;
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
    gulSocketDescreptor = ulSocDes;
}

//******************************* getClientSocket *****************************
//Purpose : Read the client socket value
//Inputs  : Nil
//Outputs : Nil
//Return  : Return the client socket value
//Notes   : Nil
//*****************************************************************************
static uint32 getClientSocket(void)
{
    return gulClientSocket;
}

//******************************* setClientSocket *****************************
//Purpose : Set the client socket value
//Inputs  : ulCliSoc
//Outputs : Nil
//Return  : Nil
//Notes   : Nil
//*****************************************************************************
static void setClientSocket(uint32 ulCliSoc)
{
    gulClientSocket = ulCliSoc;
}

// EOF

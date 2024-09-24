//********************** Socket communication - Client ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : client.h
// Summary  : Includes the function declarations 
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 13/09/2024
//
//*****************************************************************************
#ifndef _CLIENT_H_
#define _CLIENT_H_

//******************************* Include Files *******************************
#include <stdbool.h>
#include "types.h"

//**************************** Forward Declarations ***************************
bool startClientCon(void);
bool readMessage(uint8* RecieveBuffer);
bool sendMessage(uint8* message);
bool closeClientConnection(void);

#endif // _CLIENT_H_ 

// EOF
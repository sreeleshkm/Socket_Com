//********************** Socket communication - Server ************************
// Copyright (c) 2024 Trenser Technology Solutions (P) Ltd.
// All Rights Reserved
//*****************************************************************************
//
// File     : server.h
// Summary  : Includes the function declarations 
// Note     : Nil
// Author   : Sreelesh KM
// Date     : 20/09/2024
//
//*****************************************************************************
#ifndef _SERVER_H_
#define _SERVER_H_

//******************************* Include Files *******************************
#include <stdbool.h>
#include "types.h"

//**************************** Forward Declarations ***************************
bool serverStartCon(void);
bool serverReadMessage(uint8* pucRecieveBuffer);
bool serverSendMessage(uint8* pucMessage);
bool serverCloseConnection(void);

#endif // _SERVER_H_ 

// EOF
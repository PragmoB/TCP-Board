#pragma once

#include "PublicDeclare.h"

#include "PDU.h"

void CreateAccount(PDUAccount* PDUAcc, SOCKET sock, sqlite3* db);
void AccessAccount(PDUAccount* PDUAcc, SOCKET sock, sqlite3* db, map<unsigned int, string*>& client);
void ChangeName(PDUAccount* PDUAcc, SOCKET sock, sqlite3* db);

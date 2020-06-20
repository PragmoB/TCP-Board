#pragma once

#define PDUACC_SIZE 52
#define PDUBRD_SIZE 819

typedef struct tagPDUAccount
{
    char pKind[2];
    char ID[16];
    char PW[16];
    char name[20];
} PDUAccount;

typedef struct tagPDUBoard
{
    char pKind[2];
    char title[46];
    char content[751];
    unsigned char pDummy[20];
} PDUBoard;

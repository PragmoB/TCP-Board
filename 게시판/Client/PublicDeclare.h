#pragma once

#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

#include <cstdlib>

#include <cstring>

#include <vector>

#include <winsock2.h> // used with static library: libwsock32.a

#include <mmsystem.h> // used with static library: libwinmm.a

#define MENU_SIZE_X 117
#define MENU_SIZE_Y 27

using namespace std;

void gotoxy(int x, int y);

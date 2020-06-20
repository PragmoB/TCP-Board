#include "PublicDeclare.h"
#include "AboutAccount.h"
#include "AboutBoard.h"

unsigned int WINAPI Color(void* arg);

char color='Y';

int main()
{
    MCI_OPEN_PARMS mciOpen;
    MCI_PLAY_PARMS mciPlay;
    int dwID;

    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    DWORD dwThreadID;

    unsigned char input_var;
    short colunms = 1;

    system("title °Ô½ÃÆÇ");
    system("COLOR 0F");
    system("mode con cols=120 lines=30");


    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << " ÃÊ±âÈ­ ½ÇÆÐ. ¿¡·¯ÄÚµå: %d" << WSAGetLastError();
        getch();
        return 1;
    }

    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << " ¼ÒÄÏ »ý¼º ½ÇÆÐ : " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }

    char IP[100] = "";
    cout << "port : 4570, IP : ";
    cin >> IP;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons( 4570 );


    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cerr << " ¿¬°á ½ÇÆÐ.";

        switch(WSAGetLastError())
        {
        case 2:
            cout << " ¼­¹ö ¿î¿µ ½Ã°£ÀÌ ¾Æ´Õ´Ï´Ù. ´ÙÀ½¿¡ Ã£¾Æ¿Í ÁÖ¼¼¿ä." << endl;
            break;

        case 10060:
            cout << " ¼­¹ö ¿¬°áÀÌ ³Ê¹« ¿À·¡°á·Á ¿¬°áÀ» Áß´ÜÇß½À´Ï´Ù." << endl;
            break;

        default:
            cout << " ¿¡·¯ÄÚµå: " << WSAGetLastError() << endl;
        }
        closesocket(s);
        WSACleanup();
        getch();
        return 1;
    }


    send(s, "d4hUEHQ3d452", strlen("d4hUEHQ3d452"), 0);

    system("cls");


    // ÀÎÆ®·Î »ç¿îµå Àç»ý
    PlaySound(TEXT(".\\Sound\\Intro.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    // ÀÎÆ®·Î
    Sleep(200);
    system("COLOR 6");
    cout << " À¯Æ©ºê ÇÃ·¡±×¸ð ±¸µ¶";
    Sleep(360);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(342);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(396);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(93);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(276);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(300);
    system("COLOR E");
    Sleep(70);
    system("COLOR 6");
    Sleep(43);
    system("COLOR E");
    Sleep(105);
    system("COLOR 6");
    Sleep(1550);
    system("COLOR E");
    Sleep(350);


    _beginthreadex(NULL,0, &Color , 0 ,0 ,(unsigned*)&dwThreadID);

    // À½¾Ç Àç»ý
    mciOpen.lpstrDeviceType = "mpegvideo";
    mciOpen.lpstrElementName = ".\\BGM\\mainBgm.mp3";
    mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT|MCI_OPEN_TYPE|MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
    dwID = mciOpen.wDeviceID;
    mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);


    string ID="", PW="";
    while(true)
    {
        Main_Menu:

        system("cls");
        color = 'Y';
        colunms = 1;
        input_var = 0;

        cout << '';
        for(int i=0;i < MENU_SIZE_X;i++)
        {
            if(MENU_SIZE_X / 2 == i) // Áß°£¿¡ °Å²Ù·Î —©À¯ ¹®ÀÚ »ðÀÔ
            {
                cout << '';
                continue;
            }
            cout << '';

        }
        cout << '' << endl;
        for(int i=0;i < MENU_SIZE_Y;i++)
        {
            cout << '';
            for(int i=0;i < MENU_SIZE_X;i++)
            {
                if(MENU_SIZE_X / 2 == i) // Áß°£¿¡ |¹®ÀÚ »ðÀÔ
                {
                    cout << '';
                    continue;
                }
                cout << ' ';
            }

            cout << '' << endl;
        }
        cout << '';
        for(int i=0;i < MENU_SIZE_X;i++)
        {
            if(MENU_SIZE_X / 2 == i)
            {
                cout << '';
                continue;
            }
            cout << '';
        }
        cout << '' << endl;

        gotoxy(2, 1);
        cout << " ----- Main Menu -----";
        gotoxy(2, 2);
        cout << " 1. °èÁ¤ °ü·Ã";
        gotoxy(2, 3);
        cout << "  2. °Ô½ÃÆÇ °ü·Ã";
        gotoxy(2, 4);
        cout << "  3. Á¾·á";
        gotoxy(3, 2);
        while(input_var != 13)
        {
            input_var = getch();
            if(input_var == 224)
            {
                input_var = getch();
                if(input_var == 80)
                {
                    if(colunms > 2)
                        continue;

                    gotoxy(2, colunms+1);
                    cout << ' ';
                    gotoxy(2, ++colunms+1);

                    PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                    cout << '';

                }
                if(input_var == 72)
                {
                    if(colunms < 2)
                        continue;

                    gotoxy(2, colunms+1);
                    cout << ' ';
                    gotoxy(2, --colunms+1);

                    PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

                    cout << '';
                }
            }
            if(input_var == 27)
                goto Main_Menu;
        }
        PlaySound(TEXT(".\\Sound\\ChoiceSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        input_var = 0;
        switch(colunms)
        {
        case 1:
            colunms = 1;

            gotoxy(2, 7);
            cout << " ----- °èÁ¤ °ü·Ã ¸Þ´º -----";
            gotoxy(2, 9);
            cout << " 1. °èÁ¤ »ý¼º";
            gotoxy(2, 10);
            cout << "  2. ·Î±×ÀÎ";
            gotoxy(2, 11);

            while(input_var != 13)
            {
                input_var = getch();
                if(input_var == 224)
                {
                    input_var = getch();
                    if(input_var == 80)
                    {
                        if(colunms > 1)
                            continue;

                        gotoxy(2, colunms+8);
                        cout << ' ';
                        gotoxy(2, ++colunms+8);

                        PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                        cout << '';

                    }
                    if(input_var == 72)
                    {
                        if(colunms < 2)
                            continue;

                        gotoxy(2, colunms+8);
                        cout << ' ';
                        gotoxy(2, --colunms+8);

                        PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                        cout << '';
                    }
                }
                else if(input_var == 27)
                    goto Main_Menu;
            }

            PlaySound(TEXT(".\\Sound\\ChoiceSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            switch(colunms)
            {
            case 1:
                color = 'G';
                gotoxy(2, 25);
                cout << " Warning: ºñ¹Ð¹øÈ£ È®ÀÎ½Ã ´Ù ¾²°í ³­ÈÄ ¿£ÅÍ¸¦";
                gotoxy(2, 26);
                cout << " ´©¸£Áö ¸¶¼¼¿ä(ÀÚµ¿À¸·Î ³Ñ¾î°©´Ï´Ù)";
                CreateAccount(s, ID, PW);
                break;

            case 2:
                color = 'B';
                AccessAccount(s, ID, PW);
                break;
            }
            break;

        case 2:

            colunms = 1;

            gotoxy(2, 7);
            cout << " ----- °Ô½ÃÆÇ °ü·Ã ¸Þ´º -----";
            gotoxy(2, 9);
            cout << " 1. °Ô½ÃÆÇ »ý¼º";
            gotoxy(2, 10);
            cout << "  2. °Ë»ö";
            gotoxy(3, 9);

            while(input_var != 13)
            {
                input_var = getch();
                if(input_var == 224)
                {
                    input_var = getch();
                    if(input_var == 80)
                    {
                        if(colunms > 1)
                            continue;

                        gotoxy(2, colunms+8);
                        cout << ' ';
                        gotoxy(2, ++colunms+8);

                        PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                        cout << '';

                    }
                    if(input_var == 72)
                    {
                        if(colunms < 2)
                            continue;

                        gotoxy(2, colunms+8);
                        cout << ' ';
                        gotoxy(2, --colunms+8);

                        PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                        cout << '';
                    }
                }
                else if(input_var == 27)
                    goto Main_Menu;
            }

            PlaySound(TEXT(".\\Sound\\ChoiceSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            switch(colunms)
            {
            case 1:

                CreateBoard(s);
                break;
            case 2:

                FindBoard(s);
                break;
            }

            break;
        case 3:
            send(s, "3", strlen("3"), 0);

            Sleep(50);
            closesocket(s);
            WSACleanup();
            return 0;
        }
    }
    return 0;
}

unsigned int WINAPI Color(void* arg) // »ö±ò·Î ¸ÚÀ» ³»ÁÖ´Â ÇÔ¼ö
{
    while(true)
    {
        switch(color)
        {
        case 'Y':
            system("COLOR E");
            Sleep(900);
            system("COLOR 6");
            Sleep(900);
            break;
        case 'G':
            system("COLOR 2");
            Sleep(900);
            system("COLOR A");
            Sleep(900);
            break;

        case 'B':
            system("COLOR 3");
            Sleep(900);
            system("COLOR B");
            Sleep(900);
            break;
        }
    }
}

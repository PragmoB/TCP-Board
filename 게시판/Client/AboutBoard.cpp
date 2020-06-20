#include "PublicDeclare.h"

#include "PDU.h"

void CreateBoard(SOCKET s)
{
    char buff_rcv[30]="";
    bool isOver=false;

    if(send(s, "4", strlen("4"), 0) < 0)
    {
        gotoxy(3, 12);
        cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
        getch();
        exit(1);
    }
    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        gotoxy(3, 12);
        cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
        getch();
        exit(1);
    }

    if(strcmp(buff_rcv, " ·Î±×ÀÎ µÊ"))
    {
        gotoxy(3, 12);
        cout << " ·Î±×ÀÎÀ» ÇØÁÖ¼¼¿ä.";
        gotoxy(4, 13);
        system("PAUSE");
        return;
    }

    gotoxy(3, MENU_SIZE_Y - 4);
    cout << " Warning: Á¦¸ñÀº 29Byte, ³»¿ëÀº 499ByteÀÌ³»·Î ÀÛ¼º.";
    gotoxy(3, MENU_SIZE_Y - 3);
    cout << " # ÇÑ±Û, ÀÛÀºµû¿ÈÇ¥':2Byte";
    gotoxy(3, MENU_SIZE_Y - 2);
    cout << " # ¿µ¾î, '¸¦ Á¦¿ÜÇÑ Æ¯¼ö¹®ÀÚ, °ø¹é: 1Byte";
    gotoxy(4, MENU_SIZE_Y - 1);
    system("PAUSE");
    char input_var;
    PDUBoard PDUBrd;

    system("cls");
    cout << endl;
    cout << " ----- °Ô½ÃÆÇ »ý¼º -----" << endl;
    cout << endl;
    cout << " quit ÀÔ·Â½Ã Main Menu·Î ÀÌµ¿" << endl;
    cout << " 15ÀÚ ÀÌ³» Á¦¸ñ | ";

    memset(PDUBrd.title, 0, 46);
    cin.getline(&PDUBrd.title[1], 29);
    PDUBrd.title[0] = ' ';
    if(!strcmp(PDUBrd.title," quit"))
        return;

    for(int i=0;i < strlen(PDUBrd.title);i++)
    {
        if(PDUBrd.title[i] == '\'')
        {
            if(strlen(PDUBrd.title) >= 30)
            {
                isOver = true;
                if(i == strlen(PDUBrd.title)-1)
                {
                    PDUBrd.title[i] = 0;
                    break;
                }
                for(int j = 29;j >= i+2;j--)
                {
                    PDUBrd.title[j] = PDUBrd.title[j-1];
                }
            }
            else
                for(int j = strlen(PDUBrd.title);j >= i+2;j--)
                {
                    PDUBrd.title[j] = PDUBrd.title[j-1];
                }
            PDUBrd.title[i+1] = '\'';
            i++;
        }
    }
    if(cin.fail() || isOver == true)
    {
        cout << " ÃÖ´ë ÀÔ·Â °¡´ÉÇÑ ¹®ÀÚ ¼ö ÃÊ°ú·Î" << PDUBrd.content << " ±îÁö¸¸ Àû¿ëµË´Ï´Ù. °è¼Ó ÁøÇàÇÏ½Ã°Ú½À´Ï±î? y/n" << endl;
        cin.clear();
        fflush(stdin);

        do
        {
            input_var = getch();
        }while(input_var < 32 || input_var > 126);

        if(tolower(input_var) == 'n')
            return;
    }
    isOver = false;

    cout << endl;
    cout << " ³»¿ë" << endl;

    memset(PDUBrd.content, 0, 751);
    PDUBrd.content[0] = ' ';
    cin.getline(&PDUBrd.content[1], 499);

    if(!strcmp(PDUBrd.content, " quit"))
        return;

    for(int i=0;i < strlen(PDUBrd.content);i++)
    {
        if(PDUBrd.content[i] == '\'') // ÀÛÀºµû¿ÈÇ¥'°¡ ÀÖÀ¸¸é ±× µÚ¿¡ ÀÛÀºµû¿ÈÇ¥¸¦ ÇÏ³ª ´õ ³¢¿ö³Ö´Â´Ù(Äõ¸®¿¡¼­ ÀÛÀº µû¿ÈÇ¥¸¦ Ç¥ÇöÇÒ‹š´Â 2¹ø Ä£´Ù. '±×°Í'ÀÌ ºÎ·¯Áü(x) ''±×°Í''ÀÌ ºÎ·¯Áü(o))
        {
            // ÀÛÀº µû¿ÈÇ¥¸¦ ÇÏ³ª ´õ »ðÀÔÇÏ±â À§ÇØ µ¥ÀÌÅÍ¸¦ ¿À¸¥ÂÊÀ¸·Î ¹Ð¾î³½´Ù.
            if(strlen(PDUBrd.content) >= 500) // °ø°£ÀÌ ²ËÃ¡À»¶§´Â ¿¹¿ÜÀûÀ¸·Î Ã³¸®.
            {
                isOver = true;
                if(i == strlen(PDUBrd.content)-1)
                {
                    PDUBrd.content[i] = 0;
                    break;
                }
                for(int j = 499;j >= i+2;j--)
                {
                    PDUBrd.content[j] = PDUBrd.content[j-1];
                }
            }
            else // Á¤»óÀûÀ¸·Î Ã³¸®.
                for(int j = strlen(PDUBrd.content);j >= i+2;j--)
                {
                    PDUBrd.content[j] = PDUBrd.content[j-1];
                }

            // È®º¸ÇÑ ÀÚ¸®¿¡ ÀÛÀº µû¿ÈÇ¥¸¦ »ðÀÔ.
            PDUBrd.content[i+1] = '\'';
            i++;
        }
    }
    if(cin.fail() || isOver == true)
    {
        cout << endl << endl;

        cout << " ÃÖ´ë ÀÔ·Â °¡´ÉÇÑ ¹®ÀÚ ¼ö ÃÊ°ú·Î " << PDUBrd.content << " ±îÁö¸¸ Àû¿ëµË´Ï´Ù. °è¼Ó ÁøÇàÇÏ½Ã°Ú½À´Ï±î? y/n" << endl;
        cin.clear();
        fflush(stdin);

        do
        {
            input_var = getch();
        }while(input_var < 32 || input_var > 126);

        if(tolower(input_var) == 'n')
            return;

    }

    PDUBrd.pKind[0] = '2';
    PDUBrd.pKind[1] = '1';

    if(send(s, (const char*)&PDUBrd, PDUBRD_SIZE, 0) < 0)
    {
        cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
        getch();
        exit(1);
    }

    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
        getch();
        exit(1);
    }
    cout << buff_rcv << endl;
    cout << ' ';    system("PAUSE");
}
void FindBoard(SOCKET s)
{
    int column=1; // À§ÂÊÀ» ±âÁØÀ¸·Î ¼±ÅÃµÈ Ä®·³ÀÌ ¸î ¹øÂ°¿¡ ¼ÓÇØÀÖ´ÂÁö ³ªÅ¸³»´Â ¼ö
    int page=1; // ¼±ÅÃµÈ ÆäÀÌÁö°¡ ¸î ¹øÂ°¿¡ ¼ÓÇØÀÖ´ÂÁö ³ªÅ¸³»´Â ¼ö
    int boards=0; // ¼­¹ö·Î ºÎÅÍ Àü´Þ¹ÞÀº º¸µåÀÇ ¼ö
    unsigned char input_var = 0; // »ç¿ëÀÚÀÇ ÀÔ·ÂÀ» ¹Þ´Â º¯¼ö

    char buff_rcv[PDUBRD_SIZE] = "";
    char buff_snd[PDUBRD_SIZE] = "";

    bool breakbit = false;
    PDUBoard* pBuff;

    vector<vector<PDUBoard>> brdpage;
    vector<PDUBoard> brdlist;

    gotoxy(2, 13);
    cout << " ---- °Ë»ö ----" << endl;
    gotoxy(2, 15);
    cout << " 1. ¿À·¡µÈ ¼ø  " << endl;
    gotoxy(2, 16);
    cout << "  2. °Ë»ö" << endl;
    while(input_var != 13)
    {
        input_var = getch();

        if(input_var == 224)
        {
            input_var = getch();


            if(input_var == 72)
            {
                if(column < 2)
                    continue;

                gotoxy(2, column+14);
                cout << ' ';
                gotoxy(2, column+14-1);
                cout << '';
                PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                column--;
            }
            else if(input_var == 80)
            {
                if(column > 1)
                    continue;

                gotoxy(2, column+14);
                cout << ' ';
                gotoxy(2, column+14+1);
                cout << '';
                PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                column++;
            }
        }
        if(input_var == 27)
            return;
    }
    input_var = 0;
    PlaySound(TEXT(".\\Sound\\ChoiceSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    if(column == 1) // ¿À·¡µÈ ¼øÀÏ °æ¿ì
    {
        gotoxy(MENU_SIZE_X / 2 + 3, 4);
        cout << '';
        column = 1;
        pBuff = (PDUBoard*)buff_snd;

        pBuff->pKind[0] = '2';
        pBuff->pKind[1] = '2';
        strcpy(pBuff->title, "FTO"); // (FTO = From The Oldest) ¿À·¡µÈ ¼øÀ¸·Î

        gotoxy(MENU_SIZE_X / 2 + 3, 2);
        cout << " ---- ¿À·¡µÈ ¼ø ----";

        while(TRUE)
        {
            pBuff = (PDUBoard*)buff_snd;
            // intÇü µ¥ÀÌÅÍ¸¦ charÇü¿¡ ¾ïÁö·Î ±¸°Ü³Ö´Â Áß..
            pBuff->pDummy[0] = ((unsigned char*)&boards)[0];
            pBuff->pDummy[1] = ((unsigned char*)&boards)[1];
            pBuff->pDummy[2] = ((unsigned char*)&boards)[2];
            pBuff->pDummy[3] = ((unsigned char*)&boards)[3];

            if(send(s, buff_snd, PDUBRD_SIZE, 0) < 0)
            {
                gotoxy(MENU_SIZE_X / 2 + 3, 4);
                cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
                getch();
                exit(1);
            }

            pBuff = (PDUBoard*)buff_rcv;
            for(;;boards++)
            {
                if(recv(s, buff_rcv, PDUBRD_SIZE, 0) < 0)
                {
                    system("cls");
                    cout << " ¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾îÁ³½À´Ï´Ù." << endl;
                    exit(1);
                }
                if(!strcmp(buff_rcv, "End"))
                {
                    break;
                }
                brdlist.push_back(*pBuff);
                memset(buff_rcv, 0x00, PDUBRD_SIZE);
            }

            brdpage.push_back(brdlist);

            column = 1;
            while(TRUE)
            {

                while(input_var != 13)
                {
                    if(input_var == 77 || input_var == 75 || input_var == 0 )
                    {
                        for(int i=0;i < 12;i++)
                        {
                            gotoxy(MENU_SIZE_X / 2 + 4, i + 4);
                            cout << "                                           ";
                        }
                        // °Ô½ÃÆÇ ¼±ÅÃÈ­¸é Ãâ·Â. ¹æÇâÅ°¿Í ¿£ÅÍÅ°·Î ¼±ÅÃ.
                        for(int i=0;i < brdpage[page-1].size();i++)
                        {
                            gotoxy(MENU_SIZE_X / 2 + 4, i + 4);
                            cout << brdpage[page-1][i].title;
                            cout << " -" << brdpage[page-1][i].pDummy; // ±Û¾´ÀÌ Ãâ·Â
                        }
                        if(input_var == 77 || input_var == 75)
                        {
                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + column);
                            cout << ' ';
                            gotoxy(MENU_SIZE_X / 2 + 3, 4);
                            cout << '';
                            column = 1;
                            gotoxy((MENU_SIZE_X / 2 + MENU_SIZE_X) / 2 - 3, 18);
                            cout << "      ";
                        }
                        else
                        {
                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + column);
                            cout << '';
                        }
                        gotoxy((MENU_SIZE_X / 2 + MENU_SIZE_X) / 2 - 3, 18);
                        cout << (char)27 << ' ' << page << '/' << brdpage.size() << " ";
                    }

                    input_var = getch();

                    if(input_var == 224)
                    {
                        input_var = getch();

                        switch(input_var)
                        {
                        case 77:

                            // Áö±Ý º¸°íÀÖ´Â °Ô½Ã¹°À» Á¦¿ÜÇÑ ´Ù¸¥°ÍµéÀÌ ÀÌ ´õ Á¸ÀçÇÏ´Â°¡?
                            if(page*12 == boards)
                            {
                                breakbit=true;
                                for(int i=0;i < 12;i++)
                                {
                                    brdlist.pop_back();
                                }
                                page++;
                            }
                            else if(page*12 < boards)
                                page++;
                            else
                            {
                                input_var = 1;
                                break;
                            }
                            PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                            break;
                        case 75:

                            if(page <= 1)
                            {
                                input_var = 1;
                                break;
                            }
                            page--;
                            PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                            break;
                        case 80:

                            // brdpage[page-1] -> Áö±Ý º¸´Â ÆäÀÌÁö¸¦ ÀÇ¹Ì.
                            // brdpage[page-1].size() -> Áö±Ý º¸´Â ÆäÀÌÁöÀÇ °Ô½ÃÆÇ°³¼ö
                            if(column >= brdpage[page-1].size())
                                break;

                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + column);
                            cout << ' ';
                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + ++column);
                            cout << '';

                            PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                            break;
                        case 72:


                            if(column <= 1)
                                break;

                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + column);
                            cout << ' ';
                            gotoxy(MENU_SIZE_X / 2 + 3, 3 + --column);
                            cout << '';
                            PlaySound(TEXT(".\\Sound\\SelectSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                            break;
                        case 13:

                            break;

                        default:

                            input_var = 1;
                        }
                        if(breakbit)
                            break;
                    }
                    if(input_var == 27)
                    {
                        return;
                    }
                }
                if(breakbit)
                {
                    breakbit = false;
                    break;
                }
                PlaySound(TEXT(".\\Sound\\ChoiceSound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                input_var=0;
                system("cls");
                cout << brdpage[page-1][column-1].title << " -" << brdpage[page-1][column-1].pDummy << endl;
                cout << endl;
                cout << brdpage[page-1][column-1].content << endl;
                cout << ' ';    system("PAUSE");

                // º¸µå ¼±ÅÃÈ­¸éÀ¸·Î µ¹¾Æ¿À¸é ´Ù½Ã ¿¹»Ú°Ô ²Ù¹Î´Ù.
                system("cls");
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
                cout << "  1. °èÁ¤ °ü·Ã";
                gotoxy(2, 3);
                cout << " 2. °Ô½ÃÆÇ °ü·Ã";
                gotoxy(2, 4);
                cout << "  3. Á¾·á";


                gotoxy(2, 7);
                cout << " ----- °Ô½ÃÆÇ °ü·Ã ¸Þ´º -----";
                gotoxy(2, 9);
                cout << "  1. °Ô½ÃÆÇ »ý¼º";
                gotoxy(2, 10);
                cout << " 2. °Ë»ö";

                gotoxy(2, 13);
                cout << " ---- °Ë»ö ----" << endl;
                gotoxy(2, 15);
                cout << " 1. ¿À·¡µÈ ¼ø  " << endl;
                gotoxy(2, 16);
                cout << "  2. °Ë»ö" << endl;


                gotoxy(MENU_SIZE_X / 2 + 3, 2);
                cout << " ---- ¿À·¡µÈ ¼ø ----";
            }
        }
    }
}

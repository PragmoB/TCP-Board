#include "PublicDeclare.h"

#include "PDU.h"

void CreateBoard(SOCKET s)
{
    char buff_rcv[30]="";
    bool isOver=false;

    if(send(s, "4", strlen("4"), 0) < 0)
    {
        gotoxy(3, 12);
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }
    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        gotoxy(3, 12);
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }

    if(strcmp(buff_rcv, " �α��� ��"))
    {
        gotoxy(3, 12);
        cout << " �α����� ���ּ���.";
        gotoxy(4, 13);
        system("PAUSE");
        return;
    }

    gotoxy(3, MENU_SIZE_Y - 4);
    cout << " Warning: ������ 29Byte, ������ 499Byte�̳��� �ۼ�.";
    gotoxy(3, MENU_SIZE_Y - 3);
    cout << " # �ѱ�, ��������ǥ':2Byte";
    gotoxy(3, MENU_SIZE_Y - 2);
    cout << " # ����, '�� ������ Ư������, ����: 1Byte";
    gotoxy(4, MENU_SIZE_Y - 1);
    system("PAUSE");
    char input_var;
    PDUBoard PDUBrd;

    system("cls");
    cout << endl;
    cout << " ----- �Խ��� ���� -----" << endl;
    cout << endl;
    cout << " quit �Է½� Main Menu�� �̵�" << endl;
    cout << " 15�� �̳� ���� | ";

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
        cout << " �ִ� �Է� ������ ���� �� �ʰ���" << PDUBrd.content << " ������ ����˴ϴ�. ��� �����Ͻðڽ��ϱ�? y/n" << endl;
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
    cout << " ����" << endl;

    memset(PDUBrd.content, 0, 751);
    PDUBrd.content[0] = ' ';
    cin.getline(&PDUBrd.content[1], 499);

    if(!strcmp(PDUBrd.content, " quit"))
        return;

    for(int i=0;i < strlen(PDUBrd.content);i++)
    {
        if(PDUBrd.content[i] == '\'') // ��������ǥ'�� ������ �� �ڿ� ��������ǥ�� �ϳ� �� �����ִ´�(�������� ���� ����ǥ�� ǥ���ҋ��� 2�� ģ��. '�װ�'�� �η���(x) ''�װ�''�� �η���(o))
        {
            // ���� ����ǥ�� �ϳ� �� �����ϱ� ���� �����͸� ���������� �о��.
            if(strlen(PDUBrd.content) >= 500) // ������ ��á������ ���������� ó��.
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
            else // ���������� ó��.
                for(int j = strlen(PDUBrd.content);j >= i+2;j--)
                {
                    PDUBrd.content[j] = PDUBrd.content[j-1];
                }

            // Ȯ���� �ڸ��� ���� ����ǥ�� ����.
            PDUBrd.content[i+1] = '\'';
            i++;
        }
    }
    if(cin.fail() || isOver == true)
    {
        cout << endl << endl;

        cout << " �ִ� �Է� ������ ���� �� �ʰ��� " << PDUBrd.content << " ������ ����˴ϴ�. ��� �����Ͻðڽ��ϱ�? y/n" << endl;
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
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }

    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }
    cout << buff_rcv << endl;
    cout << ' ';    system("PAUSE");
}
void FindBoard(SOCKET s)
{
    int column=1; // ������ �������� ���õ� Į���� �� ��°�� �����ִ��� ��Ÿ���� ��
    int page=1; // ���õ� �������� �� ��°�� �����ִ��� ��Ÿ���� ��
    int boards=0; // ������ ���� ���޹��� ������ ��
    unsigned char input_var = 0; // ������� �Է��� �޴� ����

    char buff_rcv[PDUBRD_SIZE] = "";
    char buff_snd[PDUBRD_SIZE] = "";

    bool breakbit = false;
    PDUBoard* pBuff;

    vector<vector<PDUBoard>> brdpage;
    vector<PDUBoard> brdlist;

    gotoxy(2, 13);
    cout << " ---- �˻� ----" << endl;
    gotoxy(2, 15);
    cout << " 1. ������ ��  " << endl;
    gotoxy(2, 16);
    cout << "  2. �˻�" << endl;
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

    if(column == 1) // ������ ���� ���
    {
        gotoxy(MENU_SIZE_X / 2 + 3, 4);
        cout << '';
        column = 1;
        pBuff = (PDUBoard*)buff_snd;

        pBuff->pKind[0] = '2';
        pBuff->pKind[1] = '2';
        strcpy(pBuff->title, "FTO"); // (FTO = From The Oldest) ������ ������

        gotoxy(MENU_SIZE_X / 2 + 3, 2);
        cout << " ---- ������ �� ----";

        while(TRUE)
        {
            pBuff = (PDUBoard*)buff_snd;
            // int�� �����͸� char���� ������ ���ִܳ� ��..
            pBuff->pDummy[0] = ((unsigned char*)&boards)[0];
            pBuff->pDummy[1] = ((unsigned char*)&boards)[1];
            pBuff->pDummy[2] = ((unsigned char*)&boards)[2];
            pBuff->pDummy[3] = ((unsigned char*)&boards)[3];

            if(send(s, buff_snd, PDUBRD_SIZE, 0) < 0)
            {
                gotoxy(MENU_SIZE_X / 2 + 3, 4);
                cout << " �������� ������ ���������ϴ�." << endl;
                getch();
                exit(1);
            }

            pBuff = (PDUBoard*)buff_rcv;
            for(;;boards++)
            {
                if(recv(s, buff_rcv, PDUBRD_SIZE, 0) < 0)
                {
                    system("cls");
                    cout << " �������� ������ ���������ϴ�." << endl;
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
                        // �Խ��� ����ȭ�� ���. ����Ű�� ����Ű�� ����.
                        for(int i=0;i < brdpage[page-1].size();i++)
                        {
                            gotoxy(MENU_SIZE_X / 2 + 4, i + 4);
                            cout << brdpage[page-1][i].title;
                            cout << " -" << brdpage[page-1][i].pDummy; // �۾��� ���
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

                            // ���� �����ִ� �Խù��� ������ �ٸ��͵��� �� �� �����ϴ°�?
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

                            // brdpage[page-1] -> ���� ���� �������� �ǹ�.
                            // brdpage[page-1].size() -> ���� ���� �������� �Խ��ǰ���
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

                // ���� ����ȭ������ ���ƿ��� �ٽ� ���ڰ� �ٹδ�.
                system("cls");
                cout << '';
                for(int i=0;i < MENU_SIZE_X;i++)
                {
                    if(MENU_SIZE_X / 2 == i) // �߰��� �Ųٷ� ���� ���� ����
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
                        if(MENU_SIZE_X / 2 == i) // �߰��� |���� ����
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
                cout << "  1. ���� ����";
                gotoxy(2, 3);
                cout << " 2. �Խ��� ����";
                gotoxy(2, 4);
                cout << "  3. ����";


                gotoxy(2, 7);
                cout << " ----- �Խ��� ���� �޴� -----";
                gotoxy(2, 9);
                cout << "  1. �Խ��� ����";
                gotoxy(2, 10);
                cout << " 2. �˻�";

                gotoxy(2, 13);
                cout << " ---- �˻� ----" << endl;
                gotoxy(2, 15);
                cout << " 1. ������ ��  " << endl;
                gotoxy(2, 16);
                cout << "  2. �˻�" << endl;


                gotoxy(MENU_SIZE_X / 2 + 3, 2);
                cout << " ---- ������ �� ----";
            }
        }
    }
}

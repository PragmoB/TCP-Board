#include "PublicDeclare.h"

#include "PDU.h"

char CreateAccount(SOCKET s, string& ID, string& PW)
{
    char input;
    char buff_rcv[PDUACC_SIZE]="";
    char name[19];
    PDUAccount PDUAcc;
    string PW2;

    gotoxy(MENU_SIZE_X / 2 + 3, 2);
    cout << " ----- ���� ���� -----";
    gotoxy(MENU_SIZE_X / 2 + 3, 4);
    cout << " Esc: ������";
    gotoxy(MENU_SIZE_X / 2 + 3, 5);
    cout << " ID";
    gotoxy(MENU_SIZE_X / 2 + 4, 6);
    do
    {
        input = getch();
        if(input == 0x08) // BackSpaceŰ
        {
            if(ID.length() != 0)
                ID.pop_back();

            else
                continue;

            gotoxy(MENU_SIZE_X / 2 + 4, 6);
            cout << "                 ";

            gotoxy(MENU_SIZE_X / 2 + 4, 6);
            cout << ID;

            continue;
        }
        else if(input == 0x1B) // EscŰ
        {
            ID = "";
            return input;
        }
        else if(input == 0x0D) // EnterŰ
        {
            break;
        }
        else if(input == 224)
            getch();

        if(input > 126 || input < 32 || input == 39) // �⺻���� ���ڰ� �ƴϸ�(39�� ���������� syntax error �� ����Ű�� �����̴�.)
            continue;

        ID.push_back(input);    cout << input;

    }while(strlen(ID.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 8);
    cout << " ��й�ȣ �Է� ";
    gotoxy(MENU_SIZE_X / 2 + 4, 9);
    do
    {
        input = getch();
        if(input == 0x08) // BackspaceŰ
        {
            if(PW.length() != 0)
                PW.pop_back();

            else
                continue;

            gotoxy(MENU_SIZE_X / 2 + 4, 9);
            cout << "                 ";

            gotoxy(MENU_SIZE_X / 2 + 4, 9);
            for(unsigned int i=0;i < PW.length();i++)
                cout << '*';

            continue;
        }
        else if(input == 0x1B) // EscŰ
        {
            ID = "";
            PW = "";
            return input;
        }
        else if(input == 0x0D) // EnterŰ
        {
            if(strlen(PW.c_str()) < 4) // ��й�ȣ ���̰� 3�����϶� �Ѿ�� ����.
            {
                gotoxy(MENU_SIZE_X / 2 + 3, 8);
                cout << " 4�� �̻� ��й�ȣ �Է� ";

                gotoxy(MENU_SIZE_X / 2 + 4, 9);
                cout << "    ";
                PW = "";

                gotoxy(MENU_SIZE_X / 2 + 4, 9);
                continue;
            }
            break;
        }
        if(input > 126 || input < 32 || input == 39) // �⺻���� ���ڰ� �ƴϸ�(39�� ���������� syntax error �� ����Ű�� �����̴�.)
            continue;

        PW.push_back(input); cout << '*';
    }while(strlen(PW.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 11);
    cout << " ��й�ȣ Ȯ��";
    gotoxy(MENU_SIZE_X / 2 + 4, 12);
    while(PW.compare(PW2))
    {
        input = getch();
        if(input == 0x08) // Backspace
        {

            if(PW2.length() != 0)
                PW2.pop_back();

            else
                continue;

            gotoxy(MENU_SIZE_X / 2 + 4, 12);
            cout << "                 ";

            gotoxy(MENU_SIZE_X / 2 + 4, 12);
            for(unsigned int i=0;i < PW2.length();i++)
                cout << '*';

            continue;
        }
        else if(input == 0x1B) // EscŰ
        {
            ID = "";
            PW = "";
            PW2 = "";
            return input;
        }
        else if(input == 224)
            getch();

        if(input > 126 || input < 32 || input == 39) // �⺻���� ���ڰ� �ƴϸ�(39�� ���������� syntax error �� ����Ű�� �����̴�.)
            continue;

        PW2.push_back(input); cout << '*';
    }

    gotoxy(MENU_SIZE_X / 2 + 3, 13);
    cout << " ��й�ȣ�� ��ġ�մϴ�. �����ܰ�� �Ѿ�ϴ�.";
    Sleep(1200);

    gotoxy(MENU_SIZE_X / 2 + 3, 16);
    cout << " 8�� �̳� �г��� �Է�";

    gotoxy(MENU_SIZE_X / 2 + 4, 17);
    cout << "                 ";

    gotoxy(MENU_SIZE_X / 2 + 4, 17);
    name[0] = ' ';
    cin.getline(name + 1, 17);


    PDUAcc.pKind[0] = '1';
    PDUAcc.pKind[1] = '1';
    strcpy(PDUAcc.ID, ID.c_str());
    strcpy(PDUAcc.PW, PW.c_str());
    strcpy(PDUAcc.name, name);

    gotoxy(MENU_SIZE_X / 2 + 3, 20);

    if(send(s, (const char*)&PDUAcc, PDUACC_SIZE, 0) < 0)
    {
        cout << " �������� ������ ���������ϴ�.";
        getch();
        exit(1);
    }


    if(recv(s, buff_rcv, PDUACC_SIZE, 0) < 0) // ������ ���� �����߰ų� �����ߴٴ� ������ ����.
    {
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }


    if(!strcmp(buff_rcv, "UNIQUE constraint failed: Account.ID"))
        cout << " ���� ID�� ���� ������ �̹� �����մϴ�.";

    else
        cout << buff_rcv << endl;

    gotoxy(MENU_SIZE_X / 2 + 4, 21);
    system("PAUSE");

    ID = "";
    PW = "";

    input = 0x00;

    return input;
}

char AccessAccount(SOCKET s, string& ID, string& PW)
{
    char input;
    char buff_rcv[70]="";
    PDUAccount PDUAcc;

    gotoxy(MENU_SIZE_X / 2 + 3, 2);
    cout << " ----- �α��� -----";
    gotoxy(MENU_SIZE_X / 2 + 3, 4);
    cout << " Esc: ������" << endl;
    gotoxy(MENU_SIZE_X / 2 + 3, 5);
    cout << " ID";
    gotoxy(MENU_SIZE_X / 2 + 4, 6);
    do
    {
        input = getch();
        if(input == 0x08) // BackSpaceŰ
        {
            if(ID.length() != 0)
                ID.pop_back();

            else
                continue;

            gotoxy(MENU_SIZE_X / 2 + 4, 6);
            cout << "                ";

            gotoxy(MENU_SIZE_X / 2 + 4, 6);
            cout << ID;
            continue;
        }
        else if(input == 0x1B) // EscŰ
        {
            ID = "";
            return input;
        }
        else if(input == 0x0D) // EnterŰ
        {
            break;
        }
        else if(input == 224)
            getch();
        else if(input > 126 || input < 32 || input == 39) // �⺻���� ���ڰ� �ƴϸ�
            continue;

        ID.push_back(input);    cout << input;

    }while(strlen(ID.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 8);
    cout << " ��й�ȣ �Է� ";
    gotoxy(MENU_SIZE_X / 2 + 4, 9);
    do
    {
        input = getch();
        if(input == 0x08)
        {
            if(PW.length() != 0)
                PW.pop_back();

            else
                continue;

            gotoxy(MENU_SIZE_X / 2 + 4, 9);
            cout << "                "; // system("cls")��� ���� �����

            gotoxy(MENU_SIZE_X / 2 + 4, 9);
            for(unsigned int i=0;i < PW.length();i++)
                cout << '*';

            continue;
        }
        else if(input == 0x1B)
        {
            ID = "";
            PW = "";
            return input;
        }
        else if(input == 0x0D) // EnterŰ
        {
            break;
        }
        else if(input == 224)
            getch();

        else if(input > 126 || input < 32 || input == 39) // �⺻���� ���ڰ� �ƴϸ�(39�� ���������� syntax error �� ����Ű�� �����̴�.)
            continue;


        PW.push_back(input); cout << '*';

    }while(strlen(PW.c_str()) < 15 || input == 0x08);

    PDUAcc.pKind[0] = '1';
    PDUAcc.pKind[1] = '2';
    strcpy(PDUAcc.ID, ID.c_str());
    strcpy(PDUAcc.PW, PW.c_str());

    gotoxy(MENU_SIZE_X / 2 + 3, 12);

    if(send(s, (const char*)&PDUAcc, PDUACC_SIZE, 0) < 0)
    {
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }

    if(recv(s, buff_rcv, 70, 0) < 0)
    {
        cout << " �������� ������ ���������ϴ�." << endl;
        getch();
        exit(1);
    }

    cout << buff_rcv << endl;

    ID = "";
    PW = "";

    gotoxy(MENU_SIZE_X / 2 + 4, 13);
    system("PAUSE");

    input = 0x00;

    return input;
}

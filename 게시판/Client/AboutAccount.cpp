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
    cout << " ----- 계정 생성 -----";
    gotoxy(MENU_SIZE_X / 2 + 3, 4);
    cout << " Esc: 나가기";
    gotoxy(MENU_SIZE_X / 2 + 3, 5);
    cout << " ID";
    gotoxy(MENU_SIZE_X / 2 + 4, 6);
    do
    {
        input = getch();
        if(input == 0x08) // BackSpace키
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
        else if(input == 0x1B) // Esc키
        {
            ID = "";
            return input;
        }
        else if(input == 0x0D) // Enter키
        {
            break;
        }
        else if(input == 224)
            getch();

        if(input > 126 || input < 32 || input == 39) // 기본적인 문자가 아니면(39는 쿼리문에서 syntax error 를 일으키는 문자이다.)
            continue;

        ID.push_back(input);    cout << input;

    }while(strlen(ID.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 8);
    cout << " 비밀번호 입력 ";
    gotoxy(MENU_SIZE_X / 2 + 4, 9);
    do
    {
        input = getch();
        if(input == 0x08) // Backspace키
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
        else if(input == 0x1B) // Esc키
        {
            ID = "";
            PW = "";
            return input;
        }
        else if(input == 0x0D) // Enter키
        {
            if(strlen(PW.c_str()) < 4) // 비밀번호 길이가 3이하일때 넘어가지 못함.
            {
                gotoxy(MENU_SIZE_X / 2 + 3, 8);
                cout << " 4자 이상 비밀번호 입력 ";

                gotoxy(MENU_SIZE_X / 2 + 4, 9);
                cout << "    ";
                PW = "";

                gotoxy(MENU_SIZE_X / 2 + 4, 9);
                continue;
            }
            break;
        }
        if(input > 126 || input < 32 || input == 39) // 기본적인 문자가 아니면(39는 쿼리문에서 syntax error 를 일으키는 문자이다.)
            continue;

        PW.push_back(input); cout << '*';
    }while(strlen(PW.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 11);
    cout << " 비밀번호 확인";
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
        else if(input == 0x1B) // Esc키
        {
            ID = "";
            PW = "";
            PW2 = "";
            return input;
        }
        else if(input == 224)
            getch();

        if(input > 126 || input < 32 || input == 39) // 기본적인 문자가 아니면(39는 쿼리문에서 syntax error 를 일으키는 문자이다.)
            continue;

        PW2.push_back(input); cout << '*';
    }

    gotoxy(MENU_SIZE_X / 2 + 3, 13);
    cout << " 비밀번호가 일치합니다. 다음단계로 넘어갑니다.";
    Sleep(1200);

    gotoxy(MENU_SIZE_X / 2 + 3, 16);
    cout << " 8자 이내 닉네임 입력";

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
        cout << " 서버와의 연결이 끊어졌습니다.";
        getch();
        exit(1);
    }


    if(recv(s, buff_rcv, PDUACC_SIZE, 0) < 0) // 서버로 부터 성공했거나 실패했다는 응답을 받음.
    {
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }


    if(!strcmp(buff_rcv, "UNIQUE constraint failed: Account.ID"))
        cout << " 같은 ID를 가진 계정이 이미 존재합니다.";

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
    cout << " ----- 로그인 -----";
    gotoxy(MENU_SIZE_X / 2 + 3, 4);
    cout << " Esc: 나가기" << endl;
    gotoxy(MENU_SIZE_X / 2 + 3, 5);
    cout << " ID";
    gotoxy(MENU_SIZE_X / 2 + 4, 6);
    do
    {
        input = getch();
        if(input == 0x08) // BackSpace키
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
        else if(input == 0x1B) // Esc키
        {
            ID = "";
            return input;
        }
        else if(input == 0x0D) // Enter키
        {
            break;
        }
        else if(input == 224)
            getch();
        else if(input > 126 || input < 32 || input == 39) // 기본적인 문자가 아니면
            continue;

        ID.push_back(input);    cout << input;

    }while(strlen(ID.c_str()) < 15 || input == 0x08);

    gotoxy(MENU_SIZE_X / 2 + 3, 8);
    cout << " 비밀번호 입력 ";
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
            cout << "                "; // system("cls")대신 글자 지우기

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
        else if(input == 0x0D) // Enter키
        {
            break;
        }
        else if(input == 224)
            getch();

        else if(input > 126 || input < 32 || input == 39) // 기본적인 문자가 아니면(39는 쿼리문에서 syntax error 를 일으키는 문자이다.)
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
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }

    if(recv(s, buff_rcv, 70, 0) < 0)
    {
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
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

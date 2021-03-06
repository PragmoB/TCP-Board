#include "PublicDeclare.h"

#include "PDU.h"

void CreateBoard(SOCKET s)
{
    char buff_rcv[30]="";
    bool isOver=false;

    if(send(s, "4", strlen("4"), 0) < 0)
    {
        gotoxy(3, 12);
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }
    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        gotoxy(3, 12);
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }

    if(strcmp(buff_rcv, " 로그인 됨"))
    {
        gotoxy(3, 12);
        cout << " 로그인을 해주세요.";
        gotoxy(4, 13);
        system("PAUSE");
        return;
    }

    gotoxy(3, MENU_SIZE_Y - 4);
    cout << " Warning: 제목은 29Byte, 내용은 499Byte이내로 작성.";
    gotoxy(3, MENU_SIZE_Y - 3);
    cout << " # 한글, 작은따옴표':2Byte";
    gotoxy(3, MENU_SIZE_Y - 2);
    cout << " # 영어, '를 제외한 특수문자, 공백: 1Byte";
    gotoxy(4, MENU_SIZE_Y - 1);
    system("PAUSE");
    char input_var;
    PDUBoard PDUBrd;

    system("cls");
    cout << endl;
    cout << " ----- 게시판 생성 -----" << endl;
    cout << endl;
    cout << " quit 입력시 Main Menu로 이동" << endl;
    cout << " 15자 이내 제목 | ";

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
        cout << " 최대 입력 가능한 문자 수 초과로" << PDUBrd.content << " 까지만 적용됩니다. 계속 진행하시겠습니까? y/n" << endl;
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
    cout << " 내용" << endl;

    memset(PDUBrd.content, 0, 751);
    PDUBrd.content[0] = ' ';
    cin.getline(&PDUBrd.content[1], 499);

    if(!strcmp(PDUBrd.content, " quit"))
        return;

    for(int i=0;i < strlen(PDUBrd.content);i++)
    {
        if(PDUBrd.content[i] == '\'') // 작은따옴표'가 있으면 그 뒤에 작은따옴표를 하나 더 끼워넣는다(쿼리에서 작은 따옴표를 표현할떄는 2번 친다. '그것'이 부러짐(x) ''그것''이 부러짐(o))
        {
            // 작은 따옴표를 하나 더 삽입하기 위해 데이터를 오른쪽으로 밀어낸다.
            if(strlen(PDUBrd.content) >= 500) // 공간이 꽉찼을때는 예외적으로 처리.
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
            else // 정상적으로 처리.
                for(int j = strlen(PDUBrd.content);j >= i+2;j--)
                {
                    PDUBrd.content[j] = PDUBrd.content[j-1];
                }

            // 확보한 자리에 작은 따옴표를 삽입.
            PDUBrd.content[i+1] = '\'';
            i++;
        }
    }
    if(cin.fail() || isOver == true)
    {
        cout << endl << endl;

        cout << " 최대 입력 가능한 문자 수 초과로 " << PDUBrd.content << " 까지만 적용됩니다. 계속 진행하시겠습니까? y/n" << endl;
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
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }

    if(recv(s, buff_rcv, 30, 0) < 0)
    {
        cout << " 서버와의 연결이 끊어졌습니다." << endl;
        getch();
        exit(1);
    }
    cout << buff_rcv << endl;
    cout << ' ';    system("PAUSE");
}
void FindBoard(SOCKET s)
{
    int column=1; // 위쪽을 기준으로 선택된 칼럼이 몇 번째에 속해있는지 나타내는 수
    int page=1; // 선택된 페이지가 몇 번째에 속해있는지 나타내는 수
    int boards=0; // 서버로 부터 전달받은 보드의 수
    unsigned char input_var = 0; // 사용자의 입력을 받는 변수

    char buff_rcv[PDUBRD_SIZE] = "";
    char buff_snd[PDUBRD_SIZE] = "";

    bool breakbit = false;
    PDUBoard* pBuff;

    vector<vector<PDUBoard>> brdpage;
    vector<PDUBoard> brdlist;

    gotoxy(2, 13);
    cout << " ---- 검색 ----" << endl;
    gotoxy(2, 15);
    cout << " 1. 오래된 순  " << endl;
    gotoxy(2, 16);
    cout << "  2. 검색" << endl;
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

    if(column == 1) // 오래된 순일 경우
    {
        gotoxy(MENU_SIZE_X / 2 + 3, 4);
        cout << '';
        column = 1;
        pBuff = (PDUBoard*)buff_snd;

        pBuff->pKind[0] = '2';
        pBuff->pKind[1] = '2';
        strcpy(pBuff->title, "FTO"); // (FTO = From The Oldest) 오래된 순으로

        gotoxy(MENU_SIZE_X / 2 + 3, 2);
        cout << " ---- 오래된 순 ----";

        while(TRUE)
        {
            pBuff = (PDUBoard*)buff_snd;
            // int형 데이터를 char형에 억지로 구겨넣는 중..
            pBuff->pDummy[0] = ((unsigned char*)&boards)[0];
            pBuff->pDummy[1] = ((unsigned char*)&boards)[1];
            pBuff->pDummy[2] = ((unsigned char*)&boards)[2];
            pBuff->pDummy[3] = ((unsigned char*)&boards)[3];

            if(send(s, buff_snd, PDUBRD_SIZE, 0) < 0)
            {
                gotoxy(MENU_SIZE_X / 2 + 3, 4);
                cout << " 서버와의 연결이 끊어졌습니다." << endl;
                getch();
                exit(1);
            }

            pBuff = (PDUBoard*)buff_rcv;
            for(;;boards++)
            {
                if(recv(s, buff_rcv, PDUBRD_SIZE, 0) < 0)
                {
                    system("cls");
                    cout << " 서버와의 연결이 끊어졌습니다." << endl;
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
                        // 게시판 선택화면 출력. 방향키와 엔터키로 선택.
                        for(int i=0;i < brdpage[page-1].size();i++)
                        {
                            gotoxy(MENU_SIZE_X / 2 + 4, i + 4);
                            cout << brdpage[page-1][i].title;
                            cout << " -" << brdpage[page-1][i].pDummy; // 글쓴이 출력
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

                            // 지금 보고있는 게시물을 제외한 다른것들이 이 더 존재하는가?
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

                            // brdpage[page-1] -> 지금 보는 페이지를 의미.
                            // brdpage[page-1].size() -> 지금 보는 페이지의 게시판개수
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

                // 보드 선택화면으로 돌아오면 다시 예쁘게 꾸민다.
                system("cls");
                cout << '';
                for(int i=0;i < MENU_SIZE_X;i++)
                {
                    if(MENU_SIZE_X / 2 == i) // 중간에 거꾸로 뿩유 문자 삽입
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
                        if(MENU_SIZE_X / 2 == i) // 중간에 |문자 삽입
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
                cout << "  1. 계정 관련";
                gotoxy(2, 3);
                cout << " 2. 게시판 관련";
                gotoxy(2, 4);
                cout << "  3. 종료";


                gotoxy(2, 7);
                cout << " ----- 게시판 관련 메뉴 -----";
                gotoxy(2, 9);
                cout << "  1. 게시판 생성";
                gotoxy(2, 10);
                cout << " 2. 검색";

                gotoxy(2, 13);
                cout << " ---- 검색 ----" << endl;
                gotoxy(2, 15);
                cout << " 1. 오래된 순  " << endl;
                gotoxy(2, 16);
                cout << "  2. 검색" << endl;


                gotoxy(MENU_SIZE_X / 2 + 3, 2);
                cout << " ---- 오래된 순 ----";
            }
        }
    }
}

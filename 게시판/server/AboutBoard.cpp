#include "PublicDeclare.h"
#include "PDU.h"
#include "EncordingSet.h"

void CreateBoard(SOCKET sock, PDUBoard* pPDUBrd, sqlite3* db, map<unsigned int, string*> client)
{
    char* errMsg;
    char Query[PDUBRD_SIZE + 56];

    cout << endl;
    cout << " 소켓번호 " << sock << "  게시판 생성 시도" << endl;
    cout << " title: " << pPDUBrd->title << endl;
    cout << " ID: " << *client[sock] << endl;
    cout << " content" << endl;
    cout << pPDUBrd->content << endl;

    strcpy(pPDUBrd->title, ansi_to_utf8(pPDUBrd->title));
    strcpy(pPDUBrd->content, ansi_to_utf8(pPDUBrd->content));

    sprintf(Query, "INSERT INTO Board(title, ID, content) VALUES('%s', '%s', '%s')"
    , pPDUBrd->title, client[sock]->c_str(), pPDUBrd->content);

    if(sqlite3_exec(db, Query, NULL, NULL, &errMsg) == SQLITE_OK)
    {
        cout << " 게시판 생성 성공" << endl;
        cout << endl;
        send(sock, " 게시판 생성 성공", strlen(" 게시판 생성 성공"), 0);
    }
    else
    {
        cout << " 게시판 생성 실패" << endl;
        cout << " SQLite3: " << errMsg << endl;
        cout << endl;

        send(sock, errMsg, strlen(errMsg), 0);
    }
}

void FindBoard(SOCKET sock, PDUBoard* pPDUBrd, sqlite3* db)
{
    sqlite3_stmt* stmt, *stmt2;
    char Query[60]="";
    char buff_snd[PDUBRD_SIZE];

    // int형을 1바이트씩 끊어 char변수에 집어넣었기 때문에 중간 연산과정을 거쳐야 한다.
    int pages = *(int*)(pPDUBrd->pDummy);
    PDUBoard* pBuff = (PDUBoard*)buff_snd;

    cout << endl;
    cout << " 소켓번호 " << sock << " 검색시도" << endl;
    cout << " title:" << pPDUBrd->title;
    cout << " pages:" << pages << endl;
    cout << endl;

    if(!strcmp(pPDUBrd->title, "FTO"))
    {

        sprintf(Query, "SELECT title, content, ID FROM Board");

        if(sqlite3_prepare_v2(db, Query, -1, &stmt, 0) == SQLITE_OK)
        {
            for(int i=0;i < pages;i++)
                if(sqlite3_step(stmt) != SQLITE_ROW)
                {
                    send(sock, "End", strlen("End"), 0);
                    return;
                }
            for(int i=0;i < 12 && sqlite3_step(stmt) == SQLITE_ROW;i++)
            {
                strcpy(pBuff->title, utf8_to_ansi((const char*)sqlite3_column_text(stmt, 0)));
                strcpy(pBuff->content, utf8_to_ansi((const char*)sqlite3_column_text(stmt, 1)));

                // 닉네임 검색을 위한 아이디 검색
                memset(Query, 0x00, 60);
                sprintf(Query, "SELECT name FROM Account where ID='%s'", utf8_to_ansi((const char*)sqlite3_column_text(stmt, 2)));

                if(sqlite3_prepare_v2(db, Query, -1, &stmt2, 0) == SQLITE_OK)
                    if(sqlite3_step(stmt2) == SQLITE_ROW)
                        strcpy((char*)pBuff->pDummy, utf8_to_ansi((const char*)sqlite3_column_text(stmt2, 0)));

                sqlite3_finalize(stmt2);
                if(send(sock, buff_snd, PDUBRD_SIZE, 0) < 0)
                    return;

                memset(buff_snd, 0x00, PDUBRD_SIZE);
            }
            sqlite3_finalize(stmt);

            Sleep(60);
            if(send(sock, "End", strlen("End"), 0) < 0)
                return;
        }
        else
        {
            cerr << " SQLite3 error: " << sqlite3_errmsg(db);

            send(sock, " 게시판 정보가 담긴 데이터베이스를 참조하지 못했습니다.", strlen(" 게시판 정보가 담긴 데이터베이스를 참조하지 못했습니다."), 0);
        }
    }
}

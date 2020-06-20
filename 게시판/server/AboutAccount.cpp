#include "PublicDeclare.h"

#include "PDU.h"
#include "EncordingSet.h"

void CreateAccount(PDUAccount* pPDUAcc, SOCKET sock, sqlite3* db)
{
    char Query[101] = "";
    char* errMsg;

    cout << endl;
    cout << " 소켓번호 " << sock << " 계정 생성" << endl;
    cout << " ID: " << pPDUAcc->ID << endl;
    cout << " PW: " << pPDUAcc->PW << endl;
    cout << " name:" << pPDUAcc->name << endl;
    sprintf(Query, "INSERT INTO Account(ID, PW, name) VALUES('%s', '%s', '%s')" , pPDUAcc->ID, pPDUAcc->PW,
    ansi_to_utf8((const char*)pPDUAcc->name));
    if(sqlite3_exec(db, Query, NULL, NULL, &errMsg) == SQLITE_OK)
    {
        cout << " 계정 생성 완료" << endl << endl;

        send(sock, " 계정 생성 완료", strlen(" 계정 생성 완료"), 0);
    }
    else
    {
        cerr << " 소켓번호 " << sock << " 계정 생성 실패" << endl;
        cerr << " SQLite3: " << errMsg << endl << endl;

        send(sock, errMsg, strlen(errMsg), 0);
    }
}
void AccessAccount(PDUAccount* pPDUAcc, SOCKET sock, sqlite3* db, map<unsigned int, string*>& client)
{
    char Query[80]="";
    sqlite3_stmt* stmt = NULL;
    bool isBeen = false; // 입력한 정보와 같은 계정으로 로그인 한 사람이 있는가?

    cout << endl;
    cout << " 소켓번호 " << sock << " 로그인 시도" << endl;
    cout << " ID: " << pPDUAcc->ID << endl;
    cout << " PW: " << pPDUAcc->PW << endl;

    sprintf(Query, "SELECT name FROM Account where ID='%s' and PW='%s'", pPDUAcc->ID, pPDUAcc->PW);
    // DB에서 계정 정보를 참조하여 대입연산...
    if(sqlite3_prepare_v2(db, Query, -1, &stmt, 0) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            for(auto it : client) // 같은 계정으로 로그인 한 사람이 있는지 찾음.
            {
                if(it.first == sock) // 같은 계정으로 로그인 한 사람이 자기자신이면 찾는걸 건너뜀.
                {
                    continue;
                }
                if(it.second != NULL)
                    if(!strcmp(it.second->c_str(), pPDUAcc->ID))
                    {
                        cout << " 같은 계정으로 로그인 한 사람이 있음." << endl << endl;
                        send(sock, " 같은 계정으로 로그인 한 사람이 있음.", strlen(" 같은 계정으로 로그인 한 사람이 있음."), 0);

                        isBeen = true;
                        break;
                    }
            }
            if(isBeen == false)
            {
                if(client[sock] == NULL)
                {
                    client[sock] = new string(pPDUAcc->ID);
                }
                else
                {
                    *client[sock] = pPDUAcc->ID;
                }
                cout << " 성공. 이름은" << utf8_to_ansi((const char*)sqlite3_column_text(stmt, 0)) << endl << endl;
                send(sock, " 로그인 완료", strlen(" 로그인 완료"), 0);
            }
            isBeen = false;
        }
        else
        {
            cout << " 소켓번호 " << sock <<" 로그인 실패" << endl << endl;
            send(sock, " 로그인 실패", strlen(" 로그인 실패"), 0);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cerr << " SQLite3 error: " << sqlite3_errmsg(db);

        send(sock, " 계정 정보가 담긴 데이터베이스를 참조하지 못했습니다.", strlen(" 계정 정보가 담긴 데이터베이스를 참조하지 못했습니다."), 0);
    }
}

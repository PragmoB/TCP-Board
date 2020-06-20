#include "PublicDeclare.h"

#include "PDU.h"
#include "EncordingSet.h"

void CreateAccount(PDUAccount* pPDUAcc, SOCKET sock, sqlite3* db)
{
    char Query[101] = "";
    char* errMsg;

    cout << endl;
    cout << " ���Ϲ�ȣ " << sock << " ���� ����" << endl;
    cout << " ID: " << pPDUAcc->ID << endl;
    cout << " PW: " << pPDUAcc->PW << endl;
    cout << " name:" << pPDUAcc->name << endl;
    sprintf(Query, "INSERT INTO Account(ID, PW, name) VALUES('%s', '%s', '%s')" , pPDUAcc->ID, pPDUAcc->PW,
    ansi_to_utf8((const char*)pPDUAcc->name));
    if(sqlite3_exec(db, Query, NULL, NULL, &errMsg) == SQLITE_OK)
    {
        cout << " ���� ���� �Ϸ�" << endl << endl;

        send(sock, " ���� ���� �Ϸ�", strlen(" ���� ���� �Ϸ�"), 0);
    }
    else
    {
        cerr << " ���Ϲ�ȣ " << sock << " ���� ���� ����" << endl;
        cerr << " SQLite3: " << errMsg << endl << endl;

        send(sock, errMsg, strlen(errMsg), 0);
    }
}
void AccessAccount(PDUAccount* pPDUAcc, SOCKET sock, sqlite3* db, map<unsigned int, string*>& client)
{
    char Query[80]="";
    sqlite3_stmt* stmt = NULL;
    bool isBeen = false; // �Է��� ������ ���� �������� �α��� �� ����� �ִ°�?

    cout << endl;
    cout << " ���Ϲ�ȣ " << sock << " �α��� �õ�" << endl;
    cout << " ID: " << pPDUAcc->ID << endl;
    cout << " PW: " << pPDUAcc->PW << endl;

    sprintf(Query, "SELECT name FROM Account where ID='%s' and PW='%s'", pPDUAcc->ID, pPDUAcc->PW);
    // DB���� ���� ������ �����Ͽ� ���Կ���...
    if(sqlite3_prepare_v2(db, Query, -1, &stmt, 0) == SQLITE_OK)
    {
        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            for(auto it : client) // ���� �������� �α��� �� ����� �ִ��� ã��.
            {
                if(it.first == sock) // ���� �������� �α��� �� ����� �ڱ��ڽ��̸� ã�°� �ǳʶ�.
                {
                    continue;
                }
                if(it.second != NULL)
                    if(!strcmp(it.second->c_str(), pPDUAcc->ID))
                    {
                        cout << " ���� �������� �α��� �� ����� ����." << endl << endl;
                        send(sock, " ���� �������� �α��� �� ����� ����.", strlen(" ���� �������� �α��� �� ����� ����."), 0);

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
                cout << " ����. �̸���" << utf8_to_ansi((const char*)sqlite3_column_text(stmt, 0)) << endl << endl;
                send(sock, " �α��� �Ϸ�", strlen(" �α��� �Ϸ�"), 0);
            }
            isBeen = false;
        }
        else
        {
            cout << " ���Ϲ�ȣ " << sock <<" �α��� ����" << endl << endl;
            send(sock, " �α��� ����", strlen(" �α��� ����"), 0);
        }
        sqlite3_finalize(stmt);
    }
    else
    {
        cerr << " SQLite3 error: " << sqlite3_errmsg(db);

        send(sock, " ���� ������ ��� �����ͺ��̽��� �������� ���߽��ϴ�.", strlen(" ���� ������ ��� �����ͺ��̽��� �������� ���߽��ϴ�."), 0);
    }
}

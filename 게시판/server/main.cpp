#include "PublicDeclare.h"

#include "PDU.h"
#include "AboutAccount.h"
#include "AboutBoard.h"

#include <thread>
#include <process.h>

#define BUFF_SIZE  PDUBRD_SIZE+10

map<unsigned int, string*> client;

sqlite3* db = NULL;

unsigned int WINAPI active(void*);
unsigned int WINAPI ExeuntByForce(void* arg);

int main()
{
    WSADATA wsa;
    SOCKET binding_socket, client_socket;
    struct sockaddr_in server, client_addr;
    int client_addr_size;

    HANDLE hThread = NULL;
    DWORD dwThreadID = NULL;

    if(sqlite3_open(".//Data//BoardData.db", &db) != SQLITE_OK)
    {
        cerr << " 내장DB연결 실패: ";
        cerr << sqlite3_errmsg(db);
        getch();
        return 1;
    }
    cout << " 내장DB연결 성공" << endl;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << " 초기화 실패. 에러코드: " << WSAGetLastError();
        return 1;
    }
    cout << " 초기화 성공" << endl;

    if((binding_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << " 소켓 생성 실패. 에러코드: " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }
    cout << " 소켓 생성 성공" << endl;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons( 4570 );

    int nSockOpt = 1;
    setsockopt(binding_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nSockOpt, sizeof(nSockOpt));
    if (-1 == bind (binding_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        cerr << " bind()실행 에러";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit (1);
    }
    cout << " bind()실행 성공" << endl;

    if (-1 == listen (binding_socket, 5))
    {
        cerr << " 대기상태 모드 설정 실패.";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit (1);
    }
    cout << " 대기상태 모드 설정 성공" << endl;
    cout << endl;

    _beginthreadex(NULL, 0, ExeuntByForce, 0, 0, (unsigned*)&dwThreadID);
    while(1)
    {
        client_addr_size = sizeof (client_addr);

        if ( (client_socket = accept (binding_socket, (struct sockaddr *) &client_addr, &client_addr_size)) == -1)
        {
            cerr << " 클라이언트 연결 수락 실패.";
        }

        cout << " 클라이언트 접속.  소켓번호 : " << client_socket << endl;


        hThread = (HANDLE)_beginthreadex(NULL,0, active ,(void*)&client_socket,0,(unsigned*)&dwThreadID);
        if(hThread == 0)
            cerr << "_beginthreadex Error";
    }
    return 0;
}
unsigned int WINAPI active(void* arg)
{
    int sock = *(int*)arg;

    char buff_rcv[BUFF_SIZE];
    memset(buff_rcv, 0x00, BUFF_SIZE);
    client[sock] = NULL;

    // 비정상적인 서버 접속 차단
    if(recv(sock, buff_rcv, BUFF_SIZE, 0) < 0)
    {
        cout << " 소켓번호 " << sock << " 비정상적인 연결 종료" << endl;

        closesocket(sock);
        return NULL;

    }
    if(strcmp("d4hUEHQ3d452", buff_rcv))
    {
        cout << " 소켓번호 " << sock << " 비정상 접속 감지: " << buff_rcv << endl;

        closesocket(sock);
        return NULL;
    }

    memset(buff_rcv, 0x00, BUFF_SIZE);
    while(true)
    {
        if(recv(sock, buff_rcv, BUFF_SIZE, 0) < 0)
        {
            cout << " 소켓번호 " << sock << " 비정상적인 연결 종료" << endl;

            if(client[sock] != NULL)
            {
                delete client[sock];
            }
            client.erase(sock);
            closesocket(sock);
            return NULL;
        }
        /*
         * case 1: 로그인 관련 정보 처리
         * case 2: 게시판 관련 정보 처리
         * case 3: 종료
         * case 4: 로그인 여부 체크
         */
        switch(buff_rcv[0])
        {
        case '1':

            switch(buff_rcv[1])
            {

            case '1':

                CreateAccount((PDUAccount*)buff_rcv, sock, db);
                break;
            case '2':

                AccessAccount((PDUAccount*)buff_rcv, sock, db, client);
                break;
            case'3':


                break;
            }
            break;
        // -------------------------------------------------------
        case '2':

            switch(buff_rcv[1])
            {
            case '1':

                CreateBoard(sock, (PDUBoard*)buff_rcv, db, client);
                break;
            case '2':

                FindBoard(sock, (PDUBoard*)buff_rcv, db);
                break;
            }
            break;
        // -------------------------------------------------------
        case '3':

            cout << " 소켓번호 " << sock << " 연결 종료" << endl;

            if(client[sock] != NULL)
            {
                delete client[sock];
            }
            client.erase(sock);
            closesocket(sock);

            return NULL;
            break;
        // -------------------------------------------------------
        case '4':
            if(client[sock] != NULL)
            {
                if(send(sock, " 로그인 됨", strlen(" 로그인 됨"), 0) < 0)
                {
                    delete client[sock];
                    return 0;
                }
                break;
            }

            if(send(sock, " 로그인 안됨", strlen(" 로그인 안됨"), 0) < 0)
                return 0;

            break;
        //----------------------------------------------------------
        default:
            break;
        }
    }
    return NULL;
}

unsigned int WINAPI ExeuntByForce(void* arg) // 원하는 클라이언트를 소켓번호로 추방시키는 기능.
{
    int EBF_sock;

    while(TRUE)
    {
        cin >> EBF_sock;

        if(!cin)
        {
            cin.clear();

            while(cin.get() != '\n')
            { }

            continue;
        }
        closesocket(EBF_sock);
    }

}

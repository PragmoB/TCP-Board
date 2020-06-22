#include "PublicDeclare.h"

const char* ansi_to_utf8(const char* ansi)
{
    WCHAR unicode[1000]=L"";
    char utf8[1000]="";

    memset(unicode, 0, sizeof(unicode));
    memset(utf8, 0, sizeof(utf8));

    ::MultiByteToWideChar(CP_ACP, 0, ansi, -1, unicode, sizeof(unicode));
    ::WideCharToMultiByte(CP_UTF8, 0, unicode, -1, utf8, sizeof(utf8), NULL, NULL);

    return utf8;
}
const char* utf8_to_ansi(const char* utf8)
{
    WCHAR unicode[1000]=L"";
    char ansi[1000]="";

    memset(unicode, 0, sizeof(unicode));
    memset(ansi, 0, sizeof(ansi));

    ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, unicode, sizeof(unicode));
    ::WideCharToMultiByte(CP_ACP, 0, unicode, -1, ansi, sizeof(ansi), NULL, NULL);

    return ansi;
}

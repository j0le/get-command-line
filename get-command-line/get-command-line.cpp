#include <Windows.h>
#include <string>

int main()
{
    ::HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (std_out_handle == INVALID_HANDLE_VALUE)
        return 1;

    ::std::wstring cmdline_wstr(::GetCommandLineW());
    

    return 0;
}
#include <Windows.h>
#include <string>
#include <type_traits>
#include <nowide/convert.hpp>

static_assert(true == TRUE);
static_assert(false == FALSE);

int main()
{
    ::HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (std_out_handle == INVALID_HANDLE_VALUE)
        return 1;

    ::std::wstring cmdline_wstr(::GetCommandLineW());

    // check if std-out points to a console screen buffer
    bool std_out_point_to_console = false;
    {
        DWORD dummy;
        std_out_point_to_console = GetConsoleMode(std_out_handle, &dummy);
    }



    if (std_out_point_to_console) {
        cmdline_wstr += L"\n";

        const wchar_t* cmdline_wcstr = cmdline_wstr.c_str();
        const DWORD absolute_number_of_chars_to_write = cmdline_wstr.length(); // without terminating null character
        DWORD absolute_number_of_chars_written = 0;
        DWORD number_of_chars_written = 0;
        bool success = true;
        while
        (
            absolute_number_of_chars_written < absolute_number_of_chars_to_write
            &&
            (
                success = ::WriteConsoleW(
                /* _In_             HANDLE  hConsoleOutput,         */ std_out_handle,
                /* _In_       const VOID * lpBuffer,                */ &cmdline_wcstr[absolute_number_of_chars_written],
                /* _In_             DWORD   nNumberOfCharsToWrite,  */ absolute_number_of_chars_to_write - absolute_number_of_chars_written,
                /* _Out_opt_        LPDWORD lpNumberOfCharsWritten, */ &number_of_chars_written,
                /* _Reserved_       LPVOID  lpReserved              */ nullptr
                )
            )
            &&
            number_of_chars_written > 0
        ){
            absolute_number_of_chars_written += number_of_chars_written;
        }

        if
        (
            !success
            ||
            absolute_number_of_chars_written != absolute_number_of_chars_to_write
        ){
            return 1;
        }
    }
    else {
        auto cmdline_utf8_str = ::nowide::narrow(cmdline_wstr);
        static_assert(::std::is_same_v<decltype(cmdline_utf8_str), ::std::string>);
        static_assert(sizeof(::std::string::value_type) == 1);

        auto cmdline_utf8_cstr = cmdline_utf8_str.c_str();
        const DWORD absolute_number_of_bytes_to_write = cmdline_utf8_str.length(); // without terminating null character
        DWORD absolute_number_of_bytes_written = 0;
        DWORD number_of_bytes_written = 0;
        bool success = true;
        while
        (
            absolute_number_of_bytes_written < absolute_number_of_bytes_to_write
            &&
            (
                success = WriteFile(
                    /*[in]                HANDLE       hFile,                  */ std_out_handle,
                    /*[in]                LPCVOID      lpBuffer,               */ &cmdline_utf8_cstr[absolute_number_of_bytes_written],
                    /*[in]                DWORD        nNumberOfBytesToWrite,  */ absolute_number_of_bytes_to_write - absolute_number_of_bytes_written,
                    /*[out, optional]     LPDWORD      lpNumberOfBytesWritten, */ &number_of_bytes_written,
                    /*[in, out, optional] LPOVERLAPPED lpOverlapped            */ nullptr
                )
            )
            &&
            number_of_bytes_written > 0
        ){
            absolute_number_of_bytes_written += number_of_bytes_written;
        }

        if
        (
            !success
            ||
            absolute_number_of_bytes_written != absolute_number_of_bytes_to_write
        ){
            return 1;
        }
    }

    return 0;
}

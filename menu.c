#include <stdio.h>
#include <windows.h>

void print_error() {
    DWORD err_code;
    char *err_descr;

    err_code = GetLastError();
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&err_descr, 0, NULL);
    fprintf(stderr, "%s (%d)\n", err_descr, err_code);
    LocalFree(err_descr);
    return;
}

void print_usage() {
    puts("Usage: menu <menuitem> [[menuitem] menuitem ...]");
    puts("Up/Down to select, Enter to select, ESC to cancel.");
    puts("Return code is number of menu rows you selected, starting from 1, or 0 if canceled, or -1 in case of error.");
}

void abort_on(int expr) {
    if (expr) {
        print_error();
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    HANDLE in, out;
    int i;
    CONSOLE_SCREEN_BUFFER_INFO scr;
    COORD pos;
    DWORD mode;
    int loop;
    INPUT_RECORD rec;
    DWORD num;
    int choice;
    int direction;

    if (argc < 2) {
        print_usage();
        exit(-1);
    };
    abort_on(SetConsoleCtrlHandler(NULL, TRUE) == 0);
    abort_on((in = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE);
    abort_on((out = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE);
    abort_on(GetConsoleScreenBufferInfo(out, &scr) == 0);
    memcpy((void *)&pos, (const void *)&(scr.dwCursorPosition), sizeof(COORD));
    for (i = 1; i < argc; i++) {
        printf("    %d. %s\n", i, argv[i]);
    }
    abort_on(GetConsoleScreenBufferInfo(out, &scr) == 0);
    pos.X = 1;
    abort_on(SetConsoleCursorPosition(out, pos) == 0);
    putchar('>');
    abort_on(GetConsoleMode(in, &mode) == 0);
    abort_on(SetConsoleMode(in, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT)) == 0);
    loop = 1;
    choice = 1;
    while (loop) {
        abort_on(ReadConsoleInput(in, &rec, 1, &num) == 0);
        if (rec.EventType & KEY_EVENT && rec.Event.KeyEvent.bKeyDown == 1) {
            direction = 0;
            switch (rec.Event.KeyEvent.wVirtualKeyCode) {
            case VK_ESCAPE:
                choice = 0;
                loop = 0;
                break;
            case VK_RETURN:
                loop = 0;
                break;
            case VK_UP:
                direction = -1;
                break;
            case VK_DOWN:
                direction = 1;
                break;
            }
            if ((direction == -1 && choice > 1) || (direction == 1 && choice < (argc - 1))) {
                choice += direction;
                abort_on(SetConsoleCursorPosition(out, pos) == 0);
                putchar(' ');
                pos.Y += direction;
                abort_on(SetConsoleCursorPosition(out, pos) == 0);
                putchar('>');
            }
        }
    }
    abort_on(SetConsoleMode(in, mode) == 0);
    abort_on(SetConsoleCursorPosition(out, scr.dwCursorPosition) == 0);
    return choice;
}
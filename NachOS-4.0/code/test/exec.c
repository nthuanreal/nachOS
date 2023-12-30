#include "syscall.h"

int main() {
    int pid = -1;

    pid = Exec("add");

    if (pid < 0) {
        PrintString("False\n");
    }
    else {
        PrintString("Success\n");
        Join(pid);
    }
}
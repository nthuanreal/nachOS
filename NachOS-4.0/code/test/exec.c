#include "syscall.h"

int main() {
    int pid = -1;

    pid = Exec("add");

    if (pid < 0) {
        PrintString("exec.c: execute False\n");
    }
    else {
        PrintString("exec.c: execute Success\n");
        Join(pid);
    }
}
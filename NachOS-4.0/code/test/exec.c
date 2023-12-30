#include "syscall.h"

int main() {
    int pid;

    pid = Exec("createFile");

    if (pid < 0) {
        PrintString("False");
    }
    else {
        PrintString("Success");
        Join(pid);
    }
}
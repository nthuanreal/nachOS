#include "syscall.h"

char buffer[32];

int main() {
    PrintString("Enter string: ");
    ReadString(buffer,13);
    PrintString("\nString is: ");
    PrintString(buffer);
    PrintString("\n");
    Halt();
}
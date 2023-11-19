#include "syscall.h"
#define maxlen 32
int main()
{
    int len;
    char buf[32];
    char *filename;
    int OpenFileID;
    /*Create a file*/
    int CloseStatus;
    int byteRead; // bytes read
    
    PrintString("Hint: data.txt \n");
    PrintString("Nhap ten file muon doc: ");
    ReadString(filename, 32);
    OpenFileID = Open(filename, 0);
    if (OpenFileID != -1)
    {
        PrintString(" -> Mo file thanh cong\n");
        byteRead = Read(buf, 32, OpenFileID);
        PrintString("\n");

        PrintString("Noi dung file la: \n");
        PrintString("=========\n");
        PrintString(buf);
        PrintString("\n");
        PrintString("=========\n");

        CloseStatus = Close(OpenFileID);
    }
    else{
                PrintString(" -> Mo file that bai\n");
    }
    Halt();
}
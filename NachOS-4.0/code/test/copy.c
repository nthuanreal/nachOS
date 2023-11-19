#include "syscall.h"
#define maxlen 32
int main()
{
    char srcfname[maxlen];
    char desfname[maxlen];
    char buf[maxlen];
    char *alterFName = "Result_copy.txt";
    int srcfid;
    int desfid;
    int readByte;

    Create("result.txt");
    PrintString("Hint: source: data1.txt, destination: result.txt \n");
    PrintString("Nhap ten file source: \n");
    ReadString(srcfname, maxlen);
    PrintString("Nhap ten file destination: \n");
    ReadString(desfname, maxlen);
    srcfid = Open(srcfname, 0);
    desfid = Open(desfname, 0);
    if (srcfid == -1)
    {
        PrintString("File source khong ton tai!\n");
        Halt();
    }
    else if (desfid == -1)
    {
        PrintString("File destination khong ton tai! Dang tao file moi\n");
        Create(alterFName);
        desfid = Open(alterFName, 0);
        readByte = Read(buf, maxlen, srcfid);
        Write(buf, readByte, desfid);
        Close(desfid);
        Close(srcfid);
        PrintString("Copy thanh cong\n");
    }
    else
    {
        readByte = Read(buf, maxlen, srcfid);
        Write(buf, readByte, desfid);
        Close(desfid);
        Close(srcfid);
        PrintString("Copy thanh cong\n");
    }

    Halt();
}
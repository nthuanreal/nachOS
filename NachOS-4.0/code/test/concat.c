#include "syscall.h"

#define maxlen 32

int main(int argc, char *argv[])
{
    char *alterFName = "Result_concat.txt";
    char src1fname[maxlen];
    char src2fname[maxlen];
    char desfname[maxlen];
    char buf[maxlen];
    char buf2[maxlen];
    int src1fid;
    int src2fid;
    int desfid;
    int readByte1;
    int readByte2;
    Create("concat.txt");
    PrintString("Hint: source_1: data1.txt, source_2: data2.txt, destination: concat.txt \n");
    PrintString("Nhap ten file source 1: ");
    ReadString(src1fname, maxlen);
    PrintString("Nhap ten file source 2: ");
    ReadString(src2fname, maxlen);
    PrintString("Nhap ten file destination: ");
    ReadString(desfname, maxlen);
    src1fid = Open(src1fname, 0);
    src2fid = Open(src2fname, 0);
    desfid = Open(desfname, 0);

    if (src1fid == -1 || src2fid == -1)
    {
        if (src1fid == -1)
        {
            PrintString("File source 1 khong ton tai!\n");
            Halt();
        }
        if (src2fid == -1)
        {
            PrintString("File source 2 khong ton tai!\n");
            Halt();
        }
        Halt();
    }
    else
    {
        readByte1 = Read(buf, maxlen, src1fid);
        readByte2 = Read(buf2, maxlen, src2fid);
        if (desfid == -1)
        {
            PrintString("File destination khong ton tai! Dang tao file moi\n");
            Create(alterFName);
            desfid = Open(alterFName, 0);
            PrintString("Concatenate: ");
            PrintString(src1fname);
            PrintString(" + ");
            PrintString(src2fname);
            PrintString(" => ");
            PrintString(alterFName);
            PrintString("\n");
        }
        else
        {
            PrintString("Concatenate: ");
            PrintString(src1fname);
            PrintString(" + ");
            PrintString(src2fname);
            PrintString(" => ");
            PrintString(desfname);
            PrintString("\n");
        }
        Write(buf, readByte1, desfid);
        Write(buf2, readByte2, desfid);
    }

    Close(desfid);
    Close(src1fid);
    Close(src2fid);

    Halt();
    return 0;
}
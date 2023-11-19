#include "syscall.h"

int main()
{
  int result;
  char filename[32];
  char bufferRead[32];
  int result_open;
  Create("world.txt");
  Create("hello.txt");
  result_open = Open("hello.txt" , 0);
  

  PrintString("Hint: xoa file hello.txt de xem co xoa duoc file dang open khong \n");
  PrintString("Hint: xoa file world.txt \n");
  PrintString("\n");
  PrintString("Nhap ten file muon xoa: ");
  ReadString(filename, 32);
  result = Remove(filename);
  
  Halt();
    /* not reached */
}
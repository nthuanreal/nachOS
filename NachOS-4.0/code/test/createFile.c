

#include "syscall.h"

int main()
{
  int result;
  char filename[32];
  PrintString("Nhap ten file muon tao: ");
  ReadString(filename, 32);
  result = Create(filename);
  Halt();
  /* not reached */
} 
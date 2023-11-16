#include "syscall.h"

int main()
{
      char bufferRead[32];
  int result_open, result_close ,result_write , result_read;
  
  //0 read and write, 1 read only
  result_open = Open("hello.txt" , 0);
  
  result_write = Write("123459", 7 , result_open); 
    result_close = Remove("hello.txt");
    Halt();
    /* not reached */
}
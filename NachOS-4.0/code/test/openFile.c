
#include "syscall.h"

int main()
{
  char *bufferRead;
  int result_open, result_close ,result_write , result_read;
  
  //0 read and write, 1 read only
  // 0 consolin , 1 console out
   result_open = Open("hello.txt" , 0);
  // result_write = Write("1234567", 7 , 1); 
 // result_write = Write("1234567", 7 , result_open); 
  result_read = Read(bufferRead, 100 , 0); 

  Close(result_open);

  

  // result_write = Write("12345", 7 , result_open); 
  
  Halt();
  /* not reached */
}
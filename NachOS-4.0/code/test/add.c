/* add.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a add syscall that adds two values and returns the result.
 *
 */

#include "syscall.h"

int
main()
{
  int result;
  
  result = Add(42, 23);

  if (result == (42+23)) {
    PrintString("add.c:- Result = 55\n");
  }
  else {
    PrintString("add.c: Error\n");
  }
}

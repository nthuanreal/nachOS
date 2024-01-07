#include "syscall.h"
int main()
{
    SpaceId newProc = 0;
    SpaceId newProc1 = 0;
    SpaceId newProc2 = 0;
    int exitcode;

    PrintString("\n ===== Shell program =====\n- Type \"exit\" to exit shell. \n- Type \"test\" to run test program \n ================\n");

    newProc1 = Exec("add");        //
    newProc2 = Exec("createFile"); //

    exitcode = Join(newProc1);
    exitcode = Join(newProc2);
    // Exit(exitcode);
    while (1)
    {
        OpenFileId input = 0;
        char process1[32];
        int i = 0;

        PrintString("$ ");
        do
        {
            Read(&process1[i], 1, input);
        } while (process1[i++] != '\n');
        process1[--i] = '\0';
        PrintString("\n --- ");
        PrintString(process1);
        PrintString(" ---\n");
        {
            if (process1 != "" || process1 != "\n" || process1 != "\0")
            {
                newProc = Exec(process1);
                if (newProc == -9)
                {
                    Exit(exitcode);
                }
                exitcode = Join(newProc);

            }
        }
        process1[0] = "\0";
    }
}

#include "syscall.h"

int main()
{
    int serverFd, clientFd;
    int len;
    int port = 2500;
    int i, size;
    char buffer[1024];

    serverFd = OpenSocket();

    if (serverFd < 0) {
        PrintString("Cannot create socket\n");
        Halt();
    }

    if (Bind(serverFd,"192.168.0.14",port) < 0)
    {
        PrintString("Cannot bind sokcet\n");
        Halt();
    }
    if (Listen(serverFd, 10) < 0)
    {
        PrintString("Listen error\n");
        Halt();
    }
    while (1)
    {
        PrintString("waiting for clients\n");
        if ((clientFd = Accept(serverFd)) < 0)
        {
            PrintString("accept error");
            Halt();
        }

        size = Receive(clientFd, buffer, 1023);
        if (size < 0)
        {
            PrintString("read error");
            Halt();
        }
        PrintString("received from client: ");
        PrintString(buffer);
        PrintString("\n");

        i = 0;

        while (1)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
                buffer[i] = buffer[i] - 32;
            else if (buffer[i] == '\0') {
                break;
            }

            i++;
        }

        if (Send(clientFd, buffer, size) < 0)
        {
            PrintString("write error\n");
            Halt();
        }
        MyCloseSocket(clientFd);
    }
    MyCloseSocket(serverFd);
    return 0;
}
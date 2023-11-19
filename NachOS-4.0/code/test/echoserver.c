#include "syscall.h"

int main()
{
    int serverFd, clientFd;
    int len;
    int port = 2500;
    int i, size;
    char buffer[1024];
    char* IP = "192.168.0.14";

    if (SocketTCP() < 0) {
        PrintString("Cannot create file descriptors table\n");
        DeleteSocket();
        Halt();
    }
    PrintString("Enter string: ");
    ReadString(IP,13);

    serverFd = OpenSocket();

    if (serverFd < 0) {
        PrintString("Cannot create socket\n");
        DeleteSocket();
        Halt();
    }

    if (Bind(serverFd,IP,port) < 0)
    {
        PrintString("Cannot bind sokcet\n");
        DeleteSocket();
        Halt();
    }
    if (Listen(serverFd, 10) < 0)
    {
        PrintString("Listen error\n");
        DeleteSocket();
        Halt();
    }
    while (1)
    {
        PrintString("waiting for clients\n");
        if ((clientFd = Accept(serverFd)) < 0)
        {
            PrintString("accept error");
            DeleteSocket();
            Halt();
        }

        size = Receive(clientFd, buffer, 1023);
        if (size < 0)
        {
            PrintString("read error");
            DeleteSocket();
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
            DeleteSocket();
            Halt();
        }
        MyCloseSocket(clientFd);
    }
    MyCloseSocket(serverFd);
    return 0;
}
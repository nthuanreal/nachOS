#include "syscall.h"

int main()
{

    char *message[4] = {"hello server", "my name is thanh",
                        "nice to meet you", "see you later"};
    int i = 0;
    int socketId;
    char recv[1024];
    char* IP = "192.168.0.14";
    int port = 2500;

    if (SocketTCP() < 0) {
        PrintString("Cannot creat file decriptors table\n");
        DeleteSocket();
        Halt();
    }

    PrintString("Enter IP Address: ");
    ReadString(IP, 13);

    while (i < 4) {
        socketId = OpenSocket();

        if (socketId < 0) {
            PrintString("Cannot create socket\n");
            DeleteSocket();
            Halt();
        }

        if (Connect(socketId,IP,port) < 0) {
            PrintString("Cannot connect to server\n");
            DeleteSocket();
            Halt();
        }

        if (Send(socketId,message[i],255) < 0) {
            PrintString("Cannot Send to server\n");
            DeleteSocket();
            Halt();
        }

        if (Receive(socketId,recv,1023) < 0) {
            PrintString("Cannot receive from server\n");
            DeleteSocket();
            Halt();
        }
        
        PrintString(recv);
        PrintString("\n");
        i++;
    }
    DeleteSocket();
    Halt();
}
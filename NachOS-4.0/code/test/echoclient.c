#include "syscall.h"

int main()
{

    char *message[4] = {"hello server", "my name is thanh",
                        "nice to meet you", "see you later"};
    int i = 0;
    int socketIds[4];
    char recv[1024];
    char* IP = "192.168.0.14";

    PrintString("Enter IP Address: ");
    ReadString(IP, 13);

    while (i < 4) {
        socketIds[i] = OpenSocket();
        if (socketIds[i] < 0) {
            PrintString("Fail to create socket");
            Halt();
        }
        i++;
    }

    i = 0;

    while (i < 4) {
        if (Connect(socketIds[i],IP,2500) < 0) {
            PrintString("Fail to conncect \n");
            Halt();
        }
        i++;
    }

    i = 0;
    PrintString("\nReceive from server\n");
    while (i < 4) {
        if (Send(socketIds[i],message[i],255) < 0) {
            PrintString("Fail to send message\n");
            Halt();
        }
        else {
            if (Receive(socketIds[i],recv,1023) < 0) {
                PrintString("fail to receive.\n");
                Halt();
            }
            else{
                PrintString(recv);
                PrintString(" \n");
            }
        }
        i++;
    }
    i = 0;

    while (i < 4) {
        MyCloseSocket(socketIds[i]);
        i++;
    }
    Halt();
}
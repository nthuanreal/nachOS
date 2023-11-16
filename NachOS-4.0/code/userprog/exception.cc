// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "synchconsole.h"
#define MaxFileLength 32
#define READ_ONLY 1
#define READ_WRITE 0
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{

	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{

		case SC_Create:
		{
			int virtAddr;
			char *filename = new char[MaxFileLength];
			virtAddr = kernel->machine->ReadRegister(4);

			// MaxFileLength là = 32
			filename = User2System(virtAddr, MaxFileLength);
			if (filename == NULL)
			{
				printf("\n Not enough memory in system");
				kernel->machine->WriteRegister(2, -1);
				delete filename;
				IncreasePC();
				return;
			}

			if (!kernel->fileSystem->Create(filename))
			{
				printf("\n Error create file '%s'", filename);
				kernel->machine->WriteRegister(2, -1);
				delete filename;
				IncreasePC();
				return;
			}
			kernel->machine->WriteRegister(2, 0);
			delete filename; // delete filename;
			IncreasePC();
			break;
		}
		case SC_Open:
		{
			int bufAddr = kernel->machine->ReadRegister(4); // read string pointer from user
			int type = kernel->machine->ReadRegister(5);	//
			char *buf = User2System(bufAddr, MaxFileLength);
			int temp_index = findEmptySlot(kernel->fileSystem);
			OpenFile *temp = kernel->fileSystem->Open(buf);
			if (temp_index == -1 || type < 0 || type > 1 || temp == NULL)
			{
				DEBUG(dbgSys, "Can not open file");
				kernel->machine->WriteRegister(2, -1);
			}
			else
			{

				DEBUG(dbgSys, "open file successfully");
				DEBUG(dbgSys, buf << " " << temp_index);

				kernel->machine->WriteRegister(2, temp_index);
				kernel->fileSystem->fileDescriptorTable[temp_index].filename = User2System(bufAddr, MaxFileLength);
				kernel->fileSystem->fileDescriptorTable[temp_index].openFile = temp;
				kernel->fileSystem->fileDescriptorTable[temp_index].mode = type;
			}
			delete[] buf;
			IncreasePC();
			break;
		}
		case SC_Close:
		{
			int index_close = kernel->machine->ReadRegister(4);
			int result = close_syscall(kernel->fileSystem, index_close);
			if (result == -1)
			{
				DEBUG(dbgSys, "Fail to close");
			}
			else
			{
				DEBUG(dbgSys, "Close file");
			}
			kernel->machine->WriteRegister(2, result);
			IncreasePC();
			break;
		}
		case SC_Read:
		{

			int bufAddr = kernel->machine->ReadRegister(4);
			int NumBuf = kernel->machine->ReadRegister(5);
			int fileID = kernel->machine->ReadRegister(6);
			int OldPos;
			int NewPos;
			char *buf = new char[NumBuf];
			int bytesRead = 0;

			if (fileID >= 20 || fileID == 1)
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				break;
			}
			if (fileID > 1 && kernel->fileSystem->fileDescriptorTable[fileID].openFile == NULL)
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				break;
			}

			if (fileID == 0) // Console input
			{

				// int i = 0;
				// char ch = ;
				// // kernel->synchConsoleIn = new SynchConsoleInput(buf);
				// while ( &&   ) // Read up to NumBuf - 1 characters
				// {
				// 	buf[i] = ch;

				// 	if (ch == '\n' || ch == '\0')
				// 		break;

				// 	i++;
				// }
				int i = 0;
				char ch;
				do
				{
					ch = kernel->synchConsoleIn->GetChar();
					buf[i] = ch;
					i++;

				} while (ch != '\0' && ch != '\n' && i < NumBuf - 1);

				buf[i] = '\0'; // Null-terminate the +buffer

				System2User(bufAddr, i, buf);
				kernel->machine->WriteRegister(2, i);
			}
			else
			{

				OldPos = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();
				buf = User2System(bufAddr, NumBuf);

				int before_write = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();

				bytesRead = kernel->fileSystem->fileDescriptorTable[fileID].openFile->Read(buf, NumBuf);
				int after_write = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();
			}

			if (bytesRead > 0)
			{
				// Copy data from kernel to user space
				NewPos = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();
				System2User(bufAddr, bytesRead, buf);
				kernel->machine->WriteRegister(2, bytesRead);
				DEBUG(dbgSys, "the number of bytes read"
								  << bytesRead);
				DEBUG(dbgSys, "Reading successfully \n"
								  << buf);
			}
			else
			{
				kernel->machine->WriteRegister(2, -1);
				delete[] buf;
			}

			delete[] buf;
			IncreasePC();
			break;
		}
		case SC_Write:
		{
			int bufAddr = kernel->machine->ReadRegister(4);
			int NumBuf = kernel->machine->ReadRegister(5);
			int fileID = kernel->machine->ReadRegister(6);
			int OldPos;
			int NewPos;

			// mode = 1 means that openFile is just for reading only
			if (fileID >= 20 || fileID == 0)
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				break;
			}

			if (fileID > 1 && (kernel->fileSystem->fileDescriptorTable[fileID].openFile == NULL || kernel->fileSystem->fileDescriptorTable[fileID].mode == 1))
			{
				kernel->machine->WriteRegister(2, -1);
				IncreasePC();
				break;
			}

			// WRITE TO CONSOLE BY SYMCHCONSOLEOUT
			char *buf = User2System(bufAddr, NumBuf);
			if (fileID == 1)
			{
				int i = 0;
				// DEBUG(dbgSys, "fileID = 1\n");
				// kernel->synchConsoleOut = new SynchConsoleOutput(buf);
				while (buf[i] != '\0' && buf[i] != '\n')
				{
					kernel->synchConsoleOut->PutChar(buf[i]);
					i++;
				}
				buf[i] = '\n';
				kernel->synchConsoleOut->PutChar(buf[i]); // write last character

				kernel->machine->WriteRegister(2, i - 1);
				delete[] buf;
				IncreasePC();
				return;
				break;
			}

			// WRITE TO FILE
			int before_write = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();
			if ((kernel->fileSystem->fileDescriptorTable[fileID].openFile->Write(buf, NumBuf)) != 0)
			{
				int after_write = kernel->fileSystem->fileDescriptorTable[fileID].openFile->GetCurrentPos();
				System2User(bufAddr, after_write - before_write, buf);
				kernel->machine->WriteRegister(2, after_write - before_write);
				delete[] buf;
				IncreasePC();
				return;
				break;
			}

			IncreasePC();
			return;
			break;
		}
		// systemcall: SEEK
		case SC_Seek:
		{
			DEBUG(dbgSys, "Seek file\n");
			int result = SysSeek(kernel->machine->ReadRegister(4), kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "SC_Seek returning with code:" << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();
			return;
			break;
		}

		// systemcall: REmove
		case SC_Remove:
		{
			DEBUG(dbgSys, "Remove file\n");
			char *filename = User2System((int)kernel->machine->ReadRegister(4), MaxFileLength);
			int result = SysRemove(filename);
			DEBUG(dbgSys, "SC_Remove returning with code:" << result << "\n");
			kernel->machine->WriteRegister(2, (int)result);
			delete[] filename;

			IncreasePC();
			return;
			break;
		}

		case SC_Halt:
		{
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;
		}
		case SC_Add:
		{
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;

			ASSERTNOTREACHED();

			break;
		}

		default:
			cerr << "Unexpected system call " << type << "\n";
			IncreasePC();
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		IncreasePC();
		ASSERTNOTREACHED();
		break;
	}
}

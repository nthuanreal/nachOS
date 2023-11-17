/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "main.h"

void SysHalt()
{
	kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
	return op1 + op2;
}
//
// Increase program counter
void IncreasePC()
{
	// set previous program counter to current programcounter
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}
// User to system
char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	// printf("\n Filename u2s:");
	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		// printf("%c",kernelBuf[i]);
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}
// system to User
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}
// find the first slot in fileDescTable that empty and return the [index]. Else return -1
int findEmptySlot(FileSystem *temp)
{

	for (int i = 2; i < FILE_DESCRIPTOR_TABLE_SIZE; i++)
	{
		if (kernel->fileSystem->fileDescriptorTable[i].openFile == NULL)
		{
			return i;
		}
	}
	return -1;
}
bool checkValid(FileSystem *temp, int fileDescriptor);
// check if the file descritor is (in range and there is a file exist in fileDescriptorTable[fileDescriptor])
bool checkValid(FileSystem *temp, int fileDescriptor)
{
	if (fileDescriptor < 2 || fileDescriptor >= 20 || temp->fileDescriptorTable[fileDescriptor].openFile == NULL)
	{
		return false;
	}
	return true;
}
// remove file in fileDescriptorTable[fileDescriptor] and set it to NULL
int close_syscall(FileSystem *temp, int fileDescriptor)
{
	if (!checkValid(temp, fileDescriptor))
	{
		return -1;
	}

	delete temp->fileDescriptorTable[fileDescriptor].openFile;
	temp->fileDescriptorTable[fileDescriptor].openFile = NULL;
	delete temp->fileDescriptorTable[fileDescriptor].filename;
	return 0;
}

//
int SysRemove(char *filename)
{
	// kiểm tra input
	if (strlen(filename) == 0 || filename == NULL)
	{
		cout << "Remove failed: invalid filename!" << endl;
		return -1;
	}
	// kiểm tra file có mở không
	for (int i = 2; i < 20 ; i++)
	{
		if (kernel->fileSystem->fileDescriptorTable[i].openFile != NULL && strcmp(filename, kernel->fileSystem->fileDescriptorTable[i].filename) == 0)
		{
			DEBUG(dbgSys, "\n cannot remove file (file is openning) ...");
			cout << "Remove failed: " << filename << " is openning!" << endl;
			return -1;
		}
	}

	if (!kernel->fileSystem->Remove(filename))
	{
		cout << "Remove failed: " << filename << " doesn't exist!" << endl;
		return -1;
	}
	cout << "Remove file: " << filename << " success!" << endl;
	return 0;
}

int SysSeek(int position, OpenFileId id)
{
	if (id == 0 || id == 1)
	{
		cout << "Error: calling system call Seek in console!" << endl;
		return -1;
	}
	// seek into files: stdin, stdout, `out of domain` fileSystem
	if (!checkValid(kernel->fileSystem, id))
	{
		cout << "file not valid" << endl;
		return -1;
	}

	int len;
	len = kernel->fileSystem->fileDescriptorTable[id].openFile->Length(); // file size

	if (position == -1) // move file ptr to the end of file
	{
		position = len;
	}
	else
	{
		if (position > len || position < 0) // try to move file ptr to pos, pos > len --> wrong
		{
			return -1;
		}
	}
	kernel->fileSystem->fileDescriptorTable[id].openFile->Seek(position);
	return position;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */

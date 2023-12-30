#include"synch.h"
#include"pcb.h"

PCB::PCB(int id) {
    this->processID = id;

    if (id == 0) {
        this->parentID =-1;
    }
    else {
        this->processID = kernel->currentThread->processID;
    }

    this->numwait = this->exitcode = 0;
    this->thread = NULL;

    this->joinsem = new Semaphore("joinsem",0);
    this->exitsem = new Semaphore("exitsem",0);
    this->multex = new Semaphore("multex",1);
}

PCB::~PCB() {
    if (this->joinsem != NULL) {
        delete this->joinsem;
    }

    if (this->exitsem != NULL) {
        delete this->exitsem;
    }

    if (this->multex != NULL) {
        delete this->multex;
    }

    if (this->thread != NULL) {
        thread->FreeSpace();
        thread->Finish();
    }
}

int PCB::GetExitCode() {
    return this->exitcode;
}

int PCB::GetID() {
    return this->processID;
}

int PCB::GetNumWait() {
    return this->numwait;
}

void PCB::SetExitCode(int ec) {
    this->exitcode = ec;
}

void PCB::JoinWait() {
    joinsem->P();
}

void PCB::JoinRelease() {
    joinsem->V();
}

void PCB::ExitWait() {
    exitsem->P();
}

void PCB::ExitRelease() {
    exitsem->V();
}

void PCB::IncNumWait() {
    multex->P();
    numwait++;
    multex->V();
}

void PCB::DecNumWait() {
    multex->P();
    if (numwait > 0) {
        numwait--;
    }
    multex->V();
}

void PCB::SetFileName(char *fn) {
    if (fn != NULL) {
        strcpy(filename,fn);
    }
}

char *PCB::GetFileName() {
    return this->filename;
} 

void StartProcess_2(void* id) {
    int pid = *((int*)id);

    char* filename = kernel->pTab->GetFileName(pid);

    AddrSpace* space = new AddrSpace(filename);

    if (space == NULL) {
        printf("Can not create addrspace\n");
        return;
    }

    space->Execute();
    ASSERT(FALSE);
}

int PCB::Exec(char *name, int pid) {
    multex->P();
    
    this->thread = new Thread(name);

    if (this->thread == NULL) {
        printf("Not enough memory\n");
        multex->V();
        return -1;
    }

    this->thread->processID = pid;
    this->parentID = kernel->currentThread->processID;

    this->thread->Fork(StartProcess_2,&this->thread->processID);
    multex->V();
    return pid;
}


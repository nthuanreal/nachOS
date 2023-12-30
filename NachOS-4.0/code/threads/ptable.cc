#include "synch.h"
#include "ptable.h"


PTable::PTable(int size) {
    int i;

    if (size < 0) {
        return;
    }

    this->psize = size;
    bm = new Bitmap(this->psize);
    bmsem = new Semaphore("bmsem",1);

    for (i = 0; i < MAX_PROCESS; i++) {
        pcb[i] = NULL;
    }

    bm->Mark(0);
    pcb[0] = new PCB(0);
    pcb[0]->parentID = -1;
}

PTable::~PTable() {
    if (bmsem != NULL) {
        delete bmsem;
    }

    for (int i = 0; i < MAX_PROCESS; i++) {
        if (pcb[i] != NULL) {
            delete pcb[i];
        }
    }

    if (bm != NULL) {
        delete bm;
    }
}

int PTable::ExecUpdate(char *fn) {
    bmsem->P();

    if (fn == NULL) {
        printf("can not execute null file name\n");
        bmsem->V();
        return -1;
    }

    if (strcmp(fn,kernel->currentThread->getName()) == 0) {
        printf("can not execute itself\n");
        bmsem->V();
        return -1;
    }

    cerr << kernel->currentThread->getName() << endl;

    int index = this->GetFreeSlot();
    

    if (index < 0) {
        DEBUG(dbgSys,"There is no free slot\n");
        bmsem->V();
        return -1;
    }

    pcb[index] = new PCB(index);
    pcb[index]->SetFileName(fn);

    pcb[index]->parentID = kernel->currentThread->processID;

    int pid = pcb[index]->Exec(fn,index);

    bmsem->V();
    return pid;
}

int PTable::ExitUpdate(int exitcode) {
    int id = kernel->currentThread->processID;

    if (id == 0) {
        printf("Main process\n");
        kernel->currentThread->FreeSpace();
        kernel->interrupt->Halt();
        return 0;
    }

    if (IsExist(id) == false) {
        printf("This id is not exist\n");
        return -1;
    }


    pcb[id]->SetExitCode(exitcode);
    pcb[pcb[id]->parentID]->DecNumWait();

    pcb[id]->JoinRelease();
    pcb[id]->ExitWait();

    Remove(id);
    return id;
}  


int PTable::JoinUpdate(int id) {

    if (id < 0 || id >= psize || pcb[id] == NULL) {
        printf("Invalid process ID\n");
        return -1;
    }

    if (kernel->currentThread->processID != pcb[id]->parentID) {
        printf("Current process is not parent of execute process\n");
        return -1;
    }

    pcb[pcb[id]->parentID]->IncNumWait();

    pcb[id]->JoinWait();
    int exitcode = pcb[id]->GetExitCode();
    pcb[id]->ExitRelease();

    return exitcode;
}   

int PTable::GetFreeSlot() {
    return bm->FindAndSet();
}



bool PTable::IsExist(int pid) {
    return bm->Test(pid);
}

void PTable::Remove(int pid) {
    if (pcb[pid] != NULL) {
        delete pcb[pid];
        pcb[pid] = NULL;
    }
}  

char *PTable::GetFileName(int id) {
    return (pcb[id]->GetFileName());
}  
#include "synch.h"
#include "stable.h"


STable::STable() {
    bm = new Bitmap(MAX_SEMAPHORE);
    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        this->semTab[i] = NULL;
    }
}
STable::~STable() {
    if (bm) {
        delete bm;
        bm = NULL;
    }

    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        if (this->semTab[i]) {
            delete semTab[i];
            semTab[i] = NULL;
        }
    }
}

int STable::Create(char *name, int init) {

    if (name == NULL) {
        printf("name is NULL\n");
        return -1;
    }

    if (init < 0) {
        printf("Invalid init value\n");
        return -1;
    }

    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        if (bm->Test(i)) {
            if (strcmp(semTab[i]->GetName(),name) == 0) {
                return -1;
            }
        }
    }

    int index  = this->FindFreeSlot();

    if (index < 0) {
        printf("There is no free slot\n");
        return -1;
    }

    semTab[index] = new Sem(name,init);

    if (semTab[index] == NULL) {
        printf("Can not create sem element\n");
        return -1;
    }

    return 0;
}
    // If the semaphore name already exists, call this->P() to execute it.
    // If not, report an error in Wait, Signal function
int STable::Wait(char *name) {
    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        if (bm->Test(i)) {
            if (strcmp(semTab[i]->GetName(),name) == 0) {
                semTab[i]->wait();
                return 0;
            }
        }
    }
    return -1;
}

int STable::Signal(char* name) {
    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        if (bm->Test(i)) {
            if (strcmp(semTab[i]->GetName(),name) == 0) {
                semTab[i]->signal();
                return 0;
            }
        }
    }
    return -1;
}

int STable::FindFreeSlot() {
    return bm->FindAndSet();
}
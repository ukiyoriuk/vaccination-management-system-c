#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "patient.h"
#include "vaccinationBatch.h"
#include "country.h"

// Initialize a vaccine batch
tError vaccinationBatch_init(tVaccineBatch* vb, int id, tVaccine* vac, int num) {
    tError error;

    // Verify pre conditions
    assert(vb != NULL);
    assert(vac != NULL);

    // Allocate the memory for the name string field, using the length of the provided
    // text plus 1 space for the "end of string" char '\0'.
    // To allocate memory we use the malloc command.
    vb->vaccine = (tVaccine*)malloc(sizeof(tVaccine));

    // Check that memory has been allocated.
    // Pointer must be different from NULL.
    if(vb->vaccine == NULL) {
        return ERR_MEMORY_ERROR;
    } else {
        vb->vaccine->name = NULL;
    }

    // Once the memory is allocated, copy the data.
    vb->lotID = id;
    vb->quantity = num;
    error = vaccine_cpy(vb->vaccine, vac);

    // check if any error occured
    if(error != OK)
        return error;

    return OK;
}

// Release memory used by a vaccine batch
void vaccinationBatch_free(tVaccineBatch* vb) {
    // Verify pre conditions
    assert(vb != NULL);

    // All memory allocated with malloc and realloc needs to be freed using the free command.
    // In this case, as we use malloc to allocate the fields, we have to free them
    if(vb->vaccine != NULL) {
        vaccine_free(vb->vaccine);
        free(vb->vaccine);
        vb->vaccine = NULL;
    }

    vb->lotID = 0;
    vb->quantity = 0;
}

// Compare two vaccine batches
bool vaccinationBatch_equals(tVaccineBatch vb1, tVaccineBatch vb2) {
    bool result;

    result = true;
    // To see if two vaccines batches are equal, we need to see if their ids are equal.
    if(vb1.lotID != vb2.lotID) {
        // names are different
        result = false;
    }

    // All fields have the same value
    return result;
}

// Copy a vaccine batch
tError vaccinationBatch_cpy(tVaccineBatch* dest, tVaccineBatch* src) {
    tError error;

    // Verify pre conditions
    assert(dest != NULL);
    assert(src != NULL);

    // free dest vaccine (just in case)
    vaccinationBatch_free(dest);

    // initialize dest with src values.
    error = vaccinationBatch_init(dest, src->lotID, src->vaccine, src->quantity);
    // check if any error occured
    if(error != OK)
        return error;

    return OK;
}

tError vaccinationBatchList_create(tVaccinationBatchList *list) {
    if (list == NULL) return ERR_INVALID;   // parámetro nulo -> ERR_INVALID
    list->first = NULL;
    list->size  = 0u;
    return OK;
}

bool vaccinationBatchList_empty(tVaccinationBatchList list) {
    return (list.size == 0u);
}

void vaccinationBatchList_free(tVaccinationBatchList *list) {
    if (list == NULL) return;
    tVaccinationBatchListNode *p = list->first;
    while (p != NULL) {
        tVaccinationBatchListNode *n = p->next;
        free(p);
        p = n;
    }
    list->first = NULL;
    list->size  = 0u;
}

tError vaccineBatchList_insert(tVaccinationBatchList* list, tVaccineBatch vb, int index) {
    if (list == NULL) return ERR_INVALID;
    if (index < 0 || (unsigned)index > list->size) return ERR_INVALID_INDEX;

    tVaccinationBatchListNode *node = (tVaccinationBatchListNode*) malloc(sizeof(*node));
    if (node == NULL) return ERR_MEMORY_ERROR;

    node->e    = vb;
    node->next = NULL;

    if (index == 0) {
        node->next  = list->first;
        list->first = node;
    } else {
        tVaccinationBatchListNode *prev = list->first;
        for (int i = 0; i < index - 1 && prev != NULL; ++i) prev = prev->next;
        if (prev == NULL) { free(node); return ERR_INVALID_INDEX; }
        node->next = prev->next;
        prev->next = node;
    }

    list->size++;
    return OK;
}

tError vaccineBatchList_delete(tVaccinationBatchList* list, int index) {
    if (list == NULL) return ERR_INVALID;
    if (vaccinationBatchList_empty(*list)) return ERR_EMPTY_LIST;
    if (index < 0 || (unsigned)index >= list->size) return ERR_INVALID_INDEX;

    tVaccinationBatchListNode *toDel = NULL;

    if (index == 0) {
        toDel = list->first;
        list->first = toDel->next;
    } else {
        tVaccinationBatchListNode *prev = list->first;
        for (int i = 0; i < index - 1 && prev != NULL; ++i) prev = prev->next;
        if (prev == NULL || prev->next == NULL) return ERR_INVALID_INDEX;
        toDel = prev->next;
        prev->next = toDel->next;
    }

    free(toDel);
    list->size--;
    return OK;
}

tVaccinationBatchListNode* vaccineBatchList_get(tVaccinationBatchList list, int index) {
    if (list.size == 0u) return NULL;
    if (index < 0 || (unsigned)index >= list.size) return NULL;

    tVaccinationBatchListNode *p = list.first;
    for (int i = 0; i < index && p != NULL; ++i) p = p->next;
    return p;  // NULL si index fuera de rango
}

// Duplicate list
tError vaccinationBatchList_duplicate(tVaccinationBatchList *dest, tVaccinationBatchList src) {
    tVaccinationBatchListNode *currNode = NULL;
    tVaccinationBatchListNode *nextNode = NULL;
    tError err;

    // free list (just in case)
    vaccinationBatchList_free(dest);

    // Initialize new list
    err = vaccinationBatchList_create(dest);
    if(err != OK)
        return err;

    // Duplicate the list
    currNode = src.first;
    while(currNode != NULL && err == OK) {
        nextNode = currNode->next;
        err = vaccineBatchList_insert(dest, currNode->e, 0);
        currNode = nextNode;
    }
    return err;
}

// inoculate first vaccine to a patient from a batch list
void vaccineBatchList_inoculate_first_vaccine(tVaccinationBatchList* vbList, tPatient* patient) {

    if (vbList == NULL || patient == NULL) return;
    if (patient->number_doses != 0) return; // no corresponde primera

    tVaccinationBatchListNode *node = vbList->first;
    while (node != NULL) {
        tVaccineBatch *vb = &node->e;
        if (vb->quantity > 0 && patient_isSuitableForVaccine(patient, vb->vaccine)) {
            // Asignar vacuna y lote
            if (patient->vaccine == NULL) {
                size_t n = strlen(vb->vaccine->name);
                patient->vaccine = (char*)malloc((n + 1) * sizeof(char));
                if (patient->vaccine == NULL) return; // sin memoria -> no inoculamos
                strcpy(patient->vaccine, vb->vaccine->name);
            } else {
                // si viniera con string previa, no debería ocurrir en 1ª dosis, pero por seguridad:
                if (strcmp(patient->vaccine, vb->vaccine->name) != 0) {
                    // Mantener la primera vacuna que se le asigna en PR2; pero si quieres
                    // forzar a la del lote actual, libera y asigna. Aquí no tocamos.
                }
            }
            patient->lotID = vb->lotID;
            patient->number_doses += 1;
            vb->quantity -= 1;
            return;
        }
        node = node->next;
    }
}

// inoculate second vaccine to a patient from a batch list
void vaccineBatchList_inoculate_second_vaccine(tVaccinationBatchList* vbList, tPatient* patient) {

    if (vbList == NULL || patient == NULL) return;
    if (patient->number_doses != 1) return; // no corresponde segunda
    if (patient->vaccine == NULL) return;   // no sabemos cuál fue la primera
    if (strcmp(patient->vaccine, JANSSEN_VAC) == 0) {
        // Janssen es monodosis: no aplicar segunda
        return;
    }

    tVaccinationBatchListNode *node = vbList->first;
    while (node != NULL) {
        tVaccineBatch *vb = &node->e;
        if (vb->quantity > 0 &&
            vb->vaccine != NULL &&
            vb->vaccine->name != NULL &&
            strcmp(vb->vaccine->name, patient->vaccine) == 0 &&
            patient_isSuitableForVaccine(patient, vb->vaccine)) {

            patient->number_doses += 1;
            vb->quantity -= 1;
            return;
        }
        node = node->next;
    }
}

// function to explore all batches to inoculate to a patient
void vaccineBatchList_inoculate(tVaccinationBatchList* vbList, tPatient* patient) {

    if (vbList == NULL || patient == NULL) return;

    if (patient->number_doses == 0) {
        vaccineBatchList_inoculate_first_vaccine(vbList, patient);
    } else if (patient->number_doses == 1) {
        // si fue Janssen, no hay segunda
        if (patient->vaccine != NULL && strcmp(patient->vaccine, JANSSEN_VAC) == 0) return;
        vaccineBatchList_inoculate_second_vaccine(vbList, patient);
    }
}

// Sorts input list using quickSort algorithm
void vaccineBatchList_quickSortRecursive(tVaccinationBatchList *list, int low, int high) {
    if (list == NULL) return;
    if (low < high) {
        int pivot = high;
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            /* Comparador inline:
               1) lotID ascendente
               2) si empatan, nombre de vacuna en DESCENDENTE
            */
            int lj = vaccineBatchList_getlotID(*list, j);
            int lp = vaccineBatchList_getlotID(*list, pivot);

            int cmp;
            if (lj < lp) cmp = -1;
            else if (lj > lp) cmp = 1;
            else {
                tVaccinationBatchListNode *nj = vaccineBatchList_get(*list, j);
                tVaccinationBatchListNode *np = vaccineBatchList_get(*list, pivot);
                const char *aj = (nj && nj->e.vaccine && nj->e.vaccine->name) ? nj->e.vaccine->name : "";
                const char *ap = (np && np->e.vaccine && np->e.vaccine->name) ? np->e.vaccine->name : "";
                int s = strcmp(aj, ap);
                // nombre descendente: si aj > ap, va antes (consideramos "menor")
                cmp = (s == 0) ? 0 : (s > 0 ? -1 : 1);
            }

            if (cmp <= 0) { // <= mantiene estabilidad relativa respecto al pivot
                i++;
                vaccineBatchList_swap(list, i, j);
            }
        }
        vaccineBatchList_swap(list, i + 1, high);
        int pi = i + 1;

        vaccineBatchList_quickSortRecursive(list, low, pi - 1);
        vaccineBatchList_quickSortRecursive(list, pi + 1, high);
    }
}

// Entry point that calls the recursive quicksort
tError vaccineBatchList_quicksort(tVaccinationBatchList *queue) {
    if (queue == NULL) return ERR_INVALID;
    if (queue->size < 2) return OK;
    vaccineBatchList_quickSortRecursive(queue, 0, (int)queue->size - 1);
    return OK;
}

// Swap two elements in the list
tError vaccineBatchList_swap(tVaccinationBatchList* list, int index_dst, int index_src) {

    assert(list != NULL);
    assert(index_dst >= 0);
    assert(index_src >= 0);
    assert(index_dst < list->size);
    assert(index_src < list->size);

    tError err;
    tVaccinationBatchListNode * node_src, *node_dst;
    tVaccineBatch tmp;

    if(index_dst == index_src) {
        return ERR_INVALID_INDEX;
    }

    tmp.vaccine =  NULL;
    tmp.lotID = 0;
    tmp.quantity = 0;

    node_src = vaccineBatchList_get(*list, index_src);
    node_dst = vaccineBatchList_get(*list, index_dst);

    err = vaccinationBatch_cpy(&tmp, &node_src->e);
    if(err == OK) {
        vaccinationBatch_free(&node_src->e);
        err = vaccinationBatch_cpy(&node_src->e, &node_dst->e);
        if(err == OK) {
            vaccinationBatch_free(&node_dst->e);
            vaccinationBatch_cpy(&node_dst->e, &tmp);
        }
    }

    vaccinationBatch_free(&tmp);

    return err;
}

// Gets lotID from given position, -1 if out of bounds
int vaccineBatchList_getlotID(tVaccinationBatchList list, int index) {

    int i, lotID;
    tVaccinationBatchListNode *prev;

    // Loop until find index position
    i = 0;
    prev = list.first;
    while(i < index && (prev != NULL)) {
        prev = prev->next;
        i++;
    }

    if(prev == NULL) {
        lotID = -1 ;
    } else {
        lotID = prev->e.lotID;
    }

    return lotID;
}

// Helper function - Print a queue in the console - use for debugging
void vaccineBatchList_print(tVaccinationBatchList list) {
    tVaccinationBatchListNode *nodePtr = list.first;;


    while(nodePtr != NULL) {

        printf("%s) lotId %d quantity %d \n", nodePtr->e.vaccine->name, nodePtr->e.lotID, nodePtr->e.quantity);
        nodePtr = nodePtr->next;

    }

    printf("\n");
}

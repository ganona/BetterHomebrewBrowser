#include "main.hpp"
#include "queue.hpp"
#include "bhbb_dl.h"

Queue::Queue()
{
    head = NULL;
    tail = NULL;
    num = 0;
}

void Queue::clear()
{
    if(head == NULL) return;

    qnode *temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }

    num = 0;
}

void Queue::enqueue(bhbbPacket *data, int dataSize)
{
    print("Adding: %s to queue\n", data->name);
    qnode *tmp = new qnode;
    sce_paf_memset(&tmp->data, 0, sizeof(tmp->data));
    sce_paf_memcpy(&tmp->data, data, dataSize);
    tmp->next = NULL;

    if (head == NULL)
    {
        head = tmp;
        tail = tmp;
    }
    else
    {
        tail->next = tmp;
        tail = tail->next;
    }
    num ++;
    print("Done!\n");
}

bhbbPacket *Queue::Find(const char *name)
{
    qnode *n = head;
    while(n != NULL)
    {
        if(sce_paf_strcmp(name, n->data.name) == 0) return &n->data;
        n = n->next;
    }
    return NULL;
}

void Queue::remove(const char *url)
{
    //Check head isn't NULL
    if (head == NULL)
        return;

    //First, handle the case where we free the head
    if (sce_paf_strcmp(head->data.url, url) == 0)
    {
        qnode *nodeToDelete = head;
        head = head->next;
        free(nodeToDelete);
        return;
    }

    //Bail out if the head is the only node
    if (head->next == NULL)
        return;

    //Else, try to locate node we're asked to remove
    qnode **pCurrentNodeNext = &head; //This points to the current node's `next` field (or to pHead)
    while (1)
    {
        if (sce_paf_strcmp((*pCurrentNodeNext)->data.url, url) == 0) //pCurrentNodeNext points to the pointer that points to the node we need to delete
            break;

        //If the next node's next is NULL, we reached the end of the list. Bail out.
        if ((*pCurrentNodeNext)->next == NULL)
            return;

        pCurrentNodeNext = &(*pCurrentNodeNext)->next;
    }
    qnode *nodeToDelete = *pCurrentNodeNext;
    *pCurrentNodeNext = (*pCurrentNodeNext)->next;
    free(nodeToDelete);

    num --;
}

void Queue::dequeue()
{    
    //Check head isn't NULL
    if (head == NULL)
        return;
    
    qnode *nodeToDelete = head;
    head = head->next;
    free(nodeToDelete);

    num --;
    return;
}
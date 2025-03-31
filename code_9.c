// Just a simple linked list implementation


#include <stdio.h>
#include <stdlib.h>

struct Node{
    int data;
   struct Node* link;
};

struct Node* createNode(int data){
   struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->link = NULL;
    return newNode;
}

void insert(struct Node** head, int data){
   struct Node* tempNode = *head;
    while(tempNode->link != NULL){
        tempNode = tempNode->link; // Get to the end;
    }
    tempNode->link = createNode(data);
}

void printList(struct Node* head){
   struct Node* temp = head;
    while (temp != NULL)
    {
        printf("[%d]->", temp->data);
        temp = temp->link;
    }
    printf("[NULL]\n");
    
}
void delete(struct Node* head){
   struct Node* temp = head;
    while (temp->link != NULL)
    {
        temp = temp->link;
    }
    printf("LAST NODE REACHED %p", temp->link->data);
}

int main(int argc, char const *argv[])
{
    printf("Linked List Example\n");
   struct Node* headNode = createNode(22);
    insert(&headNode, 64);
    insert(&headNode, 645);
    insert(&headNode, 45);
    insert(&headNode, 124);
    printList(headNode);
    delete(headNode);
    printList(headNode);

    return 0;
}

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* createNode(const char *ruleIdentifier, const char *production)
{
    Node *newNode=(Node *)malloc(sizeof(Node));
    newNode->ruleIdentifier=strdup(ruleIdentifier);
    newNode->productions=strdup(production);
    newNode->next=NULL;
    return newNode;
}

Node* findNode(Node *head, const char *ruleIdentifier)
{
    Node *current= head;
    while(current!=NULL)
    {
        if(strcmp(current->ruleIdentifier,ruleIdentifier)==0)
        {
            return current;
        }
        current=current->next;
    }
     return NULL;   
}

void appendProduction(Node *node,const char *production)
{
    size_t newSize=strlen(node->production)+strlen(production)+4;
    node->production=(char *)realloc(node->production,newSize);

    strcat(node->productions," | ");
    strcat(node->production,production);
}

void appendOrUpdateNode(Node **head, const char *ruleIdentifier, const char *production)
{
    Node *existingNode=findNode(*head,ruleIdentifier);
    if(existingNode!=NULL) {
        appendProduction(existingNode,production);
    } else
        {
        Node *newNode=createNode(ruleIdentifier,production);
        if(*head==NULL) {
            *head=newNode;
    } else
        {
            Node *temp=*head;
            while(temp->next!=NULL) {
                temp=temp->next;
            }
            temp->next=newNode;
         }
    }
}

void freeLinkedList(Node *head)
{
    Node *current=head;
    Node *nextNode;
    while(current!=NULL)
        {
            nextNode=current->next;
            free(current->ruleIdentifier);
            free(current->production);
            free(current);
            current=nextNode;
        }
}

void splitLine(const char *line,char *ruleIdentifier,char *production)
{
    const char *delimiter=strstr(line,"->");
    if(delimiter!=NULL)
    {
        strcpy(ruleIdentifier,line,delimiter-line);
        ruleIdentifier[delimiter-line]='\0';
        strcpy(production,delimiter+2);
    }
}

Node* createLinkedList(FILE *file)
{
    Node *head=NULL;
    char line[MAX_LINE_LENGTH];
    char ruleIdentifier[MAX_LINE_LENGTH];
    char production[MAX_LINE_LENGTH];

    while(fgets(line,sizeof(line),file))
        {
            line[strcspn(line,"\n")]='\0');
            splitLine(line,ruleIdentifier,production);
            appendOrUpdateNode(&head,ruleIdentifier,production);
        }
    return head;
}

void printList(Node *head)
{
    Node *curren=head;
    while(current!=NULL)
        {
            printf("%s -> %s\n",current->ruleIdentifier,current->production);
            current=current->next;
        }
}

void removeRecursion(Node *head)
{
    Node *current=head;
    Node *finalNode;
    printf("\n");
    while(current)
    {
        char *recursiveProd=strstr(current->productions,current->ruleIdentifier);
        if (recursiveProd)
            {
            char auxiliaryRecProd[MAX_LINE_LENGTH];
            auxiliaryRecProd[0]='{';
            auxiliaryRecProd[1]=*(recursiveProd-1);
            auxiliaryRecProd[2]='}';
            auxiliaryRecProd[3]='\0';

            char auxiliaryProd[MAX_LINE_LENGTH];
            int prodCounter=0;
            strcpy(auxiliaryProd,current->productions);
            for(int i=0;i<strlen(auxiliaryProd);i++)
                if (auxiliaryProd[i]=='|')
                    prodCounter++;
            if(prodCounter==1)
            {
                char *otherAuxProduction=strchr(auxiliaryProd,'|');
                otherAuxProduction++;
                char otherProduction[MAX_LINE_LENGTH];
                strcpy(otherProduction,otherAuxProduction);
                strcat(auxiliaryRecProd,otherProduction);
            }
            size_t newRecLength=strlen(auxiliaryRecProd)+1;
            char *reallocRecProd=malloc(newRecLength);
            if(reallocRecProd)
            {
                strcpy(reallocRecProd,auxiliaryRecProd);
                current->productions=realloc(current->productions,newRecLength);
                if (current->productions)
                    strcpy(current->productions,reallocRecProd);
                free(reallocRecProd);
            }
            }
            else
            {
            char auxiliaryNonRecProd[MAX_LINE_LENGTH]="";
            strcat(auxiliaryNonRecProd,current->productions);

            size_t newNonRecLength=strlen(auxiliaryNonRecProd)+1;
            char *reallocNonRecProd=malloc(newNonRecLength);
            if (reallocNonRecProd)
            {
                strcpy(reallocNonRecProd,auxiliaryNonRecProd);
                current->productions=realloc(current->productions,newNonRecLength);
                if (current->productions)
                    strcpy(current->productions,reallocNonRecProd);
                free(reallocNonRecProd);
            }
        }
        printf("%s -> %s\n",current->ruleIdentifier,current->productions);
        current=current->next;
    }
    printf("\n");
}

void removeSecondRecursion(Node *head)
{
    if (head==NULL||head->next==NULL)
        return;

    Node *current=head;
    Node *previous=NULL;
    while (current->next!=NULL)
    {
        previous=current;
        current=current->next;
    }
    Node *temp=head;
    while (temp!=NULL)
    {
        char *tempProductions=temp->productions;
        for (int i=0;i<strlen(tempProductions);i++)
        {
            if (tempProductions[i]==*current->ruleIdentifier)
            {
                char newProductions[MAX_LINE_LENGTH]="";
                strncat(newProductions,tempProductions,i);
                strcat(newProductions,current->productions);
                strcat(newProductions,tempProductions+i+ 1);
                free(temp->productions);
                temp->productions=strdup(newProductions);
                i=-1;
            }
        }
        temp=temp->next;
    }

    char *currentProductions=current->productions;
    char *previousProductions=previous->productions;

    for (int i=0;i<strlen(previousProductions);i++)
    {
        if (previousProductions[i]==*current->ruleIdentifier)
        {
            char newProductions[MAX_LINE_LENGTH]="";
            strncat(newProductions,previousProductions,i);
            strcat(newProductions,currentProductions);
            strcat(newProductions,previousProductions+i+1);
            free(previous->productions);
            previous->productions=strdup(newProductions);
            i=-1;
        }
    }
    Node *tempPrint=head;
    while(tempPrint!=NULL)
    {
        printf("%s -> %s\n",tempPrint->ruleIdentifier,tempPrint->productions);
        tempPrint=tempPrint->next;
    }
}

void printRegex(Node *head)
{
    if (head==NULL)
        return;

    char *firstProductions=head->productions;
    char modifiedProductions[MAX_LINE_LENGTH]="";
    int j = 0;

    for (int i=0;i<strlen(firstProductions);i++)
    {
        if (firstProductions[i]=='{')
        {
            i++;
            if (firstProductions[i]!= '\0')
            {
                modifiedProductions[j++]=firstProductions[i];
                modifiedProductions[j++]='*';
            }
        }
        else
        {
            modifiedProductions[j++]=firstProductions[i];
        }
    }
    modifiedProductions[j]='\0';
    free(head->productions);
    head->productions=strdup(modifiedProductions);
    printf("\n%s -> %s\n",head->ruleIdentifier,head->productions);
}


int main() 
{
    FILE *file=fopen("aquiponeselnombredelarchivodetextoconel.txt","r");
    if(FILE==NULL)
    {
        perror("Error opening file");
        return 1;
    }
    Node *head=createLinkedList(file);
    fclose(file);
    printList(head);
    removeRecursion(head);
    removeSecondRecursion(head);
    printRegex(head);
    freeLinkedList(head);
    return 0;
}

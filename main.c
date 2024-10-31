#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void firstCycle(Node *head)
{
    Node *current = head;
    printf("\n");
    printf("First Cycle\n");
    while (current)
    {
        char *token;
        char *productionsCopy = strdup(current->productions);
        char *similarProductions = NULL;
        char *otherProductions = NULL;
        int similarCount = 0;

        token = strtok(productionsCopy, " | ");
        while (token != NULL)
        {
            if (strstr(token,current->ruleIdentifier)!=NULL&&islower(token[0]))
            {
                if (similarProductions==NULL)
                {
                    similarProductions=strdup(token);
                }
                similarCount++;
            }
            else
            {
                if(otherProductions==NULL)
                    otherProductions=strdup(token);
                else
                {
                    size_t newSize=strlen(otherProductions)+strlen(token)+4;
                    otherProductions=realloc(otherProductions,newSize);
                    strcat(otherProductions, " | ");
                    strcat(otherProductions, token);
                }
            }
            token=strtok(NULL," | ");
        }

        printf("%s -> ",current->ruleIdentifier);
        if (similarCount>0&&similarProductions!=NULL)
            printf("{%s}",similarProductions);
        if (otherProductions!=NULL)
            printf(" %s",otherProductions);
        printf("\n");
        free(productionsCopy);
        free(similarProductions);
        free(otherProductions);
        current=current->next;
    }
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
    firstCycle(head);
    freeLinkedList(head);
    return 0;
}

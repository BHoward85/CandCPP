// Brad Howard
// Lab 3 - LinkedList

// LinkedList Util C file

#include "listUtils.h"
#include "requiredIncludes.h"

Node * buildNode(FILE * fin, void * (*buildData)(FILE * in))
{
	if(fin == NULL)
	{
		perror("File input error : build node : ");
		exit(-99);
	}
	
	Node *newNode;
	newNode = (Node *)malloc(sizeof(Node));
	newNode->data = buildData(fin);
	newNode->next = NULL;
	newNode->prev = NULL;
	
	if(newNode == NULL)
	{
		perror("Null Node Error : build node : ");
		exit(-99);
	}
	
	return newNode;
}

Node * buildHistNodeByString(char * str, int num, void * (*buildData)(char * com, int n))
{
	if(str == NULL)
	{
		perror("NULL pointer error : build node : ");
		exit(-99);
	}
	
	Node *newNode;
	newNode = (Node *)malloc(sizeof(Node));
	newNode->data = buildData(str, num);
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

Node * buildAliasNodeByString(char * name, char * com, void * (*buildData)(char * name, char * com))
{
	if(name == NULL || com == NULL)
	{
		perror("NULL pointer error : build node : ");
		exit(-99);
	}
	
	Node *newNode;
	newNode = (Node *)malloc(sizeof(Node));
	newNode->data = buildData(name, com);
	//printf("in Alias: %s - %s\n", name, com);
	Alias temp = *((Alias *)newNode->data);
	//printf("in Alias: in node : %s - %s\n", temp.aliasName, temp.command);
	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

/*Node * buildNode_Type(void * passedIn)
{
	Node *newNode = malloc(sizeof(Node));
	
	newNode->prev = NULL;
	newNode->next = NULL;
	
	if(sizeof(passedIn) == sizeof(Word))
	{
		newNode->data = buildTypeWord_Prompt(stdin);
	}
	else if(sizeof(passedIn) == sizeof(Movie))
	{
		newNode->data = buildTypeMovie_Prompt(stdin);
	}
	
	return newNode;
}*/

/*void sort(LinkedList * theList, int (*compare)(const void *, const void *))
{
	if(theList == NULL)
	{
		perror("Null list error : sort : ");
		exit(-99);
	}
		
	if(theList->size == 0 || theList->size == 1)
		return;
	
	Node *curr, *iSmall, *post;
	void *temp;

	for(post = theList->head->next; post != NULL; post = post->next)
	{
		iSmall = post;

		for(curr = post->next; curr != NULL; curr = curr->next)
		{
			if(compare(iSmall->data, curr->data) > 0)
				iSmall = curr;
		}

		temp = post->data;
		post->data = iSmall->data;
		iSmall->data = temp;
	}
}*/

void buildListTotal(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in))
{
	if(myList == NULL)
	{
		perror("Null list error : build list total : ");
		exit(-99);
	}
		
	if(total <= 0)
	{
		perror("Total less than 1 error : build list total : ");
		exit(-99);
	}
	
	if(fin == NULL)
	{
		perror("Null file pointer error : build list total : ");
		exit(-99);
	}
	
	int tdex;	
	//printf("~- IN Build total list `%d` -~\n", total);
	for(tdex = 0; tdex < total; tdex++)
	{
		Node *newNode = (Node *)buildNode(fin, buildData);
		addLast(myList, newNode);
	}
}

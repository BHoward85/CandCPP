// Brad Howard
// Lab 7 - LinkedList

// LinkedList C file

#include "linkedList.h"
#include "requiredIncludes.h"

LinkedList * linkedList(int toSlimit, int toFlimit)
{
	LinkedList *list;
	
	list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = (Node *)malloc(sizeof(Node));
	list->tail = (Node *)malloc(sizeof(Node));

	list->head->data = NULL;
	list->head->prev = NULL;
	list->head->next = NULL;

	list->tail->data = NULL;
	list->tail->prev = list->head;
	list->tail->next = NULL;

	list->size = 0;
	list->toFile = toFlimit;
	list->toScreen = toSlimit;
	
	return list;
}

// Use for hist and alias
void addLast(LinkedList * theList, Node * nn)
{
	//printf("~- In Add Last -~\n");
	if(theList == NULL)
	{
		perror("Null list error : add last : ");
		exit(-99);
	}
	
	if(nn == NULL)
	{
		perror("Null node error : add last : ");
		exit(-99);
	}
	Node *curr;
	if(theList->head->next == NULL)
		curr = theList->head;
	else
		curr = theList->head->next;
	
	while(curr->next != NULL)
	{
		curr = curr->next;
	}
	
	curr->next = nn;
	nn->prev = curr;
	nn->next = NULL;
	theList->tail->prev = nn;
	
	theList->size++;
}

void addFirst(LinkedList * theList, Node * nn)
{
	if(theList == NULL)
	{
		perror("Null list error : add first : ");
		exit(-99);
	}
	
	if(nn == NULL)
	{
		perror("Null node error : add first : ");
		exit(-99);
	}
	
	if(theList->size == 0)
	{
		//theList->head->next = (Node *)malloc(sizeof(Node));
		nn->next = NULL;
		nn->prev = theList->head;

		theList->head->next = nn;
	}
	else
	{
		// place the new node into the list
		nn->next = theList->head->next;
		nn->prev = theList->head;
	
		// set head.next.prev to new node
		theList->head->next->prev = nn;

		// set head.next to the new node
		theList->head->next = nn;
	}
	theList->size++;
	//printf("Add a node, Size @ %d\n", theList->size);
}

void removeFirst(LinkedList * theList, void (*removeData)(void *))
{
	if(theList == NULL)
	{
		perror("Null list error : remove first : ");
		exit(-99);
	}
	
	if(theList->size > 0)
	{
		Node *nextNode;

		// gets the node after the first node
		nextNode = theList->head->next->next;
		// call removeData
		removeData(theList->head->next->data);
		free(theList->head->next);

		nextNode->prev = theList->head;
		theList->head->next = nextNode;

		theList->size--;
	}
}

void removeLast(LinkedList * theList, void (*removeData)(void *))
{
	if(theList == NULL)
	{
		perror("Null list error : remove last : ");
		exit(-99);
	}
	
	if(theList->size > 0)
	{
		Node *curr = theList->head->next;
	
		// move to the end of the list
		while(curr->next != NULL)
		{
			curr = curr->next;
		}

		removeData(curr->data);
	
		//set the second to last node's next to NULL
		curr->prev->next = NULL;
		theList->tail->prev = curr->prev;
	
		theList->size--;

		// set current node to NULL
		free(curr);
		curr = NULL;		
	}

	//printf("Removed a node, Size @ %d\n", theList->size);
}

void removeItem(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *))
{
	if(theList == NULL)
	{
		perror("Null list error : remove item : ");
		exit(-99);
	}
	
	if(nn == NULL)
	{
		perror("Null node error : remove item : ");
		exit(-99);
	}
	
	if(theList->size > 0)
	{
		Node *curr = theList->head->next;
	
		while(curr->next != NULL)
		{
			if(compare(nn->data, curr->data) == 0)
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			
				curr->prev = NULL;
				curr->next = NULL;
			
				removeData(curr->data);
				removeData(nn->data);
				theList->size--;
				free(curr);
				free(nn);
				return;
			}
			else
			{
				curr = curr->next;
			}
		}
		if(curr->next == NULL)
		{
			if(compare(nn->data, curr->data) == 0)
			{
				removeLast(theList, removeData);
			}
		}
		removeData(nn->data);
		free(nn);
	}
}

void clearList(LinkedList * theList, void (*removeData)(void *))
{
	//printf("IN CLEAR LIST\n");	
	if(theList == NULL)
		return;

	while(theList->size > 0)
	{
		removeLast(theList, removeData);
	}
	
	theList->toFile = 0;
	theList->toScreen = 0;
	theList->size = 0;


	free(theList->head);
	theList->head = NULL;

	free(theList->tail);
	theList->tail = NULL;
}

void printList(const LinkedList * theList, void (*convertData)(void *))
{
	if(theList == NULL)
	{
		printf("Empty List\n");
		return;
	}
	
	Node *curr = theList->head->next;
	//printf("Size of the list: %d\n", theList->size);
	while(curr != NULL)
	{
		convertData(curr->data);
		curr = curr->next;
	}
}

void printSubList(const LinkedList * theList, void (*convertData)(void *))
{
	if(theList == NULL)
	{
		printf("Empty List\n");
		return;
	}
	
	int n;
	Node *curr = theList->tail->prev;

	for(n = 0; n < theList->toScreen - 1 && n < theList->size; n++)
	{
		curr = curr->prev;
	}

	while(curr != NULL)
	{
		convertData(curr->data);
		curr = curr->next;
	}
}

void writeSubList(FILE * fout, const LinkedList * theList)
{
	if(theList == NULL)
	{
		printf("Empty List\n");
		return;
	}

	if(fout == NULL)
	{
		perror("NULL file pointer error : write sub list :\n");
		exit(-99);
	}
	
	int n;
	Node *curr = theList->tail->prev;

	for(n = 0; n < theList->toFile - 1 && n < theList->size; n++)
	{
		//printf("%d\n", n);
		curr = curr->prev;
	}

	//printf("Before While\n");
	while(curr != NULL)
	{
		//printf("in While\n");
		if(curr->data != NULL)
		{
			HistCommand temp = *((HistCommand *)curr->data);
			//printf("after cast\n");
			//printf("%s\n", temp.command);
			fprintf(fout, "%s\n", temp.command);
		}
		curr = curr->next;
	}
}

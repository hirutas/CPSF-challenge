#include <stdlib.h>
#include <string.h>

struct linkedList {
	char str[32];
	struct linkedList *next;
};

// init
struct linkedList* initLL(char *str){
	struct linkedList *data;
	
	data = (struct linkedList*)malloc(sizeof(struct linkedList));
	if(data == NULL){
		puts("Malloc error");
		exit(EXIT_FAILURE);
	}
	memcpy(data->str, str, sizeof(data->str)-1);
	data->next = NULL;
	
	return data;
}

// add
struct linkedList* addLL(struct linkedList *head, char *str){
	struct linkedList *new, *before, *last;

	// insert
	new = initLL(str);
	new->next = NULL;
	// forward pointer to the end of the list
	for(before = head; before != NULL; before = before->next){
		last = before;
	}
	last->next = new;

	return NULL;
}


// delete
struct linkedList* deleteLL(struct linkedList *head, int index){
	struct linkedList *before, *tmp;
	int i;
	
	before = head;
	i = 0;
	while(before != NULL){
		if(i == index){
			tmp = before->next->next; // save pointer
			free(before->next);
			before->next = tmp;
			break;
		}else{
			before = before->next;
		}
		i++;
	}

	return head;
}


// get
char* getLL(struct linkedList *head, int index){
	struct linkedList *before;
	int i;
	
	before = head;
	i = 0;
	while(before != NULL){
		if(i == index){
			return before->str;
			break;
		}else{
			before = before->next;
		}
		i++;
	}

	return NULL;
}


// size
int sizeLL(struct linkedList *head){
	struct linkedList *before;
	int i = 0;

	for(before = head; before != NULL; before = before->next){
		i++;
	}
	
	return i;
}

// pretty print
void printLL(struct linkedList *head){
	struct linkedList *data;
	int i;
	
	printf("[idx]\taddress\t\tstr\tnext\n");
	for(data = head, i = 0; data != NULL; data = data->next){
  	printf("[%d]\t%p\t%s\t%p\n", i, data, data->str, data->next);
		i++;
	}
}

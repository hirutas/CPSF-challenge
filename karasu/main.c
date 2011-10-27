#include <stdio.h>
#include "linkedList.h"
#include "arrayList.h"

/**
 * Demo of Linked list
 * @author Shinya Hiruta
 */
int main(void){
	int i, j;
	struct linkedList *head, *data, *next;
	
	// data
	char *keywords[10] = {
		"aaa",
		"bbb",
		"ccc",
		"ddd",
		"eee",
		"fff",
		"ggg",
		"hhh",
		"iii",
		"jjj"
	};

	head = initLL("head");
	data = head;
	data->next = next;
	
	// initialize linkedList
	for(i = 0; i < 10; i++){
		next = initLL(keywords[i]);
		data->next = next;
		data = next;
	}
	printf("%s", "* initialized\n");
	printLL(head);
	printf("size: %d\n", sizeLL(head));
	
	// add
	addLL(head, "hoge");

	printf("%s", "* added\n");
	printLL(head);
	printf("size: %d\n", sizeLL(head));

	// delete
	deleteLL(head, 1);

	printf("%s", "* deleted\n");
	printLL(head);

	// get
	printf("%s", "* get\n");
	printf("%s\n", getLL(head, 0));

	// size
	printf("size: %d\n", sizeLL(head));

	return 0;
}
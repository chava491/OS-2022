#include <stdio.h>

int main()
{
	int *node;
	int length = 10;
	
	node = &length;
	printf("node *: %d length: %d normal add: %d \r\n", node, length, (node + length/sizeof(node)));
	//can also use uint or ulong for pointer arithemtic
	return 0;
}

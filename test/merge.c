#include <stdio.h>

int main()
{
	char high = 0x84;
	char low = 0x20;
	char c;
	printf("%d\n", high<<8|low);
	printf("%d\n", (high<<8&0xFF00)|low);
	printf("%d\n", 0x8420);
	printf("%d\n", -120&0xFF);
	c = -120&0xFF;
	printf("%d\n", c);


	return 0;
}

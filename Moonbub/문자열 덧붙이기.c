#include <stdio.h>
#include <string.h>		// ���ڿ� ���̺귯��

int main(void)
{
	char str[30] = "score : ";
	char str2[30] = "300";
	
	strcat(str, str2);
	printf("%s", str);

	int score = 300;

	return 0;
}
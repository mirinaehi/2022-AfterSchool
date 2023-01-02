#include <stdio.h>

// _a와 a, _b와 b는 전~혀 상관없는 변수임. 따라서 a, b값은 바뀌지 않음.
void swap(int _a, int _b)
{
	int temp;
	temp = _a;
	_a = _b;
	_b = temp;
}

int main(void)
{
	int a = 3;
	int b = 5;

	printf("a:%d b:%d \n", a, b);

	swap(a, b);

	printf("a:%d b:%d \n", a, b);
}
#include <stdio.h>

// _a, _b�� �ּҰ�, *_a, *_b�� ����Ű�� ������ ������(int)
void swap(int* _a, int* _b)
{
	int temp;
	temp = *_a;
	*_a = *_b;
	*_b = temp;
}

int main(void)
{
	int a = 3;
	int b = 5;

	printf("a:%d b:%d \n", a, b);

	swap(&a, &b);	// _a���� a�� �ּҰ���,  _b���� b�� �ּҰ��� �Ѱ��ش�

	printf("a:%d b:%d \n", a, b);
}
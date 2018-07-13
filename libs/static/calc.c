#include <stdio.h>
#include "calc.h"

//int-представление символов *,+,-,/ соответственно
#define MULTIPLY 42
#define PLUS 43
#define MINUS 45
#define DEVIDE 47

int main(int argc, char const *argv[])
{
	int x,y;
	char operation;

	printf("Введите знак операции: [ * | + | - | / ]\n");
	scanf("%s",&operation); // breaking bad

	printf("Введите первое и второе число:\n");
	scanf("%d",&x);
	scanf("%d",&y);
	//printf("Введите знак операции: [ * | + | - | / ]\n");
	//scanf("%s",&operation); // breaking bad
	int operation_is = (int)operation;
	int answer;
	switch(operation_is){
		case MULTIPLY:
			answer = multiply(x,y);
			break;

		case PLUS:
			answer = plus(x,y);
			break;
		case MINUS:
			answer = minus(x,y);
			break;
		case DEVIDE:
			answer = devide(x,y);
			break;
	}
	printf("answer = %d\n", answer);
	return 0;

}
//проверка на x>y

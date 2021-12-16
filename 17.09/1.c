#include <stdio.h>

struct ts{
	int a;
	char* b;
};

int f (int i, int j);

int main(int argc, char** argv){
	int i = 0, j = 3;
	struct ts a = {3, argv[0]};
	struct ts* b = &a;
	i = 5 + j;

	for(; i < j; i--){
		j += f(i, j);
		printf("%d\n", i);
	}
}

int f (int i, int j){
	int t;
	t = i + j;
	return t;
}
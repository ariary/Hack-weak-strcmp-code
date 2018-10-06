#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char user_pwd[20];

	printf("Please enter the password:\n");
	scanf("%s", user_pwd);

	if (strcmp ("hackm3",user_pwd)==0)
	{
		printf("Congratulations you are log in\n");
	}
		else
			printf("You failed to log in. Try again :-(\n");
	return 0;
}

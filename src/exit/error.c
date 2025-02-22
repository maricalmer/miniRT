#include <minirt.h>

void	print_error(int errno)
{
	if (errno == 1)
		printf("Error\nWrong args\n");
	else if (errno == 2)
		printf("Error\nWrong input file\n");
	else if (errno == 3)
		perror("Error\nError opening file\n");
	else if (errno == 4)
		perror("Error\nMemory allocation failed\n");
	else if (errno == 5)
		perror("Error\nWrong input elements\n");
}
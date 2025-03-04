#include <minirt.h>

void	print_error(int errnum)
{
	if (errnum == 1)
		printf("Error\nWrong args\n");
	else if (errnum == 2)
		printf("Error\nWrong input file\n");
	else if (errnum == 3)
		perror("Error\n");
	else if (errnum == 4)
		perror("Error\n");
	else if (errnum == 5)
		printf("Error\nWrong set of element types\n");
	else if (errnum == 6)
		printf("Error\nWrong settings for ambient lighting element\n");
	else if (errnum == 7)
		printf("Error\nWrong settings for camera element\n");
	else if (errnum == 8)
		printf("Error\nWrong settings for light element\n");
	else if (errnum == 9)
		printf("Error\nWrong settings for sphere element\n");
	else if (errnum == 10)
		printf("Error\nWrong settings for plane element\n");
	else if (errnum == 11)
		printf("Error\nWrong settings for cylinder element\n");
	else if (errnum == 12)
		printf("Error\nWrong settings for tri element\n");
}
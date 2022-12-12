#include "../include/utils.h"
#include <stdio.h>

int main()
{
	bool ok;

	printf("%d\n", ft_atoi("+0", &ok));
	printf("%d\n", ok);
}

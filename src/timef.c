#include "timef.h"

long long milliseconds()
{
	return (clock() / (CLOCKS_PER_SEC / 1000ll));
}

#include "../../utils/virual.h"

class c_localize
{
public:
	const char* find_key(const char* key) 
	{
		return vfunc(this, const char*, 17, key);
	}
};
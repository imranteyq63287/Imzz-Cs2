#pragma once

#include "vector.h"

class matrix2x4_t 
{
public:
	vec3_t get_origin() 
	{
		return vec3_t(_11, _12, _13);
	}

	void set_origin(int index, vec3_t val) 
	{
		this[index]._11 = val.x;
		this[index]._12 = val.y;
		this[index]._13 = val.z;
	}

	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
		};
	};
};
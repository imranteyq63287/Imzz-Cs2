#pragma once

#include "buffer.h"
#include "../utils/typedefs/c_strong_handle.h"
#include "../utils/virual.h"

class i_res_manager 
{
public:
    void* PreCache(const char* name)
    {
        CBufferString names = CBufferString(name);

        return vmt::call_virtual<void*>(this, 48, &names, "");
    }
};
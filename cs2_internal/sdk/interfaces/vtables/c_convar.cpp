#include "c_convar.h"
#include <cstdint>
#include "../../../dependencies/hash.h"

var_iterator_t c_cvar::get_first_var_iterator()
{
    var_iterator_t it = -1;

    vfunc(this, void, 12, &it);

    return it;
}

var_iterator_t c_cvar::get_next_var(var_iterator_t previous)
{
    var_iterator_t it = previous;

    vfunc(this, void, 13, &it, previous);

    return it;
}

convar_t* c_cvar::get_by_index(var_iterator_t idx)
{
    return vfunc(this, convar_t*, 41, idx);
}

convar_t* c_cvar::get_by_name(const char* name) 
{
    for (std::uint16_t nCurrent = 0; nCurrent != static_cast<std::uint16_t>(-1); nCurrent = pConVars[nCurrent].nNext)
    {
        convar_t* pConVar = pConVars[nCurrent].pData;

        if (pConVar == nullptr)
            continue;

        if (fnv1a::hash_64(pConVar->m_name) == fnv1a::hash_64(name))
            return pConVar;
    }

    return nullptr;
}

void c_cvar::unlock_hidden_vars() 
{
    var_iterator_t it = get_first_var_iterator();

    while (true) 
    {
        convar_t* var = get_by_index(it);

        if (!var)
            break;

        var->m_flags &= ~(FCVAR_HIDDEN | FCVAR_DEVELOPMENTONLY);

        it = get_next_var(it);
    }
}
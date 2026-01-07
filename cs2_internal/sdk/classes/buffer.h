#pragma once

#include "../utils/c_utils.hpp"

inline const char* (__fastcall* Insertz)(void*, int, const char*, int, bool);
inline void(__fastcall* Purgez)(void*, int);
inline const char* (__fastcall* SetExtensionz)(void*, const char*, bool);

enum EStringConvertErrorPolicy
{
    _STRINGCONVERTFLAG_SKIP = 1,
    _STRINGCONVERTFLAG_FAIL = 2,
    _STRINGCONVERTFLAG_ASSERT = 4,

    STRINGCONVERT_REPLACE = 0,
    STRINGCONVERT_SKIP = _STRINGCONVERTFLAG_SKIP,
    STRINGCONVERT_FAIL = _STRINGCONVERTFLAG_FAIL,

    STRINGCONVERT_ASSERT_REPLACE =
    _STRINGCONVERTFLAG_ASSERT + STRINGCONVERT_REPLACE,
    STRINGCONVERT_ASSERT_SKIP = _STRINGCONVERTFLAG_ASSERT + STRINGCONVERT_SKIP,
    STRINGCONVERT_ASSERT_FAIL = _STRINGCONVERTFLAG_ASSERT + STRINGCONVERT_FAIL,
};

template <size_t max_size = 200U>
class CBufferString
{
public:
    enum EAllocationOption_t 
    {
        UNK1 = -1,
        UNK2 = 0,
        UNK3 = (1 << 1),
        UNK4 = (1 << 8),
        UNK5 = (1 << 9)
    };

    enum EAllocationFlags_t
    {
        LENGTH_MASK = (1 << 30) - 1,
        FLAGS_MASK = ~LENGTH_MASK,
        OVERFLOWED_MARKER = (1 << 30),
        FREE_HEAP_MARKER = (1 << 31),
        STACK_ALLOCATED_MARKER = (1 << 30),
        ALLOW_HEAP_ALLOCATION = (1 << 31)
    };

public:
    CBufferString(bool bAllowHeapAllocation = true) :
        m_nLength(0),
        m_nAllocatedSize((bAllowHeapAllocation* ALLOW_HEAP_ALLOCATION) | STACK_ALLOCATED_MARKER | sizeof(m_szString)),
        m_pString(nullptr) {
    }

    CBufferString(const char* pString, bool bAllowHeapAllocation = true) :
        CBufferString(bAllowHeapAllocation) {
        Insert(0, pString);
    }

public:
    CBufferString(const CBufferString& other) : CBufferString() { *this = other; }

    ~CBufferString() { Purge(); }

    void SetHeapAllocationState(bool state)
    {
        if (state)
            m_nAllocatedSize |= ALLOW_HEAP_ALLOCATION;
        else
            m_nAllocatedSize &= ~ALLOW_HEAP_ALLOCATION;
    }

    int AllocatedNum() const 
    {
        return m_nAllocatedSize & LENGTH_MASK;
    }

    int Length() const 
    { 
        return m_nLength & LENGTH_MASK; 
    }

    bool CanHeapAllocate() const
    {
        return (m_nAllocatedSize & ALLOW_HEAP_ALLOCATION) != 0;
    }

    bool IsStackAllocated() const 
    { 
        return (m_nAllocatedSize & STACK_ALLOCATED_MARKER) != 0;
    }

    bool ShouldFreeMemory() const
    {
        return (m_nLength & FREE_HEAP_MARKER) != 0; 
    }

    bool IsOverflowed() const 
    {
        return (m_nLength & OVERFLOWED_MARKER) != 0;
    }

    bool IsInputStringUnsafe(const char* pData) const
    {
        return ((void*)pData >= this && (void*)pData < &this[1]) || (!IsAllocationEmpty() && pData >= Base() && pData < (Base() + AllocatedNum()));
    }

    bool IsAllocationEmpty() const 
    { 
        return AllocatedNum() == 0; 
    }

protected:
    char* Base() 
    { 
        return IsStackAllocated() ? m_szString : (!IsAllocationEmpty() ? m_pString : nullptr); 
    }

    const char* Base() const 
    { 
        return const_cast<CBufferString*>(this)->Base(); 
    }

public:
    void Clear()
    {
        if (!IsAllocationEmpty())
            Base()[0] = '\0';

        m_nLength &= ~LENGTH_MASK;
    }

public:
    const char* Insert(int nIndex, const char* pBuf, int nCount = -1, bool bIgnoreAlignment = false)
    {
        if (!Insertz)
        {
            Insertz = reinterpret_cast<decltype(Insertz)>( g_opcodes->export_fn( reinterpret_cast<std::size_t>(g_modules.tier0), "?Insert@CBufferString@@QEAAPEBDHPEBDH_N@Z" ) );

            if (!Insertz)
            {
                std::cout << "[CBufferString] Error: Failed to locate Insert function!" << std::endl;
                return nullptr;
            }
        }

        return Insertz(this, nIndex, pBuf, nCount, bIgnoreAlignment);
    }

    void Purge(int nAllocatedBytesToPreserve = 0)
    {
        if (!Purgez)
        {
            Purgez = reinterpret_cast<decltype(Purgez)>( g_opcodes->export_fn( reinterpret_cast<std::size_t>(g_modules.tier0), "?Purge@CBufferString@@QEAAXH@Z" ));

            if (!Purgez)
            {
                std::cout << "[CBufferString] Error: Failed to locate Purge function!" << std::endl;
                return;
            }
        }

        Purgez(this, nAllocatedBytesToPreserve);
    }

private:
    int m_nLength;
    int m_nAllocatedSize;

    union 
    {
        char* m_pString;
        char m_szString[max_size];
    };
};
    #include <cstdint>

    #ifndef VALVE_PARTICLE_SYSTEM
    #define VALVE_PARTICLE_SYSTEM

    #include "../utils/typedefs/c_strong_handle.h"

    #define CALL_VIRTUAL(retType, idx, ...) \
        vmt::CallVirtual<retType>(idx, __VA_ARGS__)

    namespace vmt
    {
        template <typename T = void*>
        inline T GetVMethod(uint32_t uIndex, void* pClass) 
        {
            if (!pClass)
            {
                return T{};
            }

            void** pVTable = *static_cast<void***>(pClass);

            if (!pVTable) 
            {
                return T{};
            }

            return reinterpret_cast<T>(pVTable[uIndex]);
        }

        template <typename T, typename... Args>
        inline T CallVirtual(uint32_t uIndex, void* pClass, Args... args) 
        {
            auto pFunc = GetVMethod<T(__thiscall*)(void*, Args...)>(uIndex, pClass);
      
            if (!pFunc)
            {
                return T{};
            }

            return pFunc(pClass, args...);
        }
    }  // namespace vmt

    class vec3_t;

    class c_particle_snapshot
    {
    public:
        void draw(int count, void* data)
        {
            CALL_VIRTUAL(void, 1, this, count, data);
        }
    };

    class particle_effect
    {
    public:
        const char* name{};
        char pad[0x30]{};
    };

    class particle_information
    {
    public:
        float time{};
        float width{};
        float unk2{};
    };

    class particle_data
    {
    public:
        vec3_t* positions;
        char pad[0x50]{};
        float* times{};
        void* unk_ptr{};
        char pad2[0x10];
        float* times2{};
        void* unk_ptr2{};
    };

    struct particle_color
    {
        float r;
        float g;
        float b;
    };

    class i_particle_manager
    {
    public:
        void create_snapshot(c_strong_handle<c_particle_snapshot>* out_particle_snapshot)
        {
            CALL_VIRTUAL(void, 43, this, out_particle_snapshot);
        }

        void draw(c_strong_handle<c_particle_snapshot>* particle_snapshot, int count, void* data)
        {
            CALL_VIRTUAL(void, 44, this, particle_snapshot, count, data);
        }
    };

    class i_particle_system
    {
    public:
        void create_effect_index(unsigned int* effect_index, particle_effect* effect_data);
        void create_effect(unsigned int effect_index, int unk, void* clr, int unk2);
        void unk_function(int effect_index, unsigned int unk, const c_strong_handle<c_particle_snapshot>* particle_snapshot);
    };

    #endif
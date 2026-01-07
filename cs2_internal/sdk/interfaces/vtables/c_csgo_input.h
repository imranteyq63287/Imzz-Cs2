#pragma once
#include "../../utils/virual.h"
#include "../../utils/vector.h"

#include "../protos/usercmd.pb.h"
#include "../protos/cs_usercmd.pb.h"

#define	FL_ONGROUND				(1 << 0)
#define FL_DUCKING				(1 << 1)
#define	FL_WATERJUMP			(1 << 3)
#define FL_ONTRAIN				(1 << 4)
#define FL_INRAIN				(1 << 5)
#define FL_FROZEN				(1 << 6)
#define FL_ATCONTROLS			(1 << 7)
#define	FL_CLIENT				(1 << 8)
#define FL_FAKECLIENT			(1 << 9)
#define	FL_INWATER				(1 << 10)
#define FL_HIDEHUD_SCOPE		(1 << 11)

enum e_button
{
    IN_NONE = 0,
    IN_ALL = -1,
    IN_ATTACK = 1,
    IN_JUMP = 2,
    IN_DUCK = 4,
    IN_FORWARD = 8,
    IN_BACK = 16,
    IN_USE = 32,
    IN_TURNLEFT = 128,
    IN_TURNRIGHT = 256,
    IN_MOVELEFT = 512,
    IN_MOVERIGHT = 1024,
    IN_ATTACK2 = 2048,
    IN_RELOAD = 8192,
    IN_SPEED = 65536,
    IN_JOYAUTOSPRINT = 131072,
    IN_FIRST_MOD_SPECIFIC_BIT = 4294967296,
    IN_USEORRELOAD = 4294967296,
    IN_SCORE = 8589934592,
    IN_ZOOM = 17179869184,
    IN_JUMP_THROW_RELEASE = 34359738368,
};

class c_in_button_state
{
    void* __vfptr; //0x0000
public:
    uint64_t button_state; //0x0008
    uint64_t button_state2; //0x0010
    uint64_t button_state3; //0x0018
}; //Size: 0x0020

class c_user_cmd
{
public:
    char pad_valve_shizo[0x10];
    CSGOUserCmdPB pb;
    c_in_button_state buttons;
    char pad_002[0x20];

    std::string get_user_cmd_output()
    {
        void* unk = nullptr;
        return vfunc(this, const char*, 17, &unk);
    }
};

class c_subitck_moves
{
public:
    float when; //0x0000
    float delta; //0x0004
    uint64_t button; //0x0008
    bool pressed; //0x0010
    char pad0011[7]; //0x0011
}; //Size: 0x0018

struct c_cs_input_nessage
{
    int32_t m_frame_tick_count; //0x0000
    float m_frame_tick_fraction; //0x0004
    int32_t m_player_tick_count; //0x0008
    float m_player_tick_fraction; //0x000C
    vec3_t m_view_angles; //0x0010
    vec3_t m_shoot_position; //0x001C
    int32_t m_target_index; //0x0028
    vec3_t m_target_head_position; //0x002C
    vec3_t m_target_abs_origin; //0x0038
    vec3_t m_target_angle; //0x0044
    int32_t m_sv_show_hit_registration; //0x0050
    int32_t m_entry_index_max; //0x0054
    int32_t m_index_idk; //0x0058
};

class c_csgo_input
{
public:
    char pad_0000[592]; //0x0000
    bool block_shot; //0x0250
    bool in_third_person; //0x0251
    char pad_0252[6]; //0x0252

    vec3_t thirdperson_angles; //0x0258
    vec3_t get_view_angles();

    void set_view_angles(vec3_t& view_angles);

    c_user_cmd* get_user_cmd(void* local_controller);
};
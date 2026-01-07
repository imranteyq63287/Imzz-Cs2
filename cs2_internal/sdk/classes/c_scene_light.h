#pragma once

#include "../utils/typedefs/c_handle.h"
#include "../utils/typedefs/c_color.h"

class c_aggregate_scene_object_data
{
private:
    std::byte pad_0000[0x38]; // 0x0
public:
    c_color_rgb color; // 0x38
private:
    std::byte pad_0038[0x9];
};

class c_aggregate_scene_object
{
private:
    std::byte pad_0000[0x120];
public:
    int count; // 0x120
private:
    std::byte pad_0120[0x4];
public:
    c_aggregate_scene_object_data* array; // 0x128
};

class c_material_2
{
public:
    virtual const char* get_name() = 0;
    virtual const char* get_shared_name() = 0;
};

class c_scene_object
{
public:
    char pad[0xB8];
    c_base_handle owner;
};

struct c_scene_skybox_object
{
public:
    std::byte pad_0000[0x110]; // 0x0
    c_material_2** sky_material; // 0x110
    std::byte pad_0118[0x10]; // 0x118 
    c_color sky_color; // 0x128
};

class c_base_scene_data
{
public:
    std::byte pad_0000[24]; // 0x0000
    c_scene_object* scene_animatable_object; // 0x0018
    c_material_2* material; // 0x0020
    c_material_2* material2; // 0x0028
    std::byte pad_0030[32]; // 0x0030
    uint8_t r; // 0x0050
    uint8_t g; // 0x0051
    uint8_t b; // 0x0052
    uint8_t a; // 0x0053
    std::byte pad_0054[20]; // 0x0054
}; // Size: 0x0068
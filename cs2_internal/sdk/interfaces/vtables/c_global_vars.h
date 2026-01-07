#pragma once
#include <cstdint>

class c_global_vars
{
public:
    float real_time; //0x0000
    int frame_count; //0x0004
    float absolute_frame_time; //0x0008
    float absolute_frame_start_time_std_dev; //0x000C
    int max_clients; //0x0010
private:
    char pad_0014[0x14]; //0x0014
public:
    void* current_time_update; //0x0028
    float frame_time; //0x0030
    float current_time; //0x0034
    float player_time; //0x0038
    float variable;
    float N00000236; //0x0040
private:
    char pad_0044[0x4]; //0x0044
public:
    int32_t tick_count; //0x0048
private:
    char pad_004C[0x8]; //0x004C
public:
    float tick_fraction; //0x0054
    void* net_channel; //0x0058
private:
    char pad_0060[0x108]; //0x0060
public:
    void* network_string_table_container; //0x0168
private:
    char pad_0170[0x40]; //0x0170
public:
    char* current_map; //0x01B0
    char* current_map_name; //0x01B8
};
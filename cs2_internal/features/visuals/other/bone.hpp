#pragma once
enum bones : int
{
    Waist = 0,
    Neck = 5,
    Head = 6,
    ShoulderLeft = 8,
    ForeLeft = 9,
    HandLeft = 11,
    ShoulderRight = 13,
    ForeRight = 14,
    HandRight = 16,
    KneeLeft = 23,
    FeetLeft = 24,
    KneeRight = 26,
    FeetRight = 27
};

struct BoneConnection {
    int from;
    int to;
};


const std::pair<int, int> bone_connections[] = {
    {6, 34}, 
    {6, 33},

    {1, 2},
    {1, 3},
    {1, 6},
    {3, 4},
    {6, 7},
    {4, 5},
    {7, 8},
    {1, 0},
    {0, 9},
    {0, 11},
    {9, 10},
    {11, 12}
};



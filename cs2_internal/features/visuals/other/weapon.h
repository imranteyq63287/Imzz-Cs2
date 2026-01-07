#pragma once
#include <cstdint>
const char* weaponOptions[] = { "Pistols", "Deagle" , "Revolver", "SMG", "Heavy", "Rifles", "Auto-Sniper", "Scout", "AWP" };
enum Weapon : short {
    Deagle = 1,
    Elite = 2,
    Fiveseven = 3,
    Glock = 4,
    Ak47 = 7,
    Aug = 8,
    Awp = 9,
    Famas = 10,
    G3Sg1 = 11,
    M249 = 14,
    Mac10 = 17,
    P90 = 19,
    MP5SD = 23,
    Ump45 = 24,
    Xm1014 = 25,
    Bizon = 26,
    Mag7 = 27,
    Negev = 28,
    Sawedoff = 29,
    Tec9 = 30,
    Zeus = 31,
    P2000 = 32,
    Mp7 = 33,
    Mp9 = 34,
    Nova = 35,
    P250 = 36,
    Scar20 = 38,
    Sg556 = 39,
    Ssg08 = 40,
    CtKnife = 42,
    Flashbang = 43,
    Hegrenade = 44,
    Smokegrenade = 45,
    Molotov = 46,
    Decoy = 47,
    Incgrenade = 48,
    C4 = 49,
    M4A4 = 16,
    UspS = 61,
    M4A1Silencer = 60,
    Cz75A = 63,
    Revolver = 64,
    TKnife = 59,
    Bowie = 514,
    ShadowDaggers = 516,
    Flip = 505,
    Bayonet = 500,
    Classic = 503,
    Gut = 506,
    Karambit = 507,
    M9 = 508,
    HuntsMan = 509,
    Falchion = 512,
    Butterfly = 515,
    Paracord = 517,
    Survival = 518,
    Ursus = 519,
    Navaja = 520,
    Nomad = 521,
    Stiletto = 522,
    Talon = 523,
    Skeleton = 525,
    Kukri = 526,
    healthshot = 57
};

const char* GetWeaponName(short id) {
    switch (id) {
    case Deagle: return "Desert Eagle";
    case Elite: return "Dual Berettas";
    case Fiveseven: return "Five-SeveN";
    case Glock: return "Glock-18";
    case Ak47: return "AK-47";
    case Aug: return "AUG";
    case Awp: return "AWP";
    case Famas: return "FAMAS";
    case G3Sg1: return "G3SG1";
    case M249: return "M249";
    case Mac10: return "MAC-10";
    case P90: return "P90";
    case MP5SD: return "MP5-SD";
    case Ump45: return "UMP-45";
    case Xm1014: return "XM1014";
    case Bizon: return "PP-Bizon";
    case Mag7: return "MAG-7";
    case Negev: return "Negev";
    case Sawedoff: return "Sawed-Off";
    case Tec9: return "Tec-9";
    case Zeus: return "Zeus x27";
    case P2000: return "P2000";
    case Mp7: return "MP7";
    case Mp9: return "MP9";
    case Nova: return "Nova";
    case P250: return "P250";
    case Scar20: return "SCAR-20";
    case Sg556: return "SG 553";
    case Ssg08: return "SSG 08";
    case CtKnife: return "CT Knife";
    case TKnife: return "T Knife";
    case Flashbang: return "Flashbang";
    case Hegrenade: return "HE Grenade";
    case Smokegrenade: return "Smoke Grenade";
    case Molotov: return "Molotov";
    case Decoy: return "Decoy";
    case Incgrenade: return "Incendiary";
    case C4: return "C4";
    case M4A4: return "M4A4";
    case UspS: return "USP-S";
    case M4A1Silencer: return "M4A1-S";
    case Cz75A: return "CZ75-Auto";
    case Revolver: return "R8 Revolver";
    case healthshot: return "Healthshot";
    case Bayonet: return "Bayonet";
    case Classic: return "Classic Knife";
    case Flip: return "Flip Knife";
    case Gut: return "Gut Knife";
    case Karambit: return "Karambit";
    case M9: return "M9 Bayonet";
    case HuntsMan: return "Huntsman Knife";
    case Falchion: return "Falchion Knife";
    case Bowie: return "Bowie Knife";
    case Butterfly: return "Butterfly Knife";
    case ShadowDaggers: return "Shadow Daggers";
    case Paracord: return "Paracord Knife";
    case Survival: return "Survival Knife";
    case Ursus: return "Ursus Knife";
    case Navaja: return "Navaja Knife";
    case Nomad: return "Nomad Knife";
    case Stiletto: return "Stiletto Knife";
    case Talon: return "Talon Knife";
    case Skeleton: return "Skeleton Knife";
    case Kukri: return "Kukri Knife";
    default: return "Unknown";
    }
}
enum class GunGroup {
    Scout,
    Awp,
    Auto,
    Rifles,
    SMG,
    Pistols,
    Deagle,
    Revolver,
    Heavy,
    Unknown
};

GunGroup GetGunGroup(short weaponID) {
    switch (weaponID) {
        // Scout
    case 40:
        return GunGroup::Scout;
        // AWP
    case 9:
        return GunGroup::Awp;
        // Auto (G3SG1 and SCAR-20)
    case 11:
    case 38:
        return GunGroup::Auto;
        // Rifles
    case 7:
    case 13:
    case 16:
    case 60:
    case 39:
    case 8:
    case 10:
        return GunGroup::Rifles;
        // SMG
    case 34:
    case 33:
    case 23:
    case 24:
    case 17:
    case 19:
    case 26:
        return GunGroup::SMG;
        // Pistols
    case 4:
    case 61:
    case 32:
    case 36:
    case 2:
    case 3:
    case 30:
    case 63:
        return GunGroup::Pistols;
        // Deagle
    case 1:
        return GunGroup::Deagle;
        // Revolver
    case 64:
        return GunGroup::Revolver;
        // Heavy (Shotguns)
    case 35:
    case 25:
    case 27:
    case 29:
        return GunGroup::Heavy;
    default:
        return GunGroup::Unknown;
    }
}
const char* GunGroupToString(GunGroup group) {
    switch (group) {
    case GunGroup::Scout: return "Scout";
    case GunGroup::Awp: return "Awp";
    case GunGroup::Auto: return "Auto";
    case GunGroup::Rifles: return "Rifles";
    case GunGroup::SMG: return "SMG";
    case GunGroup::Pistols: return "Pistols";
    case GunGroup::Deagle: return "Deagle";
    case GunGroup::Revolver: return "Revolver";
    case GunGroup::Heavy: return "Heavy";
    default: return "Unknown";
    }
}

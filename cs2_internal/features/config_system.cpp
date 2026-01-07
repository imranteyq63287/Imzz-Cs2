#include "config_system.h"

#include <filesystem>
#include "Misc/other/other.h"

inline bool is_dir(const char* dir)
{
    DWORD attrs = GetFileAttributesA(dir);

    return (attrs != INVALID_FILE_ATTRIBUTES) && (attrs & FILE_ATTRIBUTE_DIRECTORY);
}

inline void create_dir_if_not_exists(const std::string& path)
{
    if (!is_dir(path.c_str()))
        CreateDirectoryA(path.c_str(), NULL);
}

void c_config_system::create_config_folder()
{
    std::string folder;

    folder = g_ctx.cheat_name + "\\";
    create_dir_if_not_exists(folder);

    folder += "config\\";
    create_dir_if_not_exists(folder);

    folder = g_other->GetDirectory() + "sounds\\";
	create_dir_if_not_exists(folder);

    g_other->downloadsound(folder);
}
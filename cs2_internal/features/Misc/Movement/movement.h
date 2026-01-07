#include "../../../includes.h"
#include "../../config_system.h"
class c_movement {
public:
	void bunnyhop(c_user_cmd* user_cmd);
	void auto_strafe(c_user_cmd* user_cmd, float o_yaw);
	void air_strafe(c_user_cmd* cmd);
	void auto_pick(c_user_cmd* pCmd);
	void duck_jump(c_user_cmd* user_cmd);
};

inline const auto g_movement = std::make_unique<c_movement>();
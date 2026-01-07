#include "../../../includes.h"
#include "../../../sdk/utils/math.h"

class c_antiaim {
public:
	void antiaim(c_user_cmd* user_cmd);
	void fix_movement(c_user_cmd* user_cmd, vec3_t angle);
	float jitter(float min, float max, bool s = false);
	float random_jitter(float min, float max);
	float switch_jitter(float min, float max, float lerp_step = 0.1f);
	float third_way_jitter(float min, float max);
};

inline const auto g_antiaim = std::make_unique<c_antiaim>();
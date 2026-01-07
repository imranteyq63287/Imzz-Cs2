#pragma once
#include "../../../includes.h"
#include "../../../sdk/utils/math.h"

class c_notification {
public:
	void AddAnimatedNotification(const char* icon, const std::string& name, const std::string& hitgroup, int damage, int remaining_health);
	void AddNotification(const char* icon, const std::string& msg);
	void AddNotificationTopLeft(const char* icon, const std::string& msg);
	void RenderNotifications();
	void RenderTopLeftNotifications();
	inline static float spacing = 0;
};

inline const auto g_notification = std::make_unique<c_notification>();
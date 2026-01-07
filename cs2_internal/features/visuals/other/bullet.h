//#pragma once
//
//#include "../../../includes.h"
//class cbullet_tracer
//{
//public:
//	void add_bullet_trace(vec3_t start, vec3_t end, c_color clr_);
//	void add_hit_effect(vec3_t start, c_color clr);
//	void add_autopeek_effect(vec3_t start, c_color clr);
//private:
//	std::mutex MHitMutex;
//
//	class cbullet_tracer_info
//	{
//	public:
//		cbullet_tracer_info(vec3_t src, vec3_t dst, float time, c_color color)
//		{
//			this->src = src;
//			this->dst = dst;
//			this->time = time;
//			this->color = color;
//		}
//
//		vec3_t src, dst;
//		float time;
//		c_color color;
//	};
//};
//
//
//inline std::unique_ptr< cbullet_tracer > g_bullet_tracer = std::make_unique< cbullet_tracer >();
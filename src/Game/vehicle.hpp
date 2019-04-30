#ifndef VEHICLE
#define VEHICLE

#include "ThirdPerson.hpp"

class Vehicle : public ThirdPerson{
	public:
		float torque = 10;
		float friction = 0.2;
		void throttle();
		void reduce_velocity();
		virtual void update();
		void update_velocity();
		bool gas = false;
};

#endif
#ifndef PLAYER__TANKINFO__HPP
#define PLAYER__TANKINFO__HPP
#include <string>

struct TankInfo
{
public:
	char address[16]; //16

	std::string tankIDName = ""; //18
	std::string tankIDPass = ""; //18
	std::string requestedName = ""; //10

	int platformID = 0;
	double gameVersion = 0.92; //5
	std::string country = "us"; //2

	std::string rid = ""; //24
	std::string mac = "02:00:00:00:00:00"; //17

};

#endif // PLAYER__TANKINFO__HPP
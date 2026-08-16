#ifndef NETWORK__COMMANDHEADER__HPP
#define NETWORK__COMMANDHEADER__HPP

#include <vector>
#include <string>

#include <Player/Player.hpp>

class World;
struct CommandHeader
{
	std::string name;
	std::string commandUsed;
	std::vector<std::string> args;
	std::string aliasUsed;

	Player* pPlayer = nullptr;
	World* pWorld = nullptr;

};

#endif // NETWORK__COMMANDHEADER__HPP
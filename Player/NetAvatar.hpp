#ifndef PLAYER__NETAVATAR__HPP
#define PLAYER__NETAVATAR__HPP

#include <string>
#include <ProtonSDK/Math.h>

class Player;
class World;
class NetAvatar
{
public:
	NetAvatar(Player* pPlayer);
	~NetAvatar();

	int GetNetID() const { return m_netID; }
	void SetNetID(const int& id) { m_netID = id; }

	CL_Vec2f GetPosition() const { return m_vec; }
	void SetPosition(const CL_Vec2f& vec) { m_vec = vec; }

	CL_Vec2f GetRespawnPoint() const { return m_respawnPoint; }
	void SetRespawnPoint(const CL_Vec2f& vec) { m_respawnPoint = vec; }

	World* GetWorld() { return m_pWorld; }
	void SetWorld(World* pWorld);

	Player* GetPlayer() const { return m_pPlayer; }

	std::string GetSpawnData(const bool& bLocal = false);
	std::string GetMusicNoteStandingOn();


	void SendCharacterState(Player* pPlayer);
	void SendClothes(Player* pPlayer, const bool& bEquipSound = false);
	void Update();
	void SendExperience(const int& xp);

    //variantlists
	void OnConsoleMessage(const std::string& msg, const int& delay = 0);
	void OnTalkBubble(const int& playerNetID, const std::string& msg, const int& bubbleType = 0, const bool& bOverride = false, const int& delay = 0);
	void OnDialogRequest(const std::string& dialog, const int& delay = 0);
	void OnTextOverlay(const std::string& msg, const int& delay = 0);
	void OnRequestWorldSelectManu(const std::string& msg, const int& delay = 0);
	void OnInvis(const bool& bInvisible, const int& delay = 0);
	void OnFailedToEnterWorld(const int& delay = 0);
	void OnSpawn(const std::string& data, const int& delay = 0);
	void OnRemove(const std::string& netID, const std::string& playerID, const int& delay = 0);
	void OnNameChanged(const std::string& name, const std::string& titleIcon, const int& netID, const int& delay = 0);

	void LogMsg(const char* traceStr, ...);

private:
	int m_netID = -1;

	CL_Vec2f m_vec = { -1.f, -1.f };
	CL_Vec2f m_respawnPoint = { -1.f, -1.f };

	World* m_pWorld = nullptr;
	Player* m_pPlayer = nullptr;

};

#endif // PLAYER__NETAVATAR__HPP
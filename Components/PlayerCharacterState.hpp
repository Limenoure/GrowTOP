#ifndef COMPONENTS__PLAYERCHARACTERSTATE__HPP
#define COMPONENTS__PLAYERCHARACTERSTATE__HPP

#include <string>

#include <ProtonSDK/MiscUtils.h>

enum eCharacterStateFlags : unsigned int
{
	STATEFLAG_NONE = 0x0,
	STATEFLAG_NOCLIP = 0x1,
	STATEFLAG_DOUBLE_JUMP = 0x2,
	STATEFLAG_INVISIBLE = 0x4,
	STATEFLAG_NO_HANDS = 0x8,
	STATEFLAG_NO_FACE = 0x10,
	STATEFLAG_NO_BODY = 0x20,
	STATEFLAG_DEVIL_HORNS = 0x40,
	STATEFLAG_GOLDEN_HALO = 0x80,
	STATEFLAG_RESPAWN_EFFECT = 0x100,
	STATEFLAG_IGNORE_LAVA = 0x200,
	STATEFLAG_IGNORE_SPIKES = 0x400,
	STATEFLAG_FROZEN = 0x800,
	STATEFLAG_CURSED = 0x1000,
	STATEFLAG_DUCT_TAPE = 0x2000,
	STATEFLAG_SMOKE = 0x4000,
	STATEFLAG_SHINY = 0x8000,
	STATEFLAG_ZOMBIE = 0x10000,
	STATEFLAG_SPICY = 0x20000,
	STATEFLAG_SHADOWS = 0x40000,
	STATEFLAG_IRRADIATED = 0x80000,
	STATEFLAG_SPOTLIGHT = 0x100000,
	STATEFLAG_PINEAPPLE_SKIN = 0x200000,
	STATEFLAG_PINEAPPLE_FLAG = 0x400000,
	STATEFLAG_SUPPORTER = 0x800000,
	STATEFLAG_SUPER_SUPPORTER = 0x1000000,
	STATEFLAG_PINEAPPLE_AURA = 0x2000000,
	STATEFLAG_WATER_ORB = 0x4000000,
	STATEFLAG_WET = 0x8000000,
	STATEFLAG_NEON_GUM = 0x10000000
};

enum eCharacterEffectFlags
{
	EFFECT_FLAG_NONE = 0x0,
	EFFECT_FLAG_WINTERFEST_CROWN_RED = 0x1,
	EFFECT_FLAG_WINTERFEST_CROWN_GREEN = 0x2,
	EFFECT_FLAG_WINTERFEST_CROWN_SILVER = 0x4,
	EFFECT_FLAG_WINTERFEST_CROWN_GOLD = 0x8,
	EFFECT_FLAG_SMALL_BODY = 0x400,
	EFFECT_FLAG_MIND_CONTROL = 0x800,
	EFFECT_FLAG_PINEAPPLE_CHEST = 0x1000

};


class PlayerCharacterState
{
public:
	PlayerCharacterState() = default;
	~PlayerCharacterState() = default;

	uint32_t GetHairColor() const { return m_hairColor; }
	void SetHairColor(const uint32_t& color) { m_hairColor = color; }

	uint32_t GetEyesColor() const { return m_eyeColor; }
	void SetEyesColor(const uint32_t& color) { m_eyeColor = color; }

	uint32_t GetLenColor() const { return m_lenColor; }
	void SetLenColor(const uint32_t& color) { m_lenColor = color; }

	float GetGravity() const { return m_gravity; }
	void SetGravity(const float& gravity) { m_gravity = gravity; }

	float GetSpeed() const { return m_speed; }
	void SetSpeed(const float& speed) { m_speed = speed; }

	float GetWaterSpeed() const { return m_waterSpeed; }
	void SetWaterSpeed(const float& wspeed) { m_waterSpeed = wspeed; }

	float GetAcceleration() const { return m_acceleration; }
	void SetAcceleration(const float& accel) { m_acceleration = accel; }

	float GetKnockPower() const { return m_knockPower; }
	void SetKnockPower(const float& power) { m_knockPower = power; }

	int GetStateFlags() const { return m_characterFlags; }
	void SetStateFlags(const int& flags) { m_characterFlags = flags; }

	bool HasStateFlag(const int& flag) { return m_characterFlags & flag; }

	void ToggleStateFlag(const int& flag, const bool& bEnable) 
	{
		if (bEnable && !HasStateFlag(flag))
		{
			m_characterFlags |= flag;
		}

		if (!bEnable && HasStateFlag(flag))
		{
			m_characterFlags &= ~flag;
		}
	}

	int GetEffectFlags() const { return m_effectFlags; }
	void SetEffectFlags(const int& flags) { m_effectFlags = flags; }

	bool HasEffectFlag(const int& flag) { return m_effectFlags & flag; }

	void ToggleEffectFlag(const int& flag, const bool& bEnable)
	{
		if (bEnable && !HasEffectFlag(flag))
		{
			m_effectFlags |= flag;
		}

		if (!bEnable && HasEffectFlag(flag))
		{
			m_effectFlags &= ~flag;
		}
	}

private:
	uint32_t m_hairColor = MAKE_RGBA(255, 255, 255, 255);
	uint32_t m_eyeColor = MAKE_RGBA(255, 255, 255, 255);
	uint32_t m_lenColor = MAKE_RGBA(0, 0, 0, 255);

	int m_characterFlags = 0;
	int m_effectFlags = 0;


	float m_gravity = 1000.f;
	float m_speed = 250.f;
	float m_waterSpeed = 125.f;
	float m_acceleration = 1200.f;
	float m_knockPower = 200.f;

};


#endif // COMPONENTS__PLAYERCHARACTERSTATE__HPP
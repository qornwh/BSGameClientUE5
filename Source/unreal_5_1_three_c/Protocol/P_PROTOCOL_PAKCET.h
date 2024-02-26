#pragma once

struct P_LOGIN_PAKCET
{
	uint16 Type;
	uint16 IdLen;
	FString Id;

	uint32 Size()
	{
		return sizeof(uint16) * 2 + sizeof(uint8) * IdLen;
	}
};

struct P_MOVE_PACKET
{
	int32 X;
	int32 Y;
	int32 Z;
	float Yew;

	uint32 Size()
	{
		return sizeof(P_MOVE_PACKET);
	}
};

struct P_CHAT_PACKET
{
	uint8 Type;
	uint32 TextLen;
	FString Text;

	uint32 Size()
	{
		return sizeof(uint8) + sizeof(uint32) + TextLen;
	}
};

struct P_SKILL_PACKET
{
	int32 SkillCode;

	uint32 Size()
	{
		return sizeof(P_SKILL_PACKET);
	}
};

struct P_HIT_PACKET
{
	// 맞은 대상
	int32 TargetCode;
	// 공격 대상
	int32 AttackCode;

	uint32 Size()
	{
		return sizeof(P_HIT_PACKET);
	}
};

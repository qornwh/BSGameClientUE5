#pragma once

struct BS_FVector
{
	BS_FVector(int32 X, int32 Y, int32 Z, float Yaw) : X(X), Y(Y), Z(Z), Yaw(Yaw)
	{
	};

	int32 X;
	int32 Y;
	int32 Z;
	float Yaw;

	BS_FVector& operator=(const BS_FVector& ref)
	{
		X = ref.X;
		Y = ref.Y;
		Z = ref.Z;
		Z = ref.Yaw;
		return *this;
	}
};

struct UnitInfo : TSharedFromThis<UnitInfo>
{
	UnitInfo(uint16 Type, uint16 Hp, int32 Code) : Type(Type), Hp(Hp), Code(Code), Hit(-1), Demage(-1), SkillCode(-1)
	{
	};

	uint16 Type;
	uint8 Hp;
	int32 Code;
	FString Name;
	int32 SkillCode;
	int8 Hit;
	int32 Demage;
	float Speed = 1.f;
	BS_FVector Position{0, 0, 0, 0.f};
};

struct PlayerInfo : UnitInfo
{
	PlayerInfo(uint16 Type, uint16 Hp, int32 Code) : UnitInfo(Type, Hp, Code)
	{
		Speed = 0.7f;
	};
};

struct MonsterInfo : UnitInfo
{
	MonsterInfo(uint16 Type, uint16 Hp, int32 Code) : UnitInfo(Type, Hp, Code)
	{
		Speed = 1.f;
	};

	int32 target = -1;
};

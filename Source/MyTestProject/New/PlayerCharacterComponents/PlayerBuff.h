// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBuff.generated.h"


class AMyNewCharacter;
class UCharacterStatusComponent;


UENUM(BlueprintType)
enum class ENewBuffType : uint8 {
	E_DUALDAMAGE = 0 UMETA(DisplayName="DamageBuff"),
	E_EVADE = 1 UMETA(DisplayName = "Evade"),
	E_TIMEDELAY = 2 UMETA(DisplayName = "Time"),
};
class MYTESTPROJECT_API PlayerBuff
{
public:
	PlayerBuff(ENewBuffType type,float time);
	virtual ~PlayerBuff();
protected:
	bool IsValid;
	ENewBuffType Type;
	float RemainTime;
	AMyNewCharacter* Owner;
public:
	virtual void BeginBuff(AMyNewCharacter& character) = 0;
	virtual void TickBuff(float delta) = 0;
	virtual void EndBuff() = 0;
	inline bool GetIsValid() const { return IsValid; }
	inline const ENewBuffType GetType() const { return Type; }
};

class MYTESTPROJECT_API DamageBuff : public PlayerBuff
{
public:
	DamageBuff(ENewBuffType type, float time);
	virtual ~DamageBuff();
private:
	UCharacterStatusComponent* status;
	FDelegateHandle EndDel;
public:
	virtual void BeginBuff(AMyNewCharacter& character) override;
	virtual void TickBuff(float delta) override;
	virtual void EndBuff() override;
};

class MYTESTPROJECT_API EvadeBuff : public PlayerBuff
{
public:
	EvadeBuff(ENewBuffType type, float time);
	virtual ~EvadeBuff();
public:
	virtual void BeginBuff(AMyNewCharacter& character) override;
	virtual void TickBuff(float delta) override;
	virtual void EndBuff() override;
};

class MYTESTPROJECT_API TimeDilation : public PlayerBuff
{
public:
	TimeDilation(ENewBuffType type, float time);
	virtual ~TimeDilation();
public:
	virtual void BeginBuff(AMyNewCharacter& character) override;
	virtual void TickBuff(float delta) override;
	virtual void EndBuff() override;
};

class MYTESTPROJECT_API PotionTimer 
{
public:
	PotionTimer();
	~PotionTimer();

private:
	bool CanUse;
	float RemainTime;
	const float PotionCoolTime;
public:
	bool Use();
	void Timer(float delta);
	inline bool GetCanUse() const { return CanUse; }
	inline const float GetRemainTime() const { return RemainTime; }
	inline const float GetPotionCoolTime() const { return PotionCoolTime; }
};
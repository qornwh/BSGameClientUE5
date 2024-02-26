// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyFSocketActor.generated.h"

UCLASS()
class UNREAL_5_1_THREE_C_API AMyFSocketActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyFSocketActor();
	AMyFSocketActor(FSocket* ClientSocket);
	~AMyFSocketActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int32 SendMessage(uint8* ptr, int32 count);
	void SetClientSocket(FSocket* ClientSocket);

private:
	void ReadMessageHandler(uint8* buffer, int32 size);

	template<typename T>
	T ReadBufferPtr(uint8* buffer, int& offset);

	FString ReadBufferStr(uint8* buffer, int& ptr, int32 len);
	FString ReadBufferStr16(uint8* buffer, int& ptr, int32 len);

private:
	FSocket* _clientSocket;
	
	TAtomic<bool> IsConnect = false;

	char* __readBuffer;
	char* __sendBuffer;

	bool _isLoad = false;
};

template <typename T>
T AMyFSocketActor::ReadBufferPtr(uint8* buffer, int& offset)
{
	int _ptr = offset;
	offset += sizeof(T);
	T* temp = reinterpret_cast<T*>(&buffer[_ptr]);
	return static_cast<T>(*temp);
}

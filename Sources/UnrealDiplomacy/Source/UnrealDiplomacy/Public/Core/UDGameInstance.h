// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UDGameInstance.generated.h"

/**
 * Persistent state per game instance.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void Quit();
};
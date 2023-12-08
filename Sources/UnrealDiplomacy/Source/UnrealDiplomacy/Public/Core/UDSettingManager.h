// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDSettingManager.generated.h"

/**
 * Temporary place for different small configurations...
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSettingManager : public UObject
{
public:
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	int32 MaxAiCount = 7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 MaxTurnCount = 12;
};
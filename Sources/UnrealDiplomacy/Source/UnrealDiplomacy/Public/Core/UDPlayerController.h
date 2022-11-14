// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "UDPlayerController.generated.h"

/**
 * Overrides default PlayerController to always use Game & UI input.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};

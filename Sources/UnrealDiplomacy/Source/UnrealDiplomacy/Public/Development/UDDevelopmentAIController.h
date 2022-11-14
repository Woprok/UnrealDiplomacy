// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Skirmish/UDSkirmishAIController.h"
#include "UDDevelopmentAIController.generated.h"

/**
 * USED FOR DEBUG ONLY
 */
UCLASS()
class UNREALDIPLOMACY_API AUDDevelopmentAIController : public AUDSkirmishAIController
{
	GENERATED_BODY()
public:
	AUDDevelopmentAIController();
	void Tick(float DeltaSeconds) override;
};

// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "Core/UDControllerInterface.h"
#include "UDSkirmishAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishAIController : public AUDAIController, public IUDControllerInterface
{
	GENERATED_BODY()
public:
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	virtual int32 GetControllerUniqueId() override;
private:
	int32 UniqueControllerId;
};

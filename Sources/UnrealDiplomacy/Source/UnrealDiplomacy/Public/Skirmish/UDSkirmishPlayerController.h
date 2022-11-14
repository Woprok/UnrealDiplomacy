// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPlayerController.h"
#include "Core/UDControllerInterface.h"
#include "UDSkirmishPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishPlayerController : public AUDPlayerController, public IUDControllerInterface
{
	GENERATED_BODY()
public:
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	virtual int32 GetControllerUniqueId() override;
private:
	int32 UniqueControllerId;
};

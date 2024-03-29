// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDefaultAction.h"
#include "UDDefaultActionInvalid.generated.h"

/**
 * Handles action that are passed to execution with undefined executor.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultActionInvalid : public UUDDefaultAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual int32 GetId() const override;
};
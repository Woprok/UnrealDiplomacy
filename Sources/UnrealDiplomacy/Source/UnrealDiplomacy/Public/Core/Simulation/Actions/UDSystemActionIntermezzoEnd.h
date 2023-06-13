// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionIntermezzoEnd.generated.h"

/**
 * Ends current intermezzo period.
 * This can be done only by server / gaia.
 * Generally invoked as consequence of turn finish.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionIntermezzoEnd : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 13;
};

// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "UDSystemActionRegentChange.generated.h"

/**
 * Changes current regent.
 * This is always invoked by the server.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionRegentChange : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual bool HasContinuations() const override { return true; }
	// This action needs to call end of intermezzo.	
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const override;
public:
	static const int32 ActionTypeId = 8;
protected:
	/**
	 * Passes regency to next player.
	 */
	void NextRegent(TObjectPtr<UUDWorldState> world);
	/**
	 * Returns regency to specified player.
	 */
	void PreviousRegent(TObjectPtr<UUDWorldState> world);
};
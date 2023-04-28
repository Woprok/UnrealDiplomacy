// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionTurnEnd.generated.h"

/**
 * Ends the turn of player that is invoking this.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionTurnEnd : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 7;
protected:
	/**
	 * Passes turn control to next player.
	 */
	void TurnAdvance(TObjectPtr<UUDWorldState> world);
	/**
	 * Returns turn control to specified player.
	 */
	void RevertTurnAdvance(TObjectPtr<UUDWorldState> world, int32 previousPlayerId);
};
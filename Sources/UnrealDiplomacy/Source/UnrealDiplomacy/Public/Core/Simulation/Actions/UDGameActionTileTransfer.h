// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameAction.h"
#include "UDGameActionTileTransfer.generated.h"

/**
 * Tile transfer that can be accepted or rejected.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionTileTransfer : public UUDGameAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGameDataTargetTile::ParameterCount; };
	virtual FUDActionPresentation GetPresentation() const override;
public:
	static const int32 ActionTypeId = 2008;
};
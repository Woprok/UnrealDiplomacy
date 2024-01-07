// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameAction.h"
#include "UDGameActionTileBuildManufactury.generated.h"

/**
 * Invoker will build Manufactury on specified tile.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionTileBuildManufactury : public UUDGameAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGameDataTile::ParameterCount; };
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager) override;
	virtual FUDActionPresentation GetPresentation() const override;
public:
	static const int32 ActionTypeId = 2022;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus")
	int32 FlatResourceBonus = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus For Correct Type")
	int32 ExtraResourceBonus = 225;
};
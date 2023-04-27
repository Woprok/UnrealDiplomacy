// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionWorldCreate.generated.h"

/**
 * Automatic action generated by the WorldGenerator that assign's duplicates of World to all existing states.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionWorldCreate : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	virtual void SetWorldGenerator(TObjectPtr<UUDWorldGenerator> worldGenerator) override
	{
		WorldGenerator = worldGenerator;
	}
public:
	static const int32 ActionTypeId = 66600;
	static const int32 RequiredParametersCount = 3;
	static FUDMapSettingsData ConvertData(FUDActionData& data)
	{
		return FUDMapSettingsData(data.ValueParameters);
	}
protected:
	UPROPERTY()
		TWeakObjectPtr<UUDWorldGenerator> WorldGenerator = nullptr;
};
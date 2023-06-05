// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSettingAction.generated.h"

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDSettingDataValue
{
	GENERATED_BODY()
public:
	FUDSettingDataValue() {}
	FUDSettingDataValue(TArray<int32> valueParameters)
	{
		Value = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
	static const int32 ParameterCount = 1;
};

/**
 * Generic ancestor for all System actions.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDSettingAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 4000;
};
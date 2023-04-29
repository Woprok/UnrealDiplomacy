// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "UDDealActionPointModifyType.generated.h"

/**
 * Modifies point type.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionPointModifyType : public UUDDealActionPointModify
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealDataPointValue::ParameterCount; };
	bool IsBackupRequired() const override { return true; };
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 3036;
	static int32 PointTypeToInteger(EUDPointType type)
	{
		return static_cast<int32>(static_cast<uint8>(type));
	}
	static EUDPointType IntegerToPointType(int32 type)
	{
		if (0 > type || type > UINT8_MAX)
			return EUDPointType::Error;
		return static_cast<EUDPointType>(type);
	}
};
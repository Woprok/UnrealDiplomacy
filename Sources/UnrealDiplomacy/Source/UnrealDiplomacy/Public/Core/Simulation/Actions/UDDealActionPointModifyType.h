// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyType.generated.h"

UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyType : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	bool IsBackupRequired() override;
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10035;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& action)
	{
		return FUDDealPointValueData(action.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& action)
	{
		return FUDValueData(action.BackupValueParameters);
	}
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
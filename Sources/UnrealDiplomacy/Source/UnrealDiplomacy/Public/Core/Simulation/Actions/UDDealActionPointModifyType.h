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
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	bool RequiresBackup() override;
	void Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
public:
	static const int32 ActionTypeId = 10035;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDValueData(data.BackupValueParameters);
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
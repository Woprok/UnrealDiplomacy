// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionFactionTakeover.generated.h"

// Forward Declarations

enum class EUDFactionController : uint8;

/**
 * Changes who/what is responsible for specified faction.
 * InvokerFactionId is used as faction that will have controller changed.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionFactionTakeover : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDSystemDataValue::ParameterCount; };
	bool IsBackupRequired() const override { return true; };
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 20;
	static int32 FactionControllerToInteger(EUDFactionController factionController);
	static EUDFactionController IntegerToFactionController(int32 factionControllerValue);
};
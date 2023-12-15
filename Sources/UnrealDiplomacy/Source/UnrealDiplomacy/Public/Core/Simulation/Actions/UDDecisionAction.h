// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDecisionAction.generated.h"

// Forward Declarations
enum class EUDDecisionType : uint8;
struct FUDDecision;

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDecisionDataAction
{
	GENERATED_BODY()
public:
	FUDDecisionDataAction() {}
	FUDDecisionDataAction(TArray<int32> valueParameters)
	{
		ActionId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 ActionId = 0;
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDecisionDataDecision
{
	GENERATED_BODY()
public:
	FUDDecisionDataDecision() {}
	FUDDecisionDataDecision(TArray<int32> valueParameters)
	{
		DecisionId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DecisionId = 0;
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDecisionDataTargetTypeActionParameters
{
	GENERATED_BODY()
public:
	FUDDecisionDataTargetTypeActionParameters() {}
	FUDDecisionDataTargetTypeActionParameters(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		Type = valueParameters[1];
		DeclineReplacementActionId = valueParameters[2];
		for (int32 i = ParameterCount; i < valueParameters.Num(); i++)
		{
			Parameters.Add(valueParameters[i]);
		}
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Type = 0;
	/** Assumes that called action in decline will use same params and transforms them to whatever is required. */
	UPROPERTY(BlueprintReadOnly)
	int32 DeclineReplacementActionId = -1;
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Parameters = { };
	static const int32 ParameterCount = 3;
};

UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDecisionAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 5000;
	static int32 DecisionTypeToInteger(EUDDecisionType type);
	static EUDDecisionType IntegerToDecisionType(int32 type);
protected:
	/**
	 * Adds pending decision associated with action and specified target.
	 */
	void AddPendingTargetDecision(int32 targetId, int32 decisionId, FUDDecision decision, TObjectPtr<UUDWorldState> world);
	/**
	 * Removes pending decision associated with action and specified target.
	 */
	void RemovePendingTargetDecision(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world);
	/**
	 * Checks if there is pending decision associated with action and specified target.
	 */
	bool IsPendingTargetDecision(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world) const;
	/**
	 * Checks if there is pending interchangeable decision associated with target.
	 */
	bool IsPendingInterchangeableTargetDecision(const FUDDecision& decision, int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world) const;
	/**
	 * Moves decision from resolved to pending.
	 */
	void MoveToPending(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world);
	/**
	 * Moves decision from pending to resolved.
	 */
	void MoveToResolved(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world);
};
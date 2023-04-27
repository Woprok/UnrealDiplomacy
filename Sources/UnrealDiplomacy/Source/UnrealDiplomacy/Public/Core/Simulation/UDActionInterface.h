// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.h"
#include "UDWorldState.h"
#include "UDWorldGenerator.h"
#include "UDModifier.h"
#include "UDModifierManager.h"
#include "UDActionInterface.generated.h"

// TODO extend by adding more functions that do verify & check restriction

/** 
 * Specifier for interface used by IUDAction.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Interface for executable action.
 * Action is defined by ability to take data and apply their expected effect on simulation.
 */
class UNREALDIPLOMACY_API IUDActionInterface
{
	GENERATED_BODY()
public:
	/**
	 * Checks if this action is valid for current state.
	 * Validity is in general checked on GaiaWorldState.
	 */
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	/**
	 * This applies action over the UUDWorldState.
	 */
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	 /**
	  * If possible action should be able to revert, whatever kind of operation it did,
	  * if it's provided with the same data.
	  */
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
	 /**
	  * Unique action id that is provided for each action.
	  * This is directly referenced by FUDActionData to determine,
	  * who can execute it as the FUDActionData is very simple container.
	  * This has to be overriden, otherwise the action will be discarded (valid values are 0 - int32.MAX).
	  */
	 virtual int32 GetActionTypeId()
	 {
		 // Default Interface call returns invalid value, e.g. -1.
		 return -1;
	 }
	 virtual int32 GetRequiredParametersCount()
	 {
		 // Default Interface call returns invalid value, e.g. -1.
		 return -1;
	 }
	 /**
	  * Set the instance of world generator used by all registred actions.
	  * If actions share world generator, they can use same data.
	  * Note: all actions that use generator needs this to be set.
	  * By default set this to every action.
	  */
	 virtual void SetWorldGenerator(TObjectPtr<UUDWorldGenerator> worldGenerator)
	 {
		 // Default Interface call is empty.
	 }
	 /**
	  * Set the instance of modifier manager used by all registred actions.
	  * Actions share modifier manager to correctly handle all modifiers.
	  * Note: all actions that use modifiers needs this to be set.
	  * By default set this to every action.
	  */
	 virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager)
	 {
		 // Default Interface call is empty.
	 }
	 /**
	  * Determines if this action is eligible to execute additional actions.
	  * Additional actions should never be composite to prevent loop!
	  */
	 virtual bool IsComposite()
	 {
		 return false;
	 }
	 /**
	  * Additional actions that should be executed after this action, based on input.
	  * Validity is in general checked on GaiaWorldState.
	  */
	 virtual TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState);
	 /**
	  * Indicates if this action requires additional backup before being applied.
	  */
	 virtual bool RequiresBackup();
	 /** 
	  * Backups specific data into provided action data so they can be restored on revert to original state.
	  */
	 virtual void Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState);
};









/**
 * Deal actions between any amount of players.
 * Core part of the game. Uses 10000-19999 for all subsequent actions.
 * Creates new deal.
 * Player that created the deal is considered a participant immediately.
 * Deals do not have an owner and are stored as part of world state.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDCreateDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10000;
	static const int32 RequiredParametersCount = 0;
}; 

/**
 * Invite player to participate, interaction is required for accepting or rejecting.
 * Player that was invinted is then considered participant.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDInviteParticipantDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10001;
	static const int32 RequiredParametersCount = 2;
	static FUDDealTargetData ConvertData(FUDActionData& data)
	{
		return FUDDealTargetData(data.ValueParameters);
	}
};

/**
 * Accept and become a participant in the deal.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAcceptParticipationDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10002;
	static const int32 RequiredParametersCount = 2;
};

/**
 * Reject and become unable to become a participant in the deal.
 * Deal can still affect this player, e.g. being target of other action...
 */
UCLASS()
class UNREALDIPLOMACY_API UUDRejectParticipationDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10003;
	static const int32 RequiredParametersCount = 2;
};

/**
 * Leave deal and loose participation, same as rejecting, but can be done at any point, if player is fed up with it.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDKickParticipantDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10004;
	static const int32 RequiredParametersCount = 2;
	static FUDDealTargetData ConvertData(FUDActionData& data)
	{
		return FUDDealTargetData(data.ValueParameters);
	}
};

/**
 * Leave deal and loose participation, same as rejecting, but can be done at any point, if player is fed up with it.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDLeaveParticipationDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10004;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

/**
 * Updates states
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateAssemblingDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10009;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateExtendingDraftDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10010;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateDemandsAndRequestsDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10011;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateBiddingDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10012;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateFinalizingDraftDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10013;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateVoteDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10014;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateResolutionDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10015;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceStateResultDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10016;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
/**
 * Updates result
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceResultPassedDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100017;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
/**
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceResultVetoedDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100018;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
/**
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceResultDisassembledDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100019;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
/**
 * Moderator is able to close deal, making all actions for it invalid and deak has no result.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAdvanceResultClosedDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10020;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};


/**
 * Allows extending of the deal with new discussion point.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAddDiscussionItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10030;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

/**
 * Removes discussion point from the deal. Item is still stored and ready to be used, if revert is called.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDIgnoreDiscussionItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10031;
	static const int32 RequiredParametersCount = 1;
	static FUDDealPointData ConvertData(FUDActionData& data) 
	{
		return FUDDealPointData(data.ValueParameters);
	}
};

/**
 * Updates discussion point from the deal.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSendMessageDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10032;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

/**
 * Allows extending of the deal with new discussion point.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAddChildDiscussionItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10033;
	static const int32 RequiredParametersCount = 2;
	static FUDDealPointData ConvertData(FUDActionData& data)
	{
		return FUDDealPointData(data.ValueParameters);
	}
};


#pragma region  UpdateDealItem

UCLASS()
class UNREALDIPLOMACY_API UUDDefineActionDealAction : public UObject, public IUDActionInterface
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
	virtual bool IsComposite() override { return true; }
	// This action needs to clear all parameters. Clean all parameters is exclusively made for this.
	virtual TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState) override;
public:
	static const int32 ActionTypeId = 10034;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDValueData(data.BackupValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDCleanParametersPointDealAction : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10051;
	static const int32 RequiredParametersCount = 2;
	static FUDDealPointData ConvertData(FUDActionData& data)
	{
		return FUDDealPointData(data.ValueParameters);
	}
};

// Covers single value parameter actions
UCLASS()
class UNREALDIPLOMACY_API UUDChangeValueParameterPointDealAction : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10052;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDValueData(data.BackupValueParameters);
	}
};

// Covers single tile parameter actions
UCLASS()
class UNREALDIPLOMACY_API UUDChangeTileParameterPointDealAction : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10053;
	static const int32 RequiredParametersCount = 4;
	static FUDDealPointTileData ConvertData(FUDActionData& data)
	{
		return FUDDealPointTileData(data.ValueParameters);
	}
	static FUDTileData ConvertBackupData(FUDActionData& data)
	{
		return FUDTileData(data.BackupValueParameters);
	}
};

// Covers single tile & additional single value actions
UCLASS()
class UNREALDIPLOMACY_API UUDChangeTileValueParameterPointDealAction : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10054;
	static const int32 RequiredParametersCount = 5;
	static FUDDealPointTileValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointTileValueData(data.ValueParameters);
	}
	static FUDTileValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDTileValueData(data.BackupValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDDefinePointTypeDealAction : public UObject, public IUDActionInterface
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

UCLASS()
class UNREALDIPLOMACY_API UUDAddInvokerDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10038;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDRemoveInvokerDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10039;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDAddTargetDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10040;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDRemoveTargetDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10041;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDReadyDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10042;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDNotReadyDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10043;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDPositiveVoteDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10044;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

UCLASS()
class UNREALDIPLOMACY_API UUDNegativeVoteDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10045;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

/**
 * This action transforms preview of actions to final actions.
 * After this players can choose if they honour this deal.
 * This is executed on clients as well. The order is strictly determined for all actions.
 * for each invoker in ascending order, then for each target in ascending order.
 * The order must remain same as the actions are temporatily assigned array index as their Id.
 * Actions can't receive Id unless they are being executed and this would also mean that
 * simulation has to receive all actions from server. Which might be taxing!
 * Single item can easily result in exponential amount of actions.
 * TODO change this to receive from server might be in the end safer for sync. 
 * Current version does not support partial information about the result.
 * In theory there is no way to have player believe that something did happen.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDFinalizeItemsDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10046;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
	static TArray<FUDActionData> FinalizeActions(TObjectPtr<UUDWorldState> targetWorldState, int32 dealUniqueId);
	static TArray<FUDDiscsussionAction> WrapActions(TArray<FUDActionData> actionData);
};

/**
 * Accepts the action.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAcceptFinalItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10047;
	static const int32 RequiredParametersCount = 2;
	static FUDDealValueData ConvertData(FUDActionData& data)
	{
		return FUDDealValueData(data.ValueParameters);
	}
};
/**
 * Deny the action.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDenyFinalItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10048;
	static const int32 RequiredParametersCount = 2;
	static FUDDealValueData ConvertData(FUDActionData& data)
	{
		return FUDDealValueData(data.ValueParameters);
	}
};
/**
 * Change the action.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAlterFinalItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10049;
	static const int32 RequiredParametersCount = 2;
	static FUDDealValueData ConvertData(FUDActionData& data)
	{
		return FUDDealValueData(data.ValueParameters);
	}
};
/**
 * Sabotage the action.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSabotageFinalItemDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10050;
	static const int32 RequiredParametersCount = 2;
	static FUDDealValueData ConvertData(FUDActionData& data)
	{
		return FUDDealValueData(data.ValueParameters);
	}
};
/*
 * Executes all actions in the deal, based on players final believe.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDExecuteAllActionsDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	virtual bool IsComposite() override { return true; }
	virtual TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState) override;
public:
	static const int32 ActionTypeId = 10055;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
	static bool AreAllActionsPrepared(TObjectPtr<UUDWorldState> targetWorldState, int32 dealUniqueId);
};
#pragma endregion

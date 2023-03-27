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
	 TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState);
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
 * Simple action example for debugging purposes.
 */ 
UCLASS()
class UNREALDIPLOMACY_API UUDLogAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 0;
	static const int32 RequiredParametersCount = 0;
};

/**
 * Notification for new Player/Ai being part of the game.
 * This allows new player to receive history and create list of all players.
 * Note: Invoker is always the new player, even, if he was added by someone esle AI by server.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAddPlayerAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1;
	static const int32 RequiredParametersCount = 0;
};

/**
 * Start of game action that is called as action only once.
 * This is only due to fact that we are otherwise using only end turn action.
 * Which is for most purposes sufficient, as any player can check if they are currently playing
 * by observing their state. First player does not know this, so Gaia which takes the first turn
 * is awaiting for this to end her turn.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDStartGameAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static 	const int32 ActionTypeId = 2;
	static const int32 RequiredParametersCount = 0;
};

UCLASS()
class UNREALDIPLOMACY_API UUDEndGameAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static 	const int32 ActionTypeId = 3;
	static const int32 RequiredParametersCount = 0;
};

/**
 * End of turn action that is called as action by clients or AIs.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDEndTurnAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 4;
	static const int32 RequiredParametersCount = 0;
};


/**
 * End of turn action that is called as action by server only.
 * Requires TargetParameter that contains the Player that was forced to end the turn.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDForceEndTurnAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 5;
	static const int32 RequiredParametersCount = 1;
	static FUDTargetData ConvertData(FUDActionData& data)
	{
		return FUDTargetData(data.ValueParameters);
	}
};

/**
 * Generates income for all players. 
 * Called only on GAIA turn.
 * Requires ValueParameter to determine amount to generate for all players.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGenerateIncomeAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100;
	static const int32 RequiredParametersCount = 1;
	static FUDValueData ConvertData(FUDActionData& data)
	{
		return FUDValueData(data.ValueParameters);
	}
};

/**
 * Simple gift of resources between two players, without any strings attached.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDUnconditionalGiftAction : public UObject, public IUDActionInterface
 {
	 GENERATED_BODY()
 public:
	 virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	 virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	 virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	 virtual int32 GetActionTypeId() override { return ActionTypeId; };
	 virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
 public:
	 static const int32 ActionTypeId = 1000;
	 static const int32 RequiredParametersCount = 2;
	 static FUDTargetValueData ConvertData(FUDActionData& data)
	 {
		 return FUDTargetValueData(data.ValueParameters);
	 }
 };


/**
 * Simple gift of resources between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGiftAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1001;
	static const int32 RequiredParametersCount = 2;
	static FUDTargetValueData ConvertData(FUDActionData& data)
	{
		return FUDTargetValueData(data.ValueParameters);
	}
};

/**
 * Confirm action for UUDGiftAction
 * Simple gift of resources between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDConfirmGiftAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1002;
	static const int32 RequiredParametersCount = 2;
};

/**
 * Reject action for UUDGiftAction
 * Simple gift of resources between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDRejectGiftAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1003;
	static const int32 RequiredParametersCount = 2;
};

/**
 * Automatic action generated by the WorldGenerator that assign's duplicates of World to all existing states.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDCreateWorldMapAction : public UObject, public IUDActionInterface
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

/**
 * Change ownership of an tile. Tile will be owned by InvokerPlayer and it will be taken from TargetPlayer.
 * Tile is always owned by someone, either player or world (which is in a certain way also considered player)
 */
UCLASS()
class UNREALDIPLOMACY_API UUDTakeTileAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 5004;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& data)
	{
		return FUDTargetTileData(data.ValueParameters);
	}
};

/**
 * Exploit tile for resource. Requires tile to be owned by the invoking player.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDExploitTileAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager) override
	{
		ModifierManager = modifierManager;
	}
public:
	static const int32 ActionTypeId = 5008;
	static const int32 RequiredParametersCount = 3;
	static FUDTileValueData ConvertData(FUDActionData& data)
	{
		return FUDTileValueData(data.ValueParameters);
	}
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
};

/**
 * Simple transfer of tile between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDTransferTileAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1004;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& data)
	{
		return FUDTargetTileData(data.ValueParameters);
	}
};

/**
 * Confirm action for UUDTransferTileAction
 * Simple transfer of tile between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDConfirmTransferTileAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1005;
	static const int32 RequiredParametersCount = 3;
};

/**
 * Reject action for UUDTransferTileAction
 * Simple transfer of tile between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDRejectTransferTileAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1006;
	static const int32 RequiredParametersCount = 3;
};

/**
 * Enables other player to exploit tile.
 * Simple/First use of modifier system.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGrantExploitTilePermissionAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager) override
	{
		ModifierManager = modifierManager;
	}
public:
	static const int32 ActionTypeId = 1007;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& data)
	{
		return FUDTargetTileData(data.ValueParameters);
	}
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
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
public:
	static const int32 ActionTypeId = 10034;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
	static FUDDealPointValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.BackupValueParameters);
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
	static FUDDealPointValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.BackupValueParameters);
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
class UNREALDIPLOMACY_API UUDAddParticipantDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10036;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
};
UCLASS()
class UNREALDIPLOMACY_API UUDRemoveParticipantDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10037;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
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
	static const int32 RequiredParametersCount = 2;
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
	static const int32 ActionTypeId = 10042;
	static const int32 RequiredParametersCount = 2;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};

#pragma endregion

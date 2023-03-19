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
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
	{
		// Default Interface call is empty.
	}
	 /**
	  * If possible action should be able to revert, whatever kind of operation it did,
	  * if it's provided with the same data.
	  */
	 virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
	 {
		 // Default Interface call is empty.
	 }
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
	 TArray<FUDActionData> GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
	 {
		 return TArray<FUDActionData>();
	 }
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
public:
	static const int32 ActionTypeId = 0;
};

/**
 * Notification for new Player/Ai being part of the game.
 * This allows new player to receive history and create list of all players.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDAddPlayerAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 1;
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
public:
	static 	const int32 ActionTypeId = 2;
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
public:
	static 	const int32 ActionTypeId = 3;
};

/**
 * End of turn action that is called as action.
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
public:
	static const int32 ActionTypeId = 4;
};

/**
 * Generates income for all players. 
 * Called only on GAIA turn.
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
public:
	static const int32 ActionTypeId = 100;
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
 public:
	 static const int32 ActionTypeId = 1000;
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
public:
	static const int32 ActionTypeId = 1001;
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
public:
	static const int32 ActionTypeId = 1002;
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
public:
	static const int32 ActionTypeId = 1003;
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
	virtual void SetWorldGenerator(TObjectPtr<UUDWorldGenerator> worldGenerator) override
	{
		WorldGenerator = worldGenerator;
	}
public:
	static const int32 ActionTypeId = 66600;
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
public:
	static const int32 ActionTypeId = 5004;
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
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager) override
	{
		ModifierManager = modifierManager;
	}
public:
	static const int32 ActionTypeId = 5008;
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
public:
	static const int32 ActionTypeId = 1004;
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
public:
	static const int32 ActionTypeId = 1005;
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
public:
	static const int32 ActionTypeId = 1006;
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
	virtual void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager) override
	{
		ModifierManager = modifierManager;
	}
public:
	static const int32 ActionTypeId = 1007;
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
public:
	static const int32 ActionTypeId = 10000;
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
public:
	static const int32 ActionTypeId = 10001;
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
public:
	static const int32 ActionTypeId = 10002;
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
public:
	static const int32 ActionTypeId = 10003;
};

/**
 * Leave deal and loose participation, same as rejecting, but can be done at any point, if player is fed up with it.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDLeaveParticipationDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 10004;
};

/**
 * Moderator is able to close deal, making all actions for it invalid and deak has no result.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDCloseDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 10005;
};

///**
// * Allows extending of the deal with new discussion point.
// */
//UCLASS()
//class UNREALDIPLOMACY_API UUDAddDiscussionPointDealAction : public UObject, public IUDActionInterface
//{
//	GENERATED_BODY()
//public:
//	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual int32 GetActionTypeId() override { return ActionTypeId; };
//public:
//	static const int32 ActionTypeId = 10006;
//};
//
///**
// * Removes discussion point from the deal.
// */
//UCLASS()
//class UNREALDIPLOMACY_API UUDRemoveDiscussionPointDealAction : public UObject, public IUDActionInterface
//{
//	GENERATED_BODY()
//public:
//	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual int32 GetActionTypeId() override { return ActionTypeId; };
//public:
//	static const int32 ActionTypeId = 10007;
//};
///**
// * Updates discussion point from the deal.
// */
//UCLASS()
//class UNREALDIPLOMACY_API UUDUpdateDiscussionPointDealAction : public UObject, public IUDActionInterface
//{
//	GENERATED_BODY()
//public:
//	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
//	virtual int32 GetActionTypeId() override { return ActionTypeId; };
//public:
//	static const int32 ActionTypeId = 10007;
//};
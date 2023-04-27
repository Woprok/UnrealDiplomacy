// Copyright Miroslav Valach

#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/UDGlobalData.h"
// Actions
#include "Core/Simulation/Actions/UDActions.h"

TScriptInterface<IUDActionInterface> UUDActionManager::GetAction(int32 actionTypeId)
{
	check(IsInitialized);
	if (!Actions.Contains(actionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Action(%d) is not defined!"), actionTypeId);
		return Actions[UUDGlobalData::InvalidActionId];
	}

	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Retrieved Action(%d)."), actionTypeId);
	return Actions[actionTypeId];
}

bool UUDActionManager::RegisterAction(TScriptInterface<IUDActionInterface> newAction)
{
	check(IsInitialized);
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Duplicate registration of Action(%d)."), newAction->GetActionTypeId());
		return false;
	}

	BindSharedToAction(newAction);

	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Action(%d) successfully registered."), newAction->GetActionTypeId());
	Actions.Add(newAction->GetActionTypeId(), newAction);
	return true;
}

void UUDActionManager::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Initializing new instance of ActionManager."));
	check(!IsInitialized);
	IsInitialized = true;

	WorldGenerator = NewObject<UUDWorldGenerator>(this);
	ModifierManager = NewObject<UUDModifierManager>(this);

	RegisterCoreActions();
	RegisterDefaultActions();
}

void UUDActionManager::RegisterCoreActions()
{
	TArray<TScriptInterface<IUDActionInterface>> coreActionList = {
		NewObject<UUDSystemActionInvalid>(this),
		NewObject<UUDSystemActionLog>(this),
		NewObject<UUDSystemActionGameStart>(this),
		NewObject<UUDSystemActionGameEnd>(this),
		NewObject<UUDSystemActionPlayerAdd>(this),
		NewObject<UUDSystemActionPlayerRemove>(this),
		NewObject<UUDSystemActionTurnEnd>(this),
		NewObject<UUDSystemActionTurnForceEnd>(this),
		NewObject<UUDSystemActionWorldCreate>(this),
	};
	RegisterActionList(coreActionList);
}

void UUDActionManager::RegisterDefaultActions()
{
	TArray<TScriptInterface<IUDActionInterface>> defaultActionList = {

	};
	RegisterActionList(defaultActionList);
}

void UUDActionManager::RegisterActionList(TArray<TScriptInterface<IUDActionInterface>> actionList)
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Attempting to register action list."));

	int32 failCount = 0;
	for (auto& action : actionList)
	{
		if (!RegisterAction(action))
		{
			++failCount;
		}
	}

	UE_LOG(LogTemp, Log, 
		TEXT("UUDActionManager: Registering action list finished with %d failures out of %d actions."),
		failCount,
		actionList.Num());
}

void UUDActionManager::BindSharedToAction(TScriptInterface<IUDActionInterface> newAction)
{
	newAction->SetWorldGenerator(WorldGenerator);
	newAction->SetModifierManager(ModifierManager);
}
// Copyright Miroslav Valach

#include "Core/Simulation/UDActionInterface.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDResourceManager.h"

bool IUDActionInterface::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Verifying...\n%s\n%s"), 
		world->FactionPerspective, *action.ToString(), *ToString());
	
	bool isActionSame = action.ActionTypeId == GetId();
	bool isParameterCountSame = action.ValueParameters.Num() == GetParameterCount();
	if (GetPresentation().Tags.Contains(UD_ACTION_TAG_VERIFY_PARAMETER_MINIMUM))
	{
		isParameterCountSame = action.ValueParameters.Num() >= GetParameterCount();
	}
	return isActionSame && isParameterCountSame;
}	 

void IUDActionInterface::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Executing %s"), 
		world->FactionPerspective, *action.ToString());
}

void IUDActionInterface::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Reverting %s"), 
		world->FactionPerspective, *action.ToString());
}

int32 IUDActionInterface::GetId() const
{
	return UUDGlobalData::InvalidActionId;
}

int32 IUDActionInterface::GetParameterCount() const
{
	return UUDGlobalData::DefaultActionParameterCount;
}

bool IUDActionInterface::HasContinuations() const
{
	return false;
}

TArray<FUDActionData> IUDActionInterface::GetContinuations(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	return { };
}

bool IUDActionInterface::IsBackupRequired() const
{
	return false;
}

void IUDActionInterface::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return;
}

void IUDActionInterface::SetWorldGenerator(TObjectPtr<UUDWorldGenerator> generator)
{
	return;
}

void IUDActionInterface::SetModifierManager(TObjectPtr<UUDModifierManager> manager)
{
	return;
}

void IUDActionInterface::SetResourceManager(TObjectPtr<UUDResourceManager> manager)
{
	return;
}

FString IUDActionInterface::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("aid"), GetId());
	formatArgs.Add(TEXT("values"), GetParameterCount());

	FString formatted = FString::Format(TEXT("Action Executor[ID={aid}, OPTIONAL={values}]"), formatArgs);

	return formatted;
}

#define LOCTEXT_NAMESPACE "ActionInterface"
FUDActionPresentation IUDActionInterface::GetPresentation() const
{
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ActionInterface", "Invalid Action")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_INVALID,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE
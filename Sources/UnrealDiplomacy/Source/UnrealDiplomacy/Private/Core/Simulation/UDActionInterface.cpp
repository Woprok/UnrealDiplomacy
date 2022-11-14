// Copyright Miroslav Valach


#include "Core/Simulation/UDActionInterface.h"

void IUDActionInterface::Execute(FUDActionData actionData)
{
}

void IUDActionInterface::Revert(FUDActionData actionData)
{
}

int32 IUDActionInterface::GetActionTypeId()
{
	return -1;
}

#pragma region UUDLogAction

void UUDLogAction::Execute(FUDActionData actionData)
{
	UE_LOG(LogTemp, Log,
		TEXT("ActionData with id(%d), was passed to executor with id(%d)."),
		actionData.ActionTypeId, ActionTypeId);
}

void UUDLogAction::Revert(FUDActionData actionData)
{

}

int32 UUDLogAction::GetActionTypeId()
{
	return ActionTypeId;
}

#pragma endregion

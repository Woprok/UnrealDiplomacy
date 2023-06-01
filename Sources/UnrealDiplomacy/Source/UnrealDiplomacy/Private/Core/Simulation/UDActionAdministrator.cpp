// Copyright Miroslav Valach

#include "Core/Simulation/UDActionAdministrator.h"

TObjectPtr<UUDMapState> UUDActionAdministrator::GetMapState()
{
	return OverseeingState->Map;
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId)
{
	return FUDActionData(actionId, OverseeingState->FactionPerspective);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, TArray<int32> optionalValues)
{
	return FUDActionData(actionId, OverseeingState->FactionPerspective, optionalValues);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, TArray<int32> optionalValues, FString optionaString)
{
	return FUDActionData(actionId, OverseeingState->FactionPerspective, optionalValues, optionaString);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, FString optionaString)
{
	return FUDActionData(actionId, OverseeingState->FactionPerspective, optionaString);
}

FUDActionData UUDActionAdministrator::GetAcceptAction(int32 actionId, FUDActionData sourceAction)
{
	return FUDActionData::AsSuccessorOf(sourceAction, actionId);
}

FUDActionData UUDActionAdministrator::GetRejectAction(int32 actionId, FUDActionData sourceAction)
{
	return FUDActionData::AsSuccessorOf(sourceAction, actionId);
}
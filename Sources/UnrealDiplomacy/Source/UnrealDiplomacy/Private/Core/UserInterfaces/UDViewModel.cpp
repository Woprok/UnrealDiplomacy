// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"

void UUDViewModel::SetModel(TObjectPtr<UUDActionAdministrator> model)
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Model successfully set."));
	Model = model;
}

void UUDViewModel::Setup()
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Used Default Setup."));
}

void UUDViewModel::Refresh()
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Used Default Refresh."));
}
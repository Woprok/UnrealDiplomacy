// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"

void UUDViewModel::SetModel(TObjectPtr<UUDActionAdministrator> model)
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Model successfully set."));
	Model = model;
}

void UUDViewModel::FullUpdate()
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Full Update."));
	OnUpdateStarting.Broadcast();
	Update();
	OnUpdateFinishing.Broadcast();
}

void UUDViewModel::Update()
{
	return;
}
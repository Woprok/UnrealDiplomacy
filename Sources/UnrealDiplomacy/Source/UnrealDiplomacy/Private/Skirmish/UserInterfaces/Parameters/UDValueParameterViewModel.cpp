// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "ValueParameter"

void UUDValueParameterViewModel::Initialize()
{

}

void UUDValueParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDValueParameterViewModel::SetContent(FUDValueParameter content)
{
	Content = content;
}
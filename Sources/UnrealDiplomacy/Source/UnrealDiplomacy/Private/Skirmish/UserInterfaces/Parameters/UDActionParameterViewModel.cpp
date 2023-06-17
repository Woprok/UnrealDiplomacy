// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "ActionParameter"

void UUDActionParameterViewModel::Initialize()
{

}

void UUDActionParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDActionParameterViewModel::SetContent(FUDActionParameter content)
{
	Content = content;
}

int32 UUDActionParameterViewModel::GetAsValue()
{
	// TODO
	return -1;
}
// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "FactionParameter"

void UUDFactionParameterViewModel::Initialize()
{

}

void UUDFactionParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDFactionParameterViewModel::SetContent(FUDFactionParameter content)
{
	Content = content;
}

int32 UUDFactionParameterViewModel::GetAsValue()
{
	// TODO
	return -1;
}
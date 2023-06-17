// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "ResourceParameter"

void UUDResourceParameterViewModel::Initialize()
{

}

void UUDResourceParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDResourceParameterViewModel::SetContent(FUDResourceParameter content)
{
	Content = content;
}

int32 UUDResourceParameterViewModel::GetAsValue()
{
	// TODO
	return -1;
}
// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "TextParameter"

void UUDTextParameterViewModel::Initialize()
{

}

void UUDTextParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDTextParameterViewModel::SetContent(FUDTextParameter content)
{
	Content = content;
}

FString UUDTextParameterViewModel::GetAsText()
{
	// TODO
	return TEXT("");
}
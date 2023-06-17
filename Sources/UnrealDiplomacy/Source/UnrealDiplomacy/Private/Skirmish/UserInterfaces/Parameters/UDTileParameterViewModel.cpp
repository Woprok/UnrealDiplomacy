// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "TileParameter"

void UUDTileParameterViewModel::Initialize()
{

}

void UUDTileParameterViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDTileParameterViewModel::SetContent(FUDTileParameter content)
{
	Content = content;
}

TArray<int32> UUDTileParameterViewModel::GetAsValueRange()
{
	// TODO
	return { -1, -1 };
}
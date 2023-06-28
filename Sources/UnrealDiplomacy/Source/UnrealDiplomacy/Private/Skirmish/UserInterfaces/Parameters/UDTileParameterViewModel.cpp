// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"

#define LOCTEXT_NAMESPACE "TileParameter"

FUDTileMinimalInfo GetInvalidTile()
{
	FUDTileMinimalInfo invalid = FUDTileMinimalInfo();
	invalid.Name = FText(LOCTEXT("TileParameter", "Invalid")).ToString();
	invalid.Position = FIntPoint(UUDGlobalData::InvalidTilePositionX, UUDGlobalData::InvalidTilePositionY);
	return invalid;
}

void UUDTileParameterViewModel::Initialize()
{
	FText tileTitle = FText(LOCTEXT("TileParameter", "Tile"));
	SetTileTitleText(tileTitle);
	FText previousText = FText(LOCTEXT("TileParameter", "<"));
	SetPreviousText(previousText);
	FText nextText = FText(LOCTEXT("TileParameter", ">"));
	SetNextText(nextText);
	SelectedTile = GetInvalidTile().Position;
	SetNameText(FText::FromString(GetInvalidTile().Name));
	SelectedTileIndex = UUDGlobalData::InvalidParameterArrayIndex;
}

void UUDTileParameterViewModel::Update()
{
	SetTileTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));

	FUDTileMinimalInfo selected;
	if (Content.HasCurrentValue)
	{
		selected = GetSelectedOrDefault(Content.CurrentValue);
	}
	else
	{
		selected = GetSelectedOrDefault(SelectedTile);
	}
	SelectedTile = selected.Position;
	SetNameText(FText::FromString(selected.Name));
	ChangeAttempted();
}

void UUDTileParameterViewModel::ChangeAttempted()
{
	if (Content.HasCurrentValue && Content.CurrentValue != GetAsValuePoint())
	{
		OnChangeEvent.Broadcast();
	}
}

FUDTileMinimalInfo UUDTileParameterViewModel::GetSelectedOrDefault(FIntPoint desiredSelectedItem)
{
	FUDTileMinimalInfo selected = GetInvalidTile();
	SelectedTileIndex = UUDGlobalData::InvalidParameterArrayIndex;
	// Find
	const auto& found = Content.Options.FindByPredicate(
		[&desiredSelectedItem](const FUDTileMinimalInfo& item) { return item.Position == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedTileIndex = Content.Options.Find(selected);
	}
	else if (Content.Options.Num() > 0)
	{
		selected = Content.Options[0];
		SelectedTileIndex = 0;
	}

	return selected;
}

#undef LOCTEXT_NAMESPACE

void UUDTileParameterViewModel::PreviousTile()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedTileIndex - distance >= 0)
	{
		SelectedTileIndex -= distance;
		FUDTileMinimalInfo selected = Content.Options[SelectedTileIndex];
		SelectedTile = selected.Position;
		SetNameText(FText::FromString(selected.Name));
		ChangeAttempted();
	}
}

void UUDTileParameterViewModel::NextTile()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedTileIndex + distance < Content.Options.Num())
	{
		SelectedTileIndex += distance;
		FUDTileMinimalInfo selected = Content.Options[SelectedTileIndex];
		SelectedTile = selected.Position;
		SetNameText(FText::FromString(selected.Name));
		ChangeAttempted();
	}
}

void UUDTileParameterViewModel::SetContent(FUDTileParameter content)
{
	Content = content;
}

TArray<int32> UUDTileParameterViewModel::GetAsValueRange()
{
	const auto point = GetAsValuePoint();
	return { point.X, point.Y };
}

FIntPoint UUDTileParameterViewModel::GetAsValuePoint()
{
	return SelectedTile;
}

void UUDTileParameterViewModel::SetTileTitleText(FText newTileTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(TileTitleText, newTileTitleText);
}

FText UUDTileParameterViewModel::GetTileTitleText() const
{
	return TileTitleText;
}

void UUDTileParameterViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDTileParameterViewModel::GetNameText() const
{
	return NameText;
}

void UUDTileParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDTileParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDTileParameterViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDTileParameterViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDTileParameterViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDTileParameterViewModel::GetNextText() const
{
	return NextText;
}
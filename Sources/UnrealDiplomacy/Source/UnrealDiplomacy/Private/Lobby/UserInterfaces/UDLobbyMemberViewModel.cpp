// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"
#include "Lobby/UserInterfaces/UDStrategyOptionViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDSettingActionFactionRename.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "LobbyMember"

void UUDLobbyMemberViewModel::Initialize()
{
	StratagemViewModelType = UUDStrategyOptionViewModel::StaticClass();

	FText memberSettingsTitle = FText(LOCTEXT("LobbyMember", "Faction Settings"));
	SetMemberSettingsTitleText(memberSettingsTitle);
	FText factionName = FText(LOCTEXT("LobbyMember", "Faction Name"));
	SetFactionNameText(factionName);
	FText strategy = FText(LOCTEXT("LobbyMember", "Stratagems"));
	SetStrategyText(strategy);
	FText strategyPoints = FText(LOCTEXT("LobbyMember", "Stratagem Points left 0"));
	SetStrategyPointsText(strategyPoints);
	//FText nationNameEditable = FText(LOCTEXT("LobbyMember", "Generic Nation Name"));
	//SetFactionNameEditableText(nationNameEditable);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDLobbyMemberViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDLobbyMemberViewModel::Update);
}

void UUDLobbyMemberViewModel::Reload()
{
	Update();
}

void UUDLobbyMemberViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	if (!IsNameEditing)
	{
		FString name = Model->GetLocalFactionName();
		SetFactionNameEditableText(FText::FromString(name));
	}
	UpdateStratagemsList();
	UpdateStratagemPoints();
}

void UUDLobbyMemberViewModel::UpdateStratagemPoints()
{
	int32 stratagemsLeft = Model->GetLocalStratagemPointsLeft();

	FText strategyPoints = FText::Format(LOCTEXT("LobbyMember", "Stratagem Points left {0}"), stratagemsLeft);
	SetStrategyPointsText(strategyPoints);
}

#undef LOCTEXT_NAMESPACE

void UUDLobbyMemberViewModel::StartNameEditation(const FText& Text)
{
	IsNameEditing = true;
}

void UUDLobbyMemberViewModel::StopNameEditation(const FText& Text, ETextCommit::Type CommitMethod)
{
	IsNameEditing = false;
	// Update only if name was changed.
	FString oldName = Model->GetLocalFactionName();
	// TODO find source of the bug that causes the binding for EditableText to not update automatically.
	FString newName = Text.ToString();
	if (oldName != newName)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionFactionRename::ActionTypeId, newName));
	}
}

void UUDLobbyMemberViewModel::UpdateStratagemsList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDLobbyMemberViewModel: UpdateStratagemsList."));
	// Retrieve all options to rebuild the list
	TArray<FUDStratagemPickableInfo> stratagems = Model->GetStratagemsList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(StratagemViewModelCollectionName, StratagemViewModelType, stratagems.Num());
	// Get rid of all models
	StratagemViewModelCollection.Empty();
	for (int32 i = 0; i < stratagems.Num(); i++)
	{
		TObjectPtr<UUDStrategyOptionViewModel> newViewModel = Cast<UUDStrategyOptionViewModel>(viewModels[i]);
		newViewModel->SetContent(stratagems[i]);
		newViewModel->FullUpdate();
		StratagemViewModelCollection.Add(newViewModel);
	}

	StratagemSourceUpdatedEvent.Broadcast(StratagemViewModelCollection);
}

void UUDLobbyMemberViewModel::SetMemberSettingsTitleText(FText newMemberSettingsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MemberSettingsTitleText, newMemberSettingsTitleText);
}

FText UUDLobbyMemberViewModel::GetMemberSettingsTitleText() const
{
	return MemberSettingsTitleText;
}

void UUDLobbyMemberViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDLobbyMemberViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDLobbyMemberViewModel::SetStrategyText(FText newStrategyText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StrategyText, newStrategyText);
}

FText UUDLobbyMemberViewModel::GetStrategyText() const
{
	return StrategyText;
}

void UUDLobbyMemberViewModel::SetStrategyPointsText(FText newStrategyPointsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StrategyPointsText, newStrategyPointsText);
}

FText UUDLobbyMemberViewModel::GetStrategyPointsText() const
{
	return StrategyPointsText;
}

void UUDLobbyMemberViewModel::SetFactionNameEditableText(FText newFactionNameEditableText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameEditableText, newFactionNameEditableText);
}

FText UUDLobbyMemberViewModel::GetFactionNameEditableText() const
{
	return FactionNameEditableText;
}
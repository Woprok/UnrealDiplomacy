// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDMessageItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "MessageItem"

void UUDMessageItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("MessageItem", "Accept"));
	SetAcceptText(name);
}

void UUDMessageItemViewModel::Update()
{
	//FText name = FText::FromString(Content.Name);
	//SetNameText(name);
}

#undef LOCTEXT_NAMESPACE

void UUDMessageItemViewModel::SetContent()
{
	//Content = content;
}

void UUDMessageItemViewModel::Accept()
{
	// TODO execution of the action with all current parameters.
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Interact."));
	//Model->RequestAction(Model->GetAction(Content.ActionTypeId));
}

void UUDMessageItemViewModel::SetAcceptText(FText newAcceptText)
{
	UE_MVVM_SET_PROPERTY_VALUE(AcceptText, newAcceptText);
}

FText UUDMessageItemViewModel::GetAcceptText() const
{
	return AcceptText;
}
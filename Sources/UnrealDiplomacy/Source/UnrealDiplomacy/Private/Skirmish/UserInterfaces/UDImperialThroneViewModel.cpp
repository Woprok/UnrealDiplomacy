// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDImperialThroneViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/Simulation/Actions/UDGameActionThroneContest.h"
#include "Core/Simulation/Actions/UDGameActionThroneLiberate.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "ImperialThrone"

FText ConstructDealToolTip()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("ImperialThrone", "Deals allows you to discuss exchange with multiple participant at the same time.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("ImperialThrone", "The more complex deal, more reputation will each participant get for finishing it.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("ImperialThrone", ".")).ToString());
	return FText::FromString(content.ToString());
}

FText ConstructThroneToolTip()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("ImperialThrone", "Whoever is on the throne at the end of the game wins.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("ImperialThrone", "If noone is able to claim the throne before the end of the final turn,")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("ImperialThrone", "then the most reputable faction receives throne and wins.")).ToString());
	return FText::FromString(content.ToString());
}

void UUDImperialThroneViewModel::Setup()
{
	FText dealCount = FText::Format(LOCTEXT("ImperialThrone", "{0}"), 0);
	SetDealCountText(dealCount);
	FText messageCount = FText::Format(LOCTEXT("ImperialThrone", "{0}"), 0);
	SetMessageCountText(messageCount);
	FText throneText = FText(LOCTEXT("ImperialThrone", "Claim"));
	SetThroneText(throneText);
	FText throneToolTip = ConstructThroneToolTip();
	SetThroneToolTipText(throneToolTip);
	FText dealToolTip = ConstructDealToolTip();
	SetDealToolTipText(dealToolTip);
	SetCanInteractValue(false);
	SetThroneStateValue(EUDImperialThroneState::Undefined);
	SetIsEmptyValue(true);
	SetIsUsurperValue(false);
	SetIsClaimerValue(false);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDImperialThroneViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDImperialThroneViewModel::Refresh);
}

void UUDImperialThroneViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateThronePresentation();
}

void UUDImperialThroneViewModel::UpdateThronePresentation()
{
	FUDThroneInfo throneInfo = Model->GetThroneInfo();
	FUDAvailableDiplomacyInfo interactionInfo = Model->GetDiplomacyInfo();

	SetCanInteractValue(throneInfo.CanInteract);

	FText dealCount = FText::Format(LOCTEXT("ImperialThrone", "{0}"), interactionInfo.DealCount);
	SetDealCountText(dealCount);
	FText messageCount = FText::Format(LOCTEXT("ImperialThrone", "{0}"), interactionInfo.MessageCount);
	SetMessageCountText(messageCount);

	FText throneText;
	// This might not be the most elegant solution, e.g. creating new binding convertor, but it will work fine as temporary solution.
	SetIsEmptyValue(false);
	SetIsUsurperValue(false);
	SetIsClaimerValue(false);
	switch (throneInfo.State)
	{
	case EUDImperialThroneState::Empty:
		throneText = FText(LOCTEXT("ImperialThrone", "Claim"));
		SetIsEmptyValue(true);
		break;
	case EUDImperialThroneState::Usurper:
		throneText = FText::Format(LOCTEXT("ImperialThrone", "{0}"), FText::FromString(throneInfo.FactionName));
		SetIsUsurperValue(true);
		break;
	case EUDImperialThroneState::Claimer:
		throneText = FText(LOCTEXT("ImperialThrone", "Abdicate"));
		SetIsClaimerValue(true);
		break;
	}
	SetThroneText(throneText);
	SetThroneStateValue(throneInfo.State);
}

#undef LOCTEXT_NAMESPACE

void UUDImperialThroneViewModel::ThroneAction()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: ThroneAction."));
	if (!GetCanInteractValue())
	{
		return;
	}

	switch (ThroneStateValue)
	{
	case EUDImperialThroneState::Empty:
		ClaimThrone();
		break;
	case EUDImperialThroneState::Claimer:
		AbdicateThrone();
		break;
	case EUDImperialThroneState::Usurper:
		ContestThrone();
		break;
	}
}

void UUDImperialThroneViewModel::ClaimThrone()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: ClaimThrone."));
	if (Model->HasActionOrStratagem(UUDGameActionThroneUsurp::ActionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: Usurping attempted!"));
		Model->RequestAction(Model->GetAction(UUDGameActionThroneUsurp::ActionTypeId));
	}
}

void UUDImperialThroneViewModel::AbdicateThrone()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: AbdicateThrone."));
	if (Model->HasActionOrStratagem(UUDGameActionThroneAbdicate::ActionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: Abdicating attempted!"));
		Model->RequestAction(Model->GetAction(UUDGameActionThroneAbdicate::ActionTypeId));
	}
}

void UUDImperialThroneViewModel::ContestThrone()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: ContestThrone."));

	if (Model->HasActionOrStratagem(UUDGameActionThroneContest::ActionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: Contesting Throne attempted!"));
		Model->RequestAction(Model->GetAction(UUDGameActionThroneContest::ActionTypeId));
	}
	else if (Model->HasActionOrStratagem(UUDGameActionThroneLiberate::ActionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: Liberating Throne attempted!"));
		Model->RequestAction(Model->GetAction(UUDGameActionThroneLiberate::ActionTypeId));
	}
}

void UUDImperialThroneViewModel::OpenDeals()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: OpenDeals."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->ShowWidget(hud->DealManagementWidget);
}

void UUDImperialThroneViewModel::OpenMessages()
{
	UE_LOG(LogTemp, Log, TEXT("UUDImperialThroneViewModel: OpenMessages."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->ShowWidget(hud->MessageManagementWidget);
}

void UUDImperialThroneViewModel::SetDealCountText(FText newDealCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealCountText, newDealCountText);
}

FText UUDImperialThroneViewModel::GetDealCountText() const
{
	return DealCountText;
}

void UUDImperialThroneViewModel::SetMessageCountText(FText newMessageCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageCountText, newMessageCountText);
}

FText UUDImperialThroneViewModel::GetMessageCountText() const
{
	return MessageCountText;
}

void UUDImperialThroneViewModel::SetThroneText(FText newThroneText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ThroneText, newThroneText);
}

FText UUDImperialThroneViewModel::GetThroneText() const
{
	return ThroneText;
}

void UUDImperialThroneViewModel::SetThroneToolTipText(FText newThroneToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ThroneToolTipText, newThroneToolTipText);
}

FText UUDImperialThroneViewModel::GetThroneToolTipText() const
{
	return ThroneToolTipText;
}

void UUDImperialThroneViewModel::SetDealToolTipText(FText newDealToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealToolTipText, newDealToolTipText);
}

FText UUDImperialThroneViewModel::GetDealToolTipText() const
{
	return DealToolTipText;
}

void UUDImperialThroneViewModel::SetCanInteractValue(bool newCanInteractValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanInteractValue, newCanInteractValue);
}

bool UUDImperialThroneViewModel::GetCanInteractValue() const
{
	return CanInteractValue;
}

void UUDImperialThroneViewModel::SetThroneStateValue(EUDImperialThroneState newThroneStateValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ThroneStateValue, newThroneStateValue);
}

EUDImperialThroneState UUDImperialThroneViewModel::GetThroneStateValue() const
{
	return ThroneStateValue;
}

void UUDImperialThroneViewModel::SetIsEmptyValue(bool newIsEmptyValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsEmptyValue, newIsEmptyValue);
}

bool UUDImperialThroneViewModel::GetIsEmptyValue() const
{
	return IsEmptyValue;
}

void UUDImperialThroneViewModel::SetIsUsurperValue(bool newIsUsurperValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsUsurperValue, newIsUsurperValue);
}

bool UUDImperialThroneViewModel::GetIsUsurperValue() const
{
	return IsUsurperValue;
}

void UUDImperialThroneViewModel::SetIsClaimerValue(bool newIsClaimerValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsClaimerValue, newIsClaimerValue);
}

bool UUDImperialThroneViewModel::GetIsClaimerValue() const
{
	return IsClaimerValue;
}
// Copyright Miroslav Valach

#include "Menu/UDExceptionManagerSubsystem.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"
#include "Menu/UserInterfaces/UDDialogueViewModel.h"
#include "Core/UDSessionSubsystem.h"

#define LOCTEXT_NAMESPACE "ExceptionManager"

FUDDialogueData GetTravelError()
{
	return FUDDialogueData(
		EUDDialogueContentType::Error,
		FText(LOCTEXT("ExceptionManager", "Travel Error")).ToString(),
		FText(LOCTEXT("ExceptionManager", "Error occured during the travel. Returned to menu.")).ToString()
	);
}
FUDDialogueData GetNetworkError()
{
	return FUDDialogueData(
		EUDDialogueContentType::Error,
		FText(LOCTEXT("ExceptionManager", "Network Error")).ToString(),
		FText(LOCTEXT("ExceptionManager", "Connection dropped. Returned to menu.")).ToString()
	);
}

#undef LOCTEXT_NAMESPACE

TObjectPtr<UUDExceptionManagerSubsystem> UUDExceptionManagerSubsystem::Get(TObjectPtr<UWorld> world)
{
	UE_LOG(LogTemp, Log, TEXT("UUDExceptionManagerSubsystem: Retrieving Subsystem from GameInstance."));
	TObjectPtr<UUDGameInstance> gameInstance = UUDGameInstance::Get(world);
	TObjectPtr<UUDExceptionManagerSubsystem> exceptionManagerSubsystem = gameInstance->GetSubsystem<UUDExceptionManagerSubsystem>();
	check(exceptionManagerSubsystem != nullptr);
	return exceptionManagerSubsystem;
}

void UUDExceptionManagerSubsystem::ShowDialogue(FUDDialogueData data)
{
	TObjectPtr<AUDHUD> hud = AUDHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> dialogueViewModel = hud->ShowWidget(DialogueWidget);

	if (!dialogueViewModel)
	{
		UE_LOG(LogTemp, Log, TEXT("UUDExceptionManagerSubsystem: Dialogue is not instanced on current HUD."));
	}

	switch (data.ContentType)
	{
	case EUDDialogueContentType::Info:
		unimplemented();
		break;
	case EUDDialogueContentType::Warning:
		unimplemented();
		break;
	case EUDDialogueContentType::Error:
		CastChecked<UUDDialogueViewModel>(dialogueViewModel)->SetErrorDialogueContent(data.TitleText, data.ContentText);
		break;
	default:
		unimplemented();
		break;
	}
}

void UUDExceptionManagerSubsystem::HideDialogue()
{
	IsDialogueStored = false;
	TObjectPtr<AUDHUD> hud = AUDHUD::Get(GetWorld());
	hud->HideWidget(DialogueWidget);
}

void UUDExceptionManagerSubsystem::ShowStoredDialogue()
{
	if (IsDialogueStored)
		ShowDialogue(LastStoredDialogue);
}

void UUDExceptionManagerSubsystem::StoreDialogue(FUDDialogueData data)
{
	IsDialogueStored = true;
	LastStoredDialogue = data;
}

void UUDExceptionManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Keep an eye on any network or server travel failures
	GEngine->OnTravelFailure().AddUObject(this, &UUDExceptionManagerSubsystem::OnTravelFailureMessages);
	GEngine->OnNetworkFailure().AddUObject(this, &UUDExceptionManagerSubsystem::OnNetworkFailureMessages);
}

void UUDExceptionManagerSubsystem::OnSessionFailure(FUDDialogueData errorData)
{
	// Asks politely for session to be ended.
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->LeaveSession();
	// After returning we should be able to show dialogue
	StoreDialogue(errorData);
}

void UUDExceptionManagerSubsystem::OnTravelFailureMessages(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Log, TEXT("UUDExceptionManagerSubsystem: Travel Failure."));
	UE_LOG(LogTemp, Log, TEXT("Travel Failure: [%s]: %s"), ETravelFailure::ToString(FailureType), *ErrorString);
	OnSessionFailure(GetTravelError());
}

void UUDExceptionManagerSubsystem::OnNetworkFailureMessages(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Log, TEXT("UUDExceptionManagerSubsystem: Network Failure."));
	UE_LOG(LogTemp, Log, TEXT("Network Failure: %s[%s]: %s"), NetDriver ? *NetDriver->NetDriverName.ToString() : TEXT("NULL"), ENetworkFailure::ToString(FailureType), *ErrorString);
	OnSessionFailure(GetNetworkError());
}
// Copyright Miroslav Valach

#include "Menu/UDExceptionManagerSubsystem.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"
#include "Menu/UserInterfaces/UDDialogueViewModel.h"

TObjectPtr<UUDExceptionManagerSubsystem> UUDExceptionManagerSubsystem::Get(TObjectPtr<UWorld> world)
{
	UE_LOG(LogTemp, Log, TEXT("UUDSessionSubsystem: Retrieving Subsystem from GameInstance."));
	TObjectPtr<UUDGameInstance> gameInstance = UUDGameInstance::Get(world);
	TObjectPtr<UUDExceptionManagerSubsystem> exceptionManagerSubsystem = gameInstance->GetSubsystem<UUDExceptionManagerSubsystem>();
	check(exceptionManagerSubsystem != nullptr);
	return exceptionManagerSubsystem;
}

void UUDExceptionManagerSubsystem::ShowDialogue(FUDDialogueData data)
{
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> dialogueViewModel = hud->ShowWidget(DialogueWidget);
	switch (data.ContentType)
	{
	case EUDDialogueContentType::Info:
		unimplemented()
		break;
	case EUDDialogueContentType::Warning:
		unimplemented()
		break;
	case EUDDialogueContentType::Error:
		CastChecked<UUDDialogueViewModel>(dialogueViewModel)->SetErrorDialogueContent(data.TitleText, data.ContentText);
		break;
	default:
		unimplemented()
		break;
	}
}

void UUDExceptionManagerSubsystem::HideDialogue()
{
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->HideWidget(DialogueWidget);
}
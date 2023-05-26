// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDDialogueViewModel.h"
#include "Menu/UDExceptionManagerSubsystem.h"

#define LOCTEXT_NAMESPACE "Dialogue"

FText ConstructContent()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Dialogue", "You have encountered legendary bug.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText::FromString(TEXT("Consider reporting this, so we can squish it!")).ToString());
	return FText::FromString(content.ToString());
}

void UUDDialogueViewModel::Initialize()
{
	FText dialogueTitle = FText(LOCTEXT("Dialogue", "Error"));
	SetDialogueTitleText(dialogueTitle);
	FText content = ConstructContent();
	SetContentText(content);
	FText close = FText(LOCTEXT("Dialogue", "Close"));
	SetCloseText(close);
}

#undef LOCTEXT_NAMESPACE

void UUDDialogueViewModel::SetErrorDialogueContent(FString title, FString content)
{
	SetDialogueTitleText(FText::FromString(title));
	SetContentText(FText::FromString(content));
}

void UUDDialogueViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDialogueViewModel: Close."));
	UUDExceptionManagerSubsystem::Get(GetWorld())->HideDialogue();
}

void UUDDialogueViewModel::SetDialogueTitleText(FText newDialogueTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DialogueTitleText, newDialogueTitleText);
}

FText UUDDialogueViewModel::GetDialogueTitleText() const
{
	return DialogueTitleText;
}

void UUDDialogueViewModel::SetContentText(FText newContentText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ContentText, newContentText);
}

FText UUDDialogueViewModel::GetContentText() const
{
	return ContentText;
}

void UUDDialogueViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDDialogueViewModel::GetCloseText() const
{
	return CloseText;
}
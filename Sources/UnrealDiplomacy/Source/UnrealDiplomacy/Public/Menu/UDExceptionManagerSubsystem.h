// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UDExceptionManagerSubsystem.generated.h"

/**
 * Determines type of the dialogue content.
 */
UENUM(BlueprintType)
enum class EUDDialogueContentType : uint8
{
	/**
	 *
	 */
	Info,
	/**
	 *
	 */
	Warning,
	/**
	 *
	 */
	Error,
};

/**
 * Wrapper around items that will be in view.
 */
USTRUCT(BlueprintType)
struct FUDDialogueData
{
	GENERATED_BODY()
public:
	FUDDialogueData() { }
	FUDDialogueData(EUDDialogueContentType contentType, FString titleText, FString contentText)
		: ContentType(contentType), TitleText(titleText), ContentText(contentText) { }
	UPROPERTY(BlueprintReadOnly)
	EUDDialogueContentType ContentType;
	UPROPERTY(BlueprintReadOnly)
	FString TitleText;	
	UPROPERTY(BlueprintReadOnly)
	FString ContentText;
};

/**
 * Subsystem for handling error messages.
 * Primarily used by returning player to menu and showing error dialogue.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDExceptionManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted ExceptionManagerSubsystem from GameInstance.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<UUDExceptionManagerSubsystem> Get(TObjectPtr<UWorld> world);
	/**
	 * Displayes dialogue with the provided message.
	 */
	void ShowDialogue(FUDDialogueData data);
	/**
	 * Hides dialogue.
	 */
	void HideDialogue();
	/**
	 * Used by views to identify target widget.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName DialogueWidget = TEXT("Dialogue");
	/**
	 * Used by views to identify target screen.
	 */
	UPROPERTY(VisibleAnywhere, Category = "ScreenNames")
	FName DialogueScreen = TEXT("DialogueScreen");
};
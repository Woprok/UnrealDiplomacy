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
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDExceptionManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * Used to initialize custom game instance class.
	 * This is subscribing to network related events.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
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
	 * Displayes dialogue with the stored message.
	 * Stored message is removed.
	 */
	UFUNCTION(BlueprintCallable)
	void ShowStoredDialogue();
	/**
	 * Stores the message for future use.
	 * Primarily for use after world travel.
	 */
	void StoreDialogue(FUDDialogueData data);
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
private:
	/**
	 * Handles all cases for session being ended by host or connection / travel error.
	 */
	void OnSessionFailure(FUDDialogueData errorData);
	void OnTravelFailureMessages(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString);
	void OnNetworkFailureMessages(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	bool IsDialogueStored = false;
	FUDDialogueData LastStoredDialogue;
};
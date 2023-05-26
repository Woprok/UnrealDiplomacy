// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDServerItemViewModel.generated.h"

// Forward Declarations

class FOnlineSessionSearchResult;

/**
 * ViewModel for single server shown in join game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDServerItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the session.
	 */
	void SetContent(const FOnlineSessionSearchResult& content);
	/**
	 * Set content of the session.
	 */
	const FOnlineSessionSearchResult& GetContent() const;
public:
	// Button Functions
	/**
	 * Attempts to join the new session, hides UI until the end of join operation.
	 */
	UFUNCTION()
	void Join();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 PingValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabled;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Starts join process.
	 */
	void StartJoiningGame();
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetPingValue(int32 newPingValue);
	int32 GetPingValue() const;
	void SetIsEnabled(bool newIsEnabled);
	bool GetIsEnabled() const;
private:
	// Fields
	const FOnlineSessionSearchResult* Content;
	FName ContentName = TEXT("");
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDMenuViewModel.generated.h"

/**
 * ViewModel for displaying menu informations once game is opened.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDMenuViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString GameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CreateGameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString JoinGameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SettingsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString QuitText;
private:
	// MVVM Setters & Getters
	void SetGameTitleText(FString newGameTitleText);
	FString GetGameTitleText() const;
	void SetCreateGameText(FString newCreateGameText);
	FString GetCreateGameText() const;
	void SetJoinGameText(FString newJoinGameText);
	FString GetJoinGameText() const;
	void SetSettingsText(FString newSettingsText);
	FString GetSettingsText() const;
	void SetQuitText(FString newQuitText);
	FString GetQuitText() const;
};
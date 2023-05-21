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
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText GameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateGameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText JoinGameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SettingsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText QuitText;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetGameTitleText(FText newGameTitleText);
	FText GetGameTitleText() const;
	void SetCreateGameText(FText newCreateGameText);
	FText GetCreateGameText() const;
	void SetJoinGameText(FText newJoinGameText);
	FText GetJoinGameText() const;
	void SetSettingsText(FText newSettingsText);
	FText GetSettingsText() const;
	void SetQuitText(FText newQuitText);
	FText GetQuitText() const;
};
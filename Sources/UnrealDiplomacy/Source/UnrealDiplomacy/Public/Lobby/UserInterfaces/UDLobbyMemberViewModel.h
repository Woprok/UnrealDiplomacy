// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLobbyMemberViewModel.generated.h"

/**
 * ViewModel for defining player settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyMemberViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MemberSettingsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NationNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StrategyText;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NationNameEditableText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Update settings, based on current values.
	 */
	void ApplyNationNameChange();
private:
	// MVVM Setters & Getters
	void SetMemberSettingsTitleText(FText newMemberSettingsTitleText);
	FText GetMemberSettingsTitleText() const;
	void SetNationNameText(FText newNationNameText);
	FText GetNationNameText() const;
	void SetStrategyText(FText newStrategyText);
	FText GetStrategyText() const;
	void SetNationNameEditableText(FText newNationNameEditableText);
	FText GetNationNameEditableText() const;
};
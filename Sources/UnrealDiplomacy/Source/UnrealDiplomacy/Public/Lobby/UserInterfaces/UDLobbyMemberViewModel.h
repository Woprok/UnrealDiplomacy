// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "UDLobbyMemberViewModel.generated.h"

// Forward Declarations

enum ETextCommit::Type : int;
class UUDStrategyOptionViewModel;

/**
 * ViewModel for defining player settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyMemberViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Changes Commited
	UFUNCTION()
	void StartNameEditation(const FText& Text);
	UFUNCTION()
	void StopNameEditation(const FText& Text, ETextCommit::Type CommitMethod);
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MemberSettingsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StrategyText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StrategyPointsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameEditableText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList StrategyOptionList;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** Generates list for pickable options. */
	UFUNCTION()
	void UpdateStratagemsList();
	/** Recalculates and updates displayed string based on local player faction stratagems. */
	void UpdateStratagemPoints();
private:
	// MVVM Setters & Getters
	void SetMemberSettingsTitleText(FText newMemberSettingsTitleText);
	FText GetMemberSettingsTitleText() const;
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetStrategyText(FText newStrategyText);
	FText GetStrategyText() const;
	void SetStrategyPointsText(FText newStrategyPointsText);
	FText GetStrategyPointsText() const;
	void SetFactionNameEditableText(FText newFactionNameEditableText);
	FText GetFactionNameEditableText() const;
	void SetStrategyOptionList(FUDViewModelList newStrategyOptionList);
	FUDViewModelList GetStrategyOptionList() const;
private:
	// Fields
	bool IsNameEditing = false;
	FName StratagemViewModelCollectionName = TEXT("StratagemItemCollection");
	TSubclassOf<UUDViewModel> StratagemViewModelType;
	TArray<TObjectPtr<UUDStrategyOptionViewModel>> StratagemViewModelCollection;
};
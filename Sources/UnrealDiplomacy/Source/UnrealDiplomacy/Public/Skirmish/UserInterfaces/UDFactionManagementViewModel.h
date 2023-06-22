// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDFactionManagementViewModel.generated.h"

// Forward Declarations

class UUDFactionInteractionViewModel;
class UUDModifierItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDFactionInteractionSourceUpdated, const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionInteractionViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDFactionModifierItemSourceUpdated, const TArray<TObjectPtr<UUDModifierItemViewModel>>& modifierItemViewModels);

/**
 * Window that allows interacting with faction and view informations.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionManagementViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Self handle for faction selection event.
	 */
	UFUNCTION()
	void OnFactionSelected(int32 selectedFactionId);
public:
	// Button Functions
	UFUNCTION()
	void Close();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionManagementTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
	// Events
	FUDFactionInteractionSourceUpdated FactionInteractionSourceUpdatedEvent;
	FUDFactionModifierItemSourceUpdated ModifierItemSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates available interaction with the faction.
	 */
	void UpdateFactionInteractionList();
	/**
	 * Updates available modifiers with the faction.
	 */
	void UpdateModifierItemList();
private:
	// MVVM Setters & Getters
	void SetFactionManagementTitleText(FText newFactionManagementTitleText);
	FText GetFactionManagementTitleText() const;
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
private:
	// Fields
	FName FactionInteractionViewModelCollectionName = TEXT("FactionInteractionCollection");
	TSubclassOf<UUDViewModel> FactionInteractionViewModelType;
	TArray<TObjectPtr<UUDFactionInteractionViewModel>> FactionInteractionViewModelCollection;

	FName ModifierItemViewModelCollectionName = TEXT("FactionModifierItemCollection");
	TSubclassOf<UUDViewModel> ModifierItemViewModelType;
	TArray<TObjectPtr<UUDModifierItemViewModel>> ModifierItemViewModelCollection;

	int32 SelectedFactionId;
};
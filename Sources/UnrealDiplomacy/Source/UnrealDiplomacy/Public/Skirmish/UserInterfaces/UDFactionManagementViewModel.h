// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDFactionManagementViewModel.generated.h"

// Forward Declarations

class UUDFactionInteractionViewModel;
class UUDModifierItemViewModel;
class UUDPolicySelectorViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDPolicySelectorChanged, const TObjectPtr<UUDPolicySelectorViewModel>& policySelectorViewModel);

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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InteractionTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OfferTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RequestTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DemandTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList FactionInteractionList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList FactionOfferList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList FactionRequestList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList FactionDemandList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ModifierItemList;
	// Events
	FUDPolicySelectorChanged PolicySelectorChangedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates interactions with the faction.
	 */
	void UpdateFactionLists();
	/**
	 * Updates available direct interaction with the faction.
	 */
	void UpdateFactionInteractionList();
	/**
	 * Updates available offer interaction with the faction.
	 */
	void UpdateFactionOfferList();
	/**
	 * Updates available request interaction with the faction.
	 */
	void UpdateFactionRequestList();
	/**
	 * Updates available demand interaction with the faction.
	 */
	void UpdateFactionDemandList();
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
	void SetInteractionTitleText(FText newInteractionTitleText);
	FText GetInteractionTitleText() const;
	void SetOfferTitleText(FText newOfferTitleText);
	FText GetOfferTitleText() const;
	void SetRequestTitleText(FText newRequestTitleText);
	FText GetRequestTitleText() const;
	void SetDemandTitleText(FText newDemandTitleText);
	FText GetDemandTitleText() const;
	void SetFactionInteractionList(FUDViewModelList newFactionInteractionList);
	FUDViewModelList GetFactionInteractionList() const;
	void SetFactionOfferList(FUDViewModelList newFactionOfferList);
	FUDViewModelList GetFactionOfferList() const;
	void SetFactionRequestList(FUDViewModelList newFactionRequestList);
	FUDViewModelList GetFactionRequestList() const;
	void SetFactionDemandList(FUDViewModelList newFactionDemandList);
	FUDViewModelList GetFactionDemandList() const;
	void SetModifierItemList(FUDViewModelList newModifierItemList);
	FUDViewModelList GetModifierItemList() const;
private:
	// Fields
	FName FactionInteractionViewModelCollectionName = TEXT("FactionInteractionCollection");
	TSubclassOf<UUDViewModel> FactionInteractionViewModelType;
	TArray<TObjectPtr<UUDFactionInteractionViewModel>> FactionInteractionViewModelCollection;

	FName FactionOfferViewModelCollectionName = TEXT("FactionOfferCollection");
	TArray<TObjectPtr<UUDFactionInteractionViewModel>> FactionOfferViewModelCollection;

	FName FactionRequestViewModelCollectionName = TEXT("FactionRequestCollection");
	TArray<TObjectPtr<UUDFactionInteractionViewModel>> FactionRequestViewModelCollection;

	FName FactionDemandViewModelCollectionName = TEXT("FactionDemandCollection");
	TArray<TObjectPtr<UUDFactionInteractionViewModel>> FactionDemandViewModelCollection;

	FName ModifierItemViewModelCollectionName = TEXT("FactionModifierItemCollection");
	TSubclassOf<UUDViewModel> ModifierItemViewModelType;
	TArray<TObjectPtr<UUDModifierItemViewModel>> ModifierItemViewModelCollection;

	// Policies
	FName PolicySelectorInstanceName = TEXT("DecisionConsequencePolicySelectorInstance");
	TSubclassOf<UUDViewModel> PolicySelectorType;
	TObjectPtr<UUDPolicySelectorViewModel> PolicySelectorInstance;

	int32 SelectedFactionId;
};
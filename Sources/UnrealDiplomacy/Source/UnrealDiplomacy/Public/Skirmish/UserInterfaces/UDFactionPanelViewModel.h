// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDFactionPanelViewModel.generated.h"

// Forward Declarations

class UUDFactionItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDFactionSourceUpdated, const TArray<TObjectPtr<UUDFactionItemViewModel>>& factionItemViewModels);

/**
 * List that holds all factions.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionPanelViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameHeaderText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ControllerHeaderText;
	// Events
	FUDFactionSourceUpdated FactionSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates faction list.
	 */
	void UpdateFactionList();
private:
	// MVVM Setters & Getters
	void SetNameHeaderText(FText newNameHeaderText);
	FText GetNameHeaderText() const;
	void SetControllerHeaderText(FText newControllerHeaderText);
	FText GetControllerHeaderText() const;
private:
	// Fields
	FName FactionViewModelCollectionName = TEXT("FactionItemCollection");
	TSubclassOf<UUDViewModel> FactionViewModelType;
	TArray<TObjectPtr<UUDFactionItemViewModel>> FactionViewModelCollection;
};
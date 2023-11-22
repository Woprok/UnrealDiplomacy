// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "UDFactionPanelViewModel.generated.h"

// Forward Declarations

class UUDFactionItemViewModel;

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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList FactionItemList;
	// Events
public:
	virtual void Setup() override;
	UFUNCTION()
	virtual void Refresh() override;
private:
	/** Updates faction list. */
	void UpdateFactionList();
private:
	// MVVM Setters & Getters
	void SetNameHeaderText(FText newNameHeaderText);
	FText GetNameHeaderText() const;
	void SetControllerHeaderText(FText newControllerHeaderText);
	FText GetControllerHeaderText() const;
	void SetFactionItemList(FUDViewModelList newFactionItemList);
	FUDViewModelList GetFactionItemList() const;
private:
	// Fields
	FName FactionViewModelCollectionName = TEXT("FactionItemCollection");
	TSubclassOf<UUDViewModel> FactionViewModelType;
	TArray<TObjectPtr<UUDFactionItemViewModel>> FactionViewModelCollection;
};
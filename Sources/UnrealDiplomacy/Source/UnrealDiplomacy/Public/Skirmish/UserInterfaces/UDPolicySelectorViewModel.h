// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDPolicySelectorViewModel.generated.h"

// Forward Declarations

class UUDPolicySelectItemViewModel;

/**
 * Probides option to select from one of the options that are implemented as PolicyItemViewModel.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDPolicySelectorViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Defines content of the policy.
	 */
	UFUNCTION()
	void SetContent();
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PolicyTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList PolicyItemList;
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
	void UpdatePolicyItemList();
private:
	// MVVM Setters & Getters
	void SetPolicyTitleText(FText newPolicyTitleText);
	FText GetPolicyTitleText() const;
	void SetPolicyItemList(FUDViewModelList newPolicyItemList);
	FUDViewModelList GetPolicyItemList() const;
private:
	// Fields
	FName PolicySelectItemViewModelCollectionName = TEXT("PolicySelectItemCollection");
	TSubclassOf<UUDViewModel> PolicySelectItemViewModelType;
	TArray<TObjectPtr<UUDPolicySelectItemViewModel>> PolicySelectItemViewModelCollection;
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDResourcePanelViewModel.generated.h"

// Forward Declarations

class UUDResourceItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDResourceSourceUpdated, const TArray<TObjectPtr<UUDResourceItemViewModel>>& resourceItemViewModels);

/**
 * List that holds all resources.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDResourcePanelViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	// Events
	FUDResourceSourceUpdated ResourceSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates resource list.
	 */
	void UpdateResourceList();
private:
	// MVVM Setters & Getters
private:
	// Fields
	FName ResourceViewModelCollectionName = TEXT("ResourceItemCollection");
	TSubclassOf<UUDViewModel> ResourceViewModelType;
	TArray<TObjectPtr<UUDResourceItemViewModel>> ResourceViewModelCollection;
};
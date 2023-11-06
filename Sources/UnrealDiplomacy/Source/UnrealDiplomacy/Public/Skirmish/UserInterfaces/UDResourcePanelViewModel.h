// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDResourcePanelViewModel.generated.h"

// Forward Declarations

class UUDResourceItemViewModel;

/**
 * List that holds all resources.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDResourcePanelViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ResourceList;
	// Events
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
	void SetResourceList(FUDViewModelList newResourceList);
	FUDViewModelList GetResourceList() const;
private:
	// Fields
	FName ResourceViewModelCollectionName = TEXT("ResourceItemCollection");
	TSubclassOf<UUDViewModel> ResourceViewModelType;
	TArray<TObjectPtr<UUDResourceItemViewModel>> ResourceViewModelCollection;
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealItemViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDDealGeneralTabViewModel;
class UUDDealEditationTabViewModel;
class UUDDealExecutionTabViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDGeneralSourceUpdated, const TObjectPtr<UUDDealGeneralTabViewModel>& tabViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDEditationSourceUpdated, const TObjectPtr<UUDDealEditationTabViewModel>& tabViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDExecutionSourceUpdated, const TObjectPtr<UUDDealExecutionTabViewModel>& tabViewModel);

/**
 * Window for showing and editing deals.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter editor.
	 */
	void InvalidateContent(FUDDealMinimalInfo content);
	/**
	 * Set content of the parameter editor.
	 */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void General();
	UFUNCTION()
	void Editation();
	UFUNCTION()
	void Execution();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DealNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText GeneralText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EditationText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ExecutionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 SelectedTabValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	int32 GeneralTabValue = 0;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	int32 EditationTabValue = 1;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	int32 ExecutionTabValue = 2;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	bool IsValidContentValue = false;
	// Events
	FUDGeneralSourceUpdated GeneralSourceUpdatedEvent;
	FUDEditationSourceUpdated EditationSourceUpdatedEvent;
	FUDExecutionSourceUpdated ExecutionSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates general.
	 */
	void UpdateGeneralInstance();
	/**
	 * Updates editation.
	 */
	void UpdateEditationInstance();
	/**
	 * Updates execution.
	 */
	void UpdateExecutionInstance();
	/** Swift check to which of the tab is selected. */
	bool IsGeneral() { return SelectedTabValue == GeneralTabValue; };
	/** Swift check to which of the tab is selected. */
	bool IsEditation() { return SelectedTabValue == EditationTabValue; };
	/** Swift check to which of the tab is selected. */
	bool IsExecution() { return SelectedTabValue == ExecutionTabValue; };
private:
	// MVVM Setters & Getters
	void SetDealNameText(FText newDealNameText);
	FText GetDealNameText() const;
	void SetGeneralText(FText newGeneralText);
	FText GetGeneralText() const;
	void SetEditationText(FText newEditationText);
	FText GetEditationText() const;
	void SetExecutionText(FText newExecutionText);
	FText GetExecutionText() const;
	void SetSelectedTabValue(int32 newSelectedTabValue);
	int32 GetSelectedTabValue() const;
	int32 GetGeneralTabValue() const;
	int32 GetEditationTabValue() const;
	int32 GetExecutionTabValue() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
private:
	// Fields
	FUDDealMinimalInfo Content;

	// Current Instances in use...
	FName GeneralTabViewModelInstanceName = TEXT("GeneralTabInstance");
	TSubclassOf<UUDViewModel> GeneralTabViewModelType;
	TObjectPtr<UUDDealGeneralTabViewModel> GeneralTabViewModelInstance;
	FName EditationTabViewModelInstanceName = TEXT("EditationTabInstance");
	TSubclassOf<UUDViewModel> EditationTabViewModelType;
	TObjectPtr<UUDDealEditationTabViewModel> EditationTabViewModelInstance;
	FName ExecutionTabViewModelInstanceName = TEXT("ExecutionTabInstance");
	TSubclassOf<UUDViewModel> ExecutionTabViewModelType;
	TObjectPtr<UUDDealExecutionTabViewModel> ExecutionTabViewModelInstance;
};
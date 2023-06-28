// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealManagementViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
struct FUDDealListInfo;
class UUDDealItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDDealItemChanged, const TObjectPtr<UUDDealItemViewModel>& dealItemViewModel);

/**
 * Window for showing and editing deals.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealManagementViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void CreateDeal();
	UFUNCTION()
	void SwitchActiveDeals();
	UFUNCTION()
	void SwitchHistoryDeals();
	UFUNCTION()
	void Close();
	UFUNCTION()
	void First();
	UFUNCTION()
	void Previous();
	UFUNCTION()
	void Next();
	UFUNCTION()
	void Last();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DealManagementTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DealCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FirstText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PreviousText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NextText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LastText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ActiveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText HistoryText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 SelectedTabValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	int32 ActiveTabValue = 0;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	int32 HistoryTabValue = 1;
	// Events
	FUDDealItemChanged ActiveDealItemChangedEvent;
	FUDDealItemChanged HistoryDealItemChangedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates items.
	 */
	void UpdateDealItems();
	/**
	 * Updates selection.
	 */
	void UpdateSelectedDealItem();
	/**
	 * Attempts to select specified item.
	 */
	FUDDealMinimalInfo GetSelectedOrDefaultActive(int32 desiredSelectedItem);
	/**
	 * Attempts to select specified item.
	 */
	FUDDealMinimalInfo GetSelectedOrDefaultHistory(int32 desiredSelectedItem);
	/** Swift check to which of the tab is selected. */
	bool IsActive() { return SelectedTabValue == ActiveTabValue; };
	/** Swift check to which of the tab is selected. */
	bool IsHistory() { return SelectedTabValue == HistoryTabValue; };
private:
	// MVVM Setters & Getters
	void SetDealManagementTitleText(FText newDealManagementTitleText);
	FText GetDealManagementTitleText() const;
	void SetDealCountText(FText newDealCountText);
	FText GetDealCountText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
	void SetFirstText(FText newFirstText);
	FText GetFirstText() const;
	void SetPreviousText(FText newPreviousText);
	FText GetPreviousText() const;
	void SetNextText(FText newNextText);
	FText GetNextText() const;
	void SetLastText(FText newLastText);
	FText GetLastText() const;
	void SetCreateText(FText newCreateText);
	FText GetCreateText() const;
	void SetCreateToolTipText(FText newCreateToolTipText);
	FText GetCreateToolTipText() const;
	void SetActiveText(FText newActiveText);
	FText GetActiveText() const;
	void SetHistoryText(FText newHistoryText);
	FText GetHistoryText() const;
	void SetSelectedTabValue(int32 newSelectedTabValue);
	int32 GetSelectedTabValue() const;
	int32 GetActiveTabValue() const;
	int32 GetHistoryTabValue() const;
private:
	// Fields
	int32 SelectedActiveIndex;
	int32 SelectedHistoryIndex;
	FUDDealMinimalInfo SelectedActiveDealItem;
	FUDDealMinimalInfo SelectedHistoryDealItem;
	FUDDealListInfo Content;

	// Current Instance type is shared.
	TSubclassOf<UUDViewModel> DealItemType;
	// Current Active Instance in use...
	FName ActiveDealItemInstanceName = TEXT("ActiveDealItemInstance");
	TObjectPtr<UUDDealItemViewModel> ActiveDealItemInstance;
	// Current History Instance in use...
	FName HistoryDealItemInstanceName = TEXT("HistoryDealItemInstance");
	TObjectPtr<UUDDealItemViewModel> HistoryDealItemInstance;
};
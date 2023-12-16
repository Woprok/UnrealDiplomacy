// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDDealGeneralTabUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDDealGeneralTabViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealGeneralTabUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealGeneralTabViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeReadyButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeVoteButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> LeaveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CancelButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> EndAssembleButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> EndDefineButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> EndVoteButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> EndResolutionButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ExecuteContractButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealGeneralTabViewModel> ViewModel;
};
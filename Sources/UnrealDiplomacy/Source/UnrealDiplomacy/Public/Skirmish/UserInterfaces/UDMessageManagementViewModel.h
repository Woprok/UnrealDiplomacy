// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDMessageManagementViewModel.generated.h"

// Forward Declarations

struct FUDMessageInfo;
struct FUDMessageInteractionInfo;
class UUDMessageItemViewModel;

/**
 * Browsing of all interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDMessageManagementViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
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
	FText MessageManagementTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageCountText;
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
	FUDViewModelContent MessageItemContent;
public:
	virtual void Setup() override;
	UFUNCTION()
	virtual void Refresh() override;
private:
	/** Updates message item to latest. */
	void UpdateMessageItems();
	/** Updates message item to latest. */
	void UpdateSelectedMessageItem();
	/**
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If messages list is empty returns invalid item.
	 */
	FUDMessageInfo GetSelectedOrDefault(int32 desiredSelectedItem);
private:
	// MVVM Setters & Getters
	void SetMessageManagementTitleText(FText newMessageManagementTitleText);
	FText GetMessageManagementTitleText() const;
	void SetMessageCountText(FText newMessageCountText);
	FText GetMessageCountText() const;
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
	void SetMessageItemContent(FUDViewModelContent newMessageItemContent);
	FUDViewModelContent GetMessageItemContent() const;
private:
	// Fields
	int32 SelectedIndex;
	FUDMessageInfo SelectedMessageItem;
	FUDMessageInteractionInfo Content;
	// Current Instance in use...
	FName MessageItemInstanceName = TEXT("MessageItemInstance");
	TSubclassOf<UUDViewModel> MessageItemType;
	TObjectPtr<UUDMessageItemViewModel> MessageItemInstance;
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDMessageItemViewModel.generated.h"

// Forward Declarations

struct FUDMessageInfo;

/**
 * Single interactable item.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDMessageItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Reset content of the interaction option to invalid.
	 * Disabling interations.
	 */
	void InvalidateContent(FUDMessageInfo content);
	/** Set content of the interaction option. */
	void SetContent(FUDMessageInfo content);
public:
	// Button Functions
	/** Confirmation of the interaction. */
	UFUNCTION()
	void Accept();
	UFUNCTION()
	void Ok();
	UFUNCTION()
	void Reject();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageTypeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageTitleText;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageConfirmText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageRejectText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OnConfirmText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OnRejectText;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasRejectValue;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText AcceptText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OkText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RejectText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasContentValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasChoicesValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetMessageTypeText(FText newMessageTypeText);
	FText GetMessageTypeText() const;
	void SetMessageTitleText(FText newMessageTitleText);
	FText GetMessageTitleText() const;

	void SetMessageConfirmText(FText newMessageConfirmText);
	FText GetMessageConfirmText() const;
	void SetMessageRejectText(FText newMessageRejectText);
	FText GetMessageRejectText() const;
	void SetOnConfirmText(FText newOnConfirmText);
	FText GetOnConfirmText() const;
	void SetOnRejectText(FText newOnRejectText);
	FText GetOnRejectText() const;

	void SetHasRejectValue(bool newHasRejectValue);
	bool GetHasRejectValue() const;

	void SetAcceptText(FText newAcceptText);
	FText GetAcceptText() const;
	void SetOkText(FText newOkText);
	FText GetOkText() const;
	void SetRejectText(FText newRejectText);
	FText GetRejectText() const;
	void SetHasContentValue(bool newHasContentValue);
	bool GetHasContentValue() const;
	void SetHasChoicesValue(bool newHasChoicesValue);
	bool GetHasChoicesValue() const;
private:
	// Fields
	FUDMessageInfo Content;
};
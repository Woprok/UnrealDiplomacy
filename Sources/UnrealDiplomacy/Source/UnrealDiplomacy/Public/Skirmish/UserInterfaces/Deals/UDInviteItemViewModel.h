// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDInviteItemViewModel.generated.h"

// Forward Declarations

struct FUDDealFactionInfo;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDInviteItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the invite player. */
	void SetContent(FUDDealFactionInfo content);
public:
	// Button Functions
	UFUNCTION()
	void Invite();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InviteText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BlockedText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsInviteableValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetInviteText(FText newInviteText);
	FText GetInviteText() const;
	void SetBlockedText(FText newBlockedText);
	FText GetBlockedText() const;
	void SetIsInviteableValue(bool newIsInviteableValue);
	bool GetIsInviteableValue() const;
private:
	// Fields
	FUDDealFactionInfo Content;
};
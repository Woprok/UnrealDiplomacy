// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDImperialThroneViewModel.generated.h"

// Forward Declarations
enum class EUDImperialThroneState : uint8;

/**
 * Contains all data related presenting informations and actions related to starting diplomacy (deal, throne, messages).
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDImperialThroneViewModel : public UUDViewModel
{
	GENERATED_BODY()

public:
	// Button Functions
	UFUNCTION()
	void ThroneAction();
	UFUNCTION()
	void OpenDeals();
	UFUNCTION()
	void OpenMessages();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DealCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ThroneText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ThroneToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DealToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanInteractValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	EUDImperialThroneState ThroneStateValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEmptyValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsUsurperValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsClaimerValue;
public:
	virtual void Setup() override;
	UFUNCTION()
	virtual void Refresh() override;
private:
	UFUNCTION()
	void ClaimThrone();
	UFUNCTION()
	void AbdicateThrone();
	UFUNCTION()
	void ContestThrone();
	/** Updates all parts of throne presentation to latest change. */
	void UpdateThronePresentation();
private:
	// MVVM Setters & Getters
	void SetDealCountText(FText newDealCountText);
	FText GetDealCountText() const;
	void SetMessageCountText(FText newMessageCountText);
	FText GetMessageCountText() const;
	void SetThroneText(FText newThroneText);
	FText GetThroneText() const;
	void SetThroneToolTipText(FText newThroneToolTipText);
	FText GetThroneToolTipText() const;
	void SetDealToolTipText(FText newDealToolTipText);
	FText GetDealToolTipText() const;
	void SetCanInteractValue(bool newCanInteractValue);
	bool GetCanInteractValue() const;
	void SetThroneStateValue(EUDImperialThroneState newThroneStateValue);
	EUDImperialThroneState GetThroneStateValue() const;
	void SetIsEmptyValue(bool newIsEmptyValue);
	bool GetIsEmptyValue() const;
	void SetIsUsurperValue(bool newIsUsurperValue);
	bool GetIsUsurperValue() const;
	void SetIsClaimerValue(bool newIsClaimerValue);
	bool GetIsClaimerValue() const;
};
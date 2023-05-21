// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDCreditsViewModel.generated.h"

/**
 * ViewModel for displaying credits.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDCreditsViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Back();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreditsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ContentText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
protected:
	virtual void Initialize() override;
private:
	// MVVM Setters & Getters
	void SetCreditsTitleText(FText newCreditsTitleText);
	FText GetCreditsTitleText() const;
	void SetContentText(FText newContentText);
	FText GetContentText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
};
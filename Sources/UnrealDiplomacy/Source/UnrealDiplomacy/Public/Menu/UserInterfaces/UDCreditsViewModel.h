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
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreditsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ContentText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetCreditsTitleText(FText newCreditsTitleText);
	FText GetCreditsTitleText() const;
	void SetContentText(FText newContentText);
	FText GetContentText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
};
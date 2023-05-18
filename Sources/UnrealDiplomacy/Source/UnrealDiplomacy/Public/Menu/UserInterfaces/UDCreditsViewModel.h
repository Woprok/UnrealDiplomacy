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
		FString CreditsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
		FString ContentText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
		FString BackText;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetCreditsTitleText(FString newCreditsTitleText);
	FString GetCreditsTitleText() const;
	void SetContentText(FString newContentText);
	FString GetContentText() const;
	void SetBackText(FString newBackText);
	FString GetBackText() const;
};
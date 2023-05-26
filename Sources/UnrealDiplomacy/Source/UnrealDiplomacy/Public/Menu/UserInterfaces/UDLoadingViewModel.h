// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLoadingViewModel.generated.h"

/**
 * ViewModel for displaying loading hint.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLoadingViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LoadingTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText GameplayHintText;
protected:
	virtual void Initialize() override;
private:
	// MVVM Setters & Getters
	void SetLoadingTitleText(FText newLoadingTitleText);
	FText GetLoadingTitleText() const;
	void SetGameplayHintText(FText newGameplayHintText);
	FText GetGameplayHintText() const;
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDNewsViewModel.generated.h"

/**
 * ViewModel for displaying update informations once game is opened.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDNewsViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Close();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NewsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
public:
	virtual void Setup() override;
private:
	// MVVM Setters & Getters
	void SetNewsTitleText(FText newNewsTitleText);
	FText GetNewsTitleText() const;
	void SetMessageText(FText newMessageText);
	FText GetMessageText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
};
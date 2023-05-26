// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDDialogueViewModel.generated.h"

/**
 * ViewModel for single system message such as error.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDialogueViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	void SetErrorDialogueContent(FString title, FString content);
public:
	// Button Functions
	UFUNCTION()
	void Close();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DialogueTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ContentText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
protected:
	virtual void Initialize() override;
private:
	// MVVM Setters & Getters
	void SetDialogueTitleText(FText newDialogueTitleText);
	FText GetDialogueTitleText() const;
	void SetContentText(FText newContentText);
	FText GetContentText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
};
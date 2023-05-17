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
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString TitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString MessageText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CloseText;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetTitleText(FString newTitleText);
	FString GetTitleText() const;
	void SetMessageText(FString newMessageText);
	FString GetMessageText() const;
	void SetCloseText(FString newCloseText);
	FString GetCloseText() const;
};
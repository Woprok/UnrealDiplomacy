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

	virtual void Update() override;

public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Title;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Message;
private:
	// MVVM Setters & Getters

	void SetTitle(FString newTitle);
	FString GetTitle() const;
	void SetMessage(FString newMessage);
	FString GetMessage() const;
};
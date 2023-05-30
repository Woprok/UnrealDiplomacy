// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDClientItemViewModel.generated.h"

/**
 * ViewModel for single joined client used in list view.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDClientItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(UObject* content);
	/**
	 * Get the content as strategy option.
	 */
	const UObject* GetContent() const;
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsHostValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetIsHostValue(bool newIsHostValue);
	bool GetIsHostValue() const;
private:
	// Fields
	UObject* Content;
};
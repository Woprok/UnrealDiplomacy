// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDStrategyOptionViewModel.generated.h"

/**
 * ViewModel for single strategy option used in tile view.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDStrategyOptionViewModel : public UUDViewModel
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
	FText OptionToolTipText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetOptionToolTipText(FText newOptionToolTipText);
	FText GetOptionToolTipText() const;
private:
	// Fields
	UObject* Content;
};
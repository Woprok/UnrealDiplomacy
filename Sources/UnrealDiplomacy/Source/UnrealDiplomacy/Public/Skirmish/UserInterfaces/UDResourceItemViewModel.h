// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDResourceItemViewModel.generated.h"

// Forward Declarations

struct FUDResourceInfo;

/**
 * Single resource in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDResourceItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDResourceInfo content);
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 AmountValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 IdValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetAmountValue(int32 newAmountValue);
	int32 GetAmountValue() const;
	void SetIdValue(int32 newIdValue);
	int32 GetIdValue() const;
private:
	// Fields
	FUDResourceInfo Content;
};
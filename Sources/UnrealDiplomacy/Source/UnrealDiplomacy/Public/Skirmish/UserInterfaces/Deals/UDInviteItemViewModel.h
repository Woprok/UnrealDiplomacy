// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDInviteItemViewModel.generated.h"

// Forward Declarations

struct FUDDealFactionInfo;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDInviteItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealFactionInfo content);
public:
	// Button Functions
	UFUNCTION()
	void Invite();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InviteText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetInviteText(FText newInviteText);
	FText GetInviteText() const;
private:
	// Fields
	FUDDealFactionInfo Content;
};
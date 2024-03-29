// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDParticipantItemViewModel.generated.h"

// Forward Declarations

struct FUDDealFactionInfo;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDParticipantItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the player participant. */
	void SetContent(FUDDealFactionInfo content);
public:
	// Button Functions
	UFUNCTION()
	void Kick();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ReadyStateText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText VoteStateText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText KickText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanKickValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetReadyStateText(FText newReadyStateText);
	FText GetReadyStateText() const;
	void SetVoteStateText(FText newVoteStateText);
	FText GetVoteStateText() const;
	void SetKickText(FText newKickText);
	FText GetKickText() const;
	void SetCanKickValue(bool newCanKickValue);
	bool GetCanKickValue() const;
private:
	// Fields
	FUDDealFactionInfo Content;
};
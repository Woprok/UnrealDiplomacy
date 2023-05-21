// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDJoinGameViewModel.generated.h"

/**
 * ViewModel for joining game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDJoinGameViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText JoinGameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RefreshText;
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetJoinGameTitleText(FText newCreateGameTitleText);
	FText GetJoinGameTitleText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetRefreshText(FText newRefreshText);
	FText GetRefreshText() const;
};
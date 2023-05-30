// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLobbyViewModel.generated.h"

/**
 * Lobby Widget
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Back();
	UFUNCTION()
	void Leave();
	UFUNCTION()
	void Start();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LobbyTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameHeaderText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LeaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StartText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetLobbyTitleText(FText newLobbyTitleText);
	FText GetLobbyTitleText() const;
	void SetNameHeaderText(FText newNameHeaderText);
	FText GetNameHeaderText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetLeaveText(FText newLeaveText);
	FText GetLeaveText() const;
	void SetStartText(FText newStartText);
	FText GetStartText() const;
};
// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDMenuViewModel.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API UUDMenuViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	virtual void Update() override;
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Title;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CreateGame;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString JoinGame;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Settings;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Quit;
private:
	// MVVM Setters & Getters
	void SetTitle(FString newTitle);
	FString GetTitle() const;
	void SetCreateGame(FString newCreateGame);
	FString GetCreateGame() const;
	void SetJoinGame(FString newJoinGame);
	FString GetJoinGame() const;
	void SetSettings(FString newSettings);
	FString GetSettings() const;
	void SetQuit(FString newQuit);
	FString GetQuit() const;
};
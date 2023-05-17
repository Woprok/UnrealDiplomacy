// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDSettingsViewModel.generated.h"

// Forward Declarations

enum class EUDWindowModeType : uint8;

/**
 * Wrapper around items that will be in view.
 */
USTRUCT(BlueprintType)
struct FUDWindowModeItem
{
	GENERATED_BODY()
public:
	FUDWindowModeItem() { }
	FUDWindowModeItem(EUDWindowModeType itemCode, FString itemText)
		: ItemCode(itemCode), ItemText(itemText) { }
	UPROPERTY(BlueprintReadOnly)
	EUDWindowModeType ItemCode;
	UPROPERTY(BlueprintReadOnly)
	FString ItemText;
};

/**
 * Wrapper around items that will be in view.
 */
USTRUCT(BlueprintType)
struct FUDResolutionItem
{
	GENERATED_BODY()
public:
	FUDResolutionItem() { }
	FUDResolutionItem(int32 width, int32 height, FString itemText)
		: Width(width), Height(height), ItemText(itemText) { }
	UPROPERTY(BlueprintReadOnly)
	int32 Width;
	UPROPERTY(BlueprintReadOnly)
	int32 Height;
	UPROPERTY(BlueprintReadOnly)
	FString ItemText;
};

/**
 * ViewModel for updating settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSettingsViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
	// View Options
	/**
	 * Creates new list of all supported window options.
	 */
	TArray<FUDWindowModeItem> CreateWindowModeOptions() const;
	/**
	 * Creates new list of all supported resolution options.
	 */
	TArray<FUDResolutionItem> CreateResolutionOptions() const;
	/**
	 * Applies changes.
	 */
	void SaveChanges();
	/**
	 * Binding for non-primitive.
	 */
	void SetSelectedWindowMode(FUDWindowModeItem selectedWindowMode);
	/**
	 * Binding for non-primitive.
	 */
	void SetSelectedResolution(FUDResolutionItem selectedResolution);
public:
	// MVVM 
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString WindowModeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString ResolutionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CreditsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SaveText;
protected:
	//UPROPERTY()
	//FUDViewModelUpdateBegin OnUpdateBegin;
private:
	// MVVM Setters & Getters
	void SetWindowModeText(FString newWindowModeText);
	FString GetWindowModeText() const;
	void SetResolutionText(FString newResolutionText);
	FString GetResolutionText() const;
	void SetCreditsText(FString newCreditsText);
	FString GetCreditsText() const;
	void SetBackText(FString newBackText);
	FString GetBackText() const;
	void SetSaveText(FString newSaveText);
	FString GetSaveText() const;
};
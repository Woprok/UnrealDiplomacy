// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealEditationTabViewModel.generated.h"

// Forward Declarations

struct FUDViewModelList;
struct FUDDealMinimalInfo;
class UUDPrimaryPointItemViewModel;

/**
 * Tab for showing and editing points that are part of deal.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealEditationTabViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter editor.
	 */
	void InvalidateContent(FUDDealMinimalInfo content);
	/**
	 * Set content of the parameter editor.
	 */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList PointItemList;
public:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Fill with current deal points.
	 */
	void UpdatePointList();
private:
	// MVVM Setters & Getters
	void SetPointItemList(FUDViewModelList newPointItemList);
	FUDViewModelList GetPointItemList() const;
private:
	// Fields
	FUDDealMinimalInfo Content;
	// Collections
	FName PointItemViewModelCollectionName = TEXT("PointItemCollection");
	TSubclassOf<UUDViewModel> PointItemViewModelType;
	TArray<TObjectPtr<UUDPrimaryPointItemViewModel>> PointItemViewModelCollection;
};
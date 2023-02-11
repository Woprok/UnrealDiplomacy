// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDGoldInformationViewModel.generated.h"

/**
 * Gold Info for Resource Bar (reusable?)
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGoldInformationViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Count = -1;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// Retrieves resource info ?
			auto current = ActionModel->GetCurrentResourceGold();
			UpdateDisplayed(current);
		}
		else
		{
			// This should not be visible ?
			// And this is why you dont leave question marks like this in the code, what is this ???
		}
	}
protected:
	/**
	 * Called on update to refresh UI elements.
	 */
	void UpdateDisplayed(int32 newCount)
	{
		SetCount(newCount);
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetCount(int32 newCount)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Count, newCount);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetCount() const
	{
		return Count;
	}
};
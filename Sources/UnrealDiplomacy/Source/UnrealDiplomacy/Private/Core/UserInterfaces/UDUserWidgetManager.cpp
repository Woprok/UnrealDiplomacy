// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDUserWidgetManager.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Kismet/GameplayStatics.h"

TObjectPtr<UUDUserWidget> UUDUserWidgetManager::Get(const FName& name)
{
	return Views[name];
}

void UUDUserWidgetManager::Register(FName name, TSubclassOf<UUDUserWidget> viewType)
{
	if (Views.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) is already registered."), *name.ToString());
		return;
	}

	Views.Add(name, Create(viewType));
	UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) successfully registered."), *name.ToString());
}

bool UUDUserWidgetManager::ShowWidget(const FName& name)
{
	if (!Views.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) is not registered."), *name.ToString());
		return false;
	}
	TObjectPtr<UUDUserWidget> widget = Views[name];
	widget->AddToViewport();
	UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) is now part of viewport."), *name.ToString());
	return true;
}

void UUDUserWidgetManager::HideWidget(const FName& name)
{
	if (!Views.Contains(name))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) is not registered."), *name.ToString());
		return;
	}
	TObjectPtr<UUDUserWidget> widget = Views[name];
	widget->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UUDUserWidgetManager: View(%s) is no longer part of viewport."), *name.ToString());
}

TObjectPtr<UUDUserWidget> UUDUserWidgetManager::Create(TSubclassOf<UUDUserWidget> viewType)
{
	auto player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	return CreateWidget<UUDUserWidget>(player, viewType);
}
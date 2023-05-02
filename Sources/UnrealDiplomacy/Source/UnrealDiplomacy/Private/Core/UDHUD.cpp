// Copyright Miroslav Valach

#include "Core/UDHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Menu/UserInterfaces/UDNewsViewModel.h"

void AUDHUD::ShowWidgets()
{
	for (auto widget : Widgets)
	{
		ShowView(widget);
	}
}

void AUDHUD::CreateWidgets()
{
	for (auto view : Views)
	{
		CreateView(view);
	}
}

void AUDHUD::CreateViewModels()
{
	ViewModels = NewObject<UUDViewModelManager>(this);
	ViewModels->RegisterViewModel(NewObject<UUDNewsViewModel>(this));
}

void AUDHUD::BindViewModel()
{
	for (auto widget : Widgets)
	{
		widget->SetViewModel();
	}
}

void AUDHUD::UpdateViewModel()
{
	ViewModels->UpdateViewModels();
}

UUDViewModel* AUDHUD::GetViewModel(FName name)
{
	return ViewModels->Get(name);
}

void AUDHUD::CreateView(TSubclassOf<UUDUserWidget> widgetClass)
{
	auto pptr = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TObjectPtr<UUDUserWidget> widget = CreateWidget<UUDUserWidget>(pptr, widgetClass);
	Widgets.Add(widget);
}

void AUDHUD::ShowView(TObjectPtr<UUDUserWidget> widget)
{
	widget->AddToViewport();
}
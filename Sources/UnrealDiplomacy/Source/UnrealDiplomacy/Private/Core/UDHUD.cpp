// Copyright Miroslav Valach

#include "Core/UDHUD.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDUserWidgetManager.h"
#include "Core/UserInterfaces/UDViewModelManager.h"

void AUDHUD::ForceUpdate()
{
	ViewModelManager->ForceUpdate();
}

void AUDHUD::SwitchScreen(const FName& screenName)
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Switching to Screen(%s)."), *screenName.ToString());
	HideScreen(CurrentScreenName);

	if (ShowScreen(screenName))
	{
		CurrentScreenName = screenName;
	}
}

void AUDHUD::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Initializing new instance of AUDHUD."));
	check(!IsInitialized);
	IsInitialized = true;

	ViewManager = NewObject<UUDUserWidgetManager>(this);
	ViewModelManager = NewObject<UUDViewModelManager>(this);

	PrepareAllScreens();
}

void AUDHUD::HideWidget(const FName& widgetName)
{
	if (!Screens.Contains(CurrentScreenName))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDHUD: Current Screen(%s) is not defined."), *CurrentScreenName.ToString());
		return;
	}
	FUDScreenInfo screen = Screens[CurrentScreenName];

	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Trying to hide Widget(%s)."), *widgetName.ToString());
	for (const auto& widget : screen.Widgets)
	{
		if (widget.Name == widgetName)
		{
			ViewManager->HideWidget(widget.Name);
		}
	}
}

TArray<TObjectPtr<UUDViewModel>>& AUDHUD::GetViewModelCollection(const FName& name, TSubclassOf<UUDViewModel> viewModelType, int32 desiredTotalCount)
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Obtaining collection for %s of size %d."), *name.ToString(), desiredTotalCount);
	ViewModelManager->RegisterCollection(name, viewModelType, desiredTotalCount);
	return ViewModelManager->GetCollection(name);
}

bool AUDHUD::ShowScreen(const FName& screenName)
{
	if (!Screens.Contains(screenName))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDHUD: Current Screen(%s) is not defined."), *screenName.ToString());
		return false;
	}
	FUDScreenInfo screen = Screens[screenName];

	for (const auto& widget : screen.Widgets)
	{
		ViewManager->ShowWidget(widget.Name);
	}
	return true;
}

void AUDHUD::HideScreen(const FName& screenName)
{
	if (!Screens.Contains(screenName))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDHUD: Current Screen(%s) is not defined."), *screenName.ToString());
		return;
	}
	FUDScreenInfo screen = Screens[screenName];

	for (const auto& widget : screen.Widgets)
	{
		ViewManager->HideWidget(widget.Name);
	}
}

void AUDHUD::PrepareAllScreens() 
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Initializing new instances for all screens."));

	for (const auto& screen : Screens)
	{
		PrepareScreen(screen.Value);
	}

	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Finished initialization for all screens."));
}

void AUDHUD::PrepareScreen(const FUDScreenInfo& screen)
{
	for (const auto& widget : screen.Widgets)
	{ 
		PrepareWidget(widget);
	}
}

void AUDHUD::PrepareWidget(const FUDWidgetInfo& widget)
{
	ViewModelManager->Register(widget.Name, widget.ViewModelType);
	ViewManager->Register(widget.Name, widget.ViewType);
	// Bind view model to view.
	// TODO consider cases where either of the register fails.
	TObjectPtr<UUDUserWidget> view = ViewManager->Get(widget.Name); 
	TObjectPtr<UUDViewModel> viewModel = ViewModelManager->Get(widget.Name);
	view->SetViewModel(viewModel);
}
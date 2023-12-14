// Copyright Miroslav Valach

#include "Core/UDHUD.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDUserWidgetManager.h"
#include "Core/UserInterfaces/UDViewModelManager.h"
#include "Kismet/GameplayStatics.h"

TObjectPtr<AUDHUD> AUDHUD::Get(TObjectPtr<UWorld> world)
{
	TObjectPtr<APlayerController> pc = UGameplayStatics::GetPlayerController(world, 0);
	check(pc != nullptr);
	TObjectPtr<AHUD> hud = pc->GetHUD();
	check(hud != nullptr);
	return CastChecked<AUDHUD>(hud);
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

	OnComponentsInitialized();

	PrepareAllScreens();
}

void AUDHUD::OnComponentsInitialized()
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: On Components Initialize."));
}

TObjectPtr<UUDViewModel> AUDHUD::ShowWidget(const FName& widgetName)
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Trying to show Widget(%s)."), *widgetName.ToString());
	if (ViewManager->ShowWidget(widgetName))
	{
		return ViewModelManager->Get(widgetName);
	};
	return nullptr;
}

void AUDHUD::HideWidget(const FName& widgetName)
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Trying to hide Widget(%s)."), *widgetName.ToString());
	ViewManager->HideWidget(widgetName);
}

TArray<TObjectPtr<UUDViewModel>> AUDHUD::GetViewModelCollection(const FName& name, TSubclassOf<UUDViewModel> viewModelType, int32 desiredTotalCount)
{
	UE_LOG(LogTemp, Log, TEXT("AUDHUD: Obtaining collection for %s of size %d."), *name.ToString(), desiredTotalCount);
	ViewModelManager->RegisterCollection(name, viewModelType, desiredTotalCount);
	TArray<TObjectPtr<UUDViewModel>> array = ViewModelManager->GetCollection(name);
	array.SetNum(desiredTotalCount, true);
	return array;
}

TObjectPtr<UUDViewModel> AUDHUD::GetViewModelCollection(const FName& name, TSubclassOf<UUDViewModel> viewModelType)
{
	TArray<TObjectPtr<UUDViewModel>> ref = GetViewModelCollection(name, viewModelType, 1);
	return ref[0];
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
		// Both should exists, otherwise initialization failed for both.
		ViewManager->ShowWidget(widget.Name);
		ViewModelManager->RefreshViewModel(widget.Name);
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
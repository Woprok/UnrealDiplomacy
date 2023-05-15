// Copyright Miroslav Valach

#include "Core/UserInterfaces/Components/UDButton.h"
#include "Components/ButtonSlot.h"
#include "Components/TextBlock.h"

TSharedRef<SWidget> UUDButton::RebuildWidget()
{
	auto result = Super::RebuildWidget();

	ButtonText = NewObject<UTextBlock>(this);
	ButtonText->SetText(FText::FromString(TEXT("My Button Test")));
	AddChild(ButtonText);

	return result;
}
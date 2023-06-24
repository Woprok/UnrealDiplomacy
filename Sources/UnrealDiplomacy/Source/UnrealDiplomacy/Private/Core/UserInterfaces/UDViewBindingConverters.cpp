// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDViewBindingConverters.h"
#include "Components/SlateWrapperTypes.h"

ESlateVisibility UUDViewBindingConverters::BoolToVisible(bool InBool)
{
	if (InBool)
	{
		return ESlateVisibility::Visible;
	}
	return ESlateVisibility::Collapsed;
}

ESlateVisibility UUDViewBindingConverters::BoolToCollapsed(bool InBool)
{
	if (InBool)
	{
		return ESlateVisibility::Collapsed;
	}
	return ESlateVisibility::Visible;
}

bool UUDViewBindingConverters::VisibleToBool(ESlateVisibility InVisibility)
{
	switch (InVisibility)
	{
	case ESlateVisibility::Visible:
		return true;
		break;
	case ESlateVisibility::Collapsed:
		return false;
		break;
	case ESlateVisibility::Hidden:
	case ESlateVisibility::HitTestInvisible:
	case ESlateVisibility::SelfHitTestInvisible:
	default:
		unimplemented();
		return false;
		break;
	}
}

bool UUDViewBindingConverters::CollapsedToBool(ESlateVisibility InVisibility)
{
	switch (InVisibility)
	{
	case ESlateVisibility::Visible:
		return false;
		break;
	case ESlateVisibility::Collapsed:
		return true;
		break;
	case ESlateVisibility::Hidden:
	case ESlateVisibility::HitTestInvisible:
	case ESlateVisibility::SelfHitTestInvisible:
	default:
		unimplemented();
		return false;
		break;
	}
}

bool UUDViewBindingConverters::ValueToBool(int32 InValue, int32 PrimaryValue)
{
	return InValue == PrimaryValue;
}
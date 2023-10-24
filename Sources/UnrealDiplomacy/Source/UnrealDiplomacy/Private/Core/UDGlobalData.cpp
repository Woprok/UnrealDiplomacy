// Copyright Miroslav Valach

#include "Core/UDGlobalData.h"

EUDWindowModeType UUDApplicationConverters::ToWindowModeType(EWindowMode::Type windowMode)
{
	switch (windowMode)
	{
	case EWindowMode::Fullscreen:
		return EUDWindowModeType::Fullscreen;
	case EWindowMode::WindowedFullscreen:
		return EUDWindowModeType::Borderless;
	case EWindowMode::Windowed:
		return EUDWindowModeType::Windowed;
	case EWindowMode::NumWindowModes:
	default:
		unimplemented();
		return EUDWindowModeType::Fullscreen;
	}
}

EWindowMode::Type UUDApplicationConverters::FromWindowModeType(EUDWindowModeType windowMode)
{
	switch (windowMode)
	{
	case EUDWindowModeType::Fullscreen:
		return EWindowMode::Fullscreen;
	case EUDWindowModeType::Borderless:
		return EWindowMode::WindowedFullscreen;
	case EUDWindowModeType::Windowed:
		return EWindowMode::Windowed;
	default:
		unimplemented();
		return EWindowMode::Fullscreen;
	}
}

bool UUDApplicationConverters::FromCheckBoxState(ECheckBoxState checkedState)
{
	switch (checkedState)
	{
	case ECheckBoxState::Unchecked:
		return false;
	case ECheckBoxState::Checked:
		return true;
	case ECheckBoxState::Undetermined:
	default:
		return false;
	}
}

ECheckBoxState UUDApplicationConverters::ToCheckBoxState(bool isChecked)
{
	if (isChecked)
	{
		return ECheckBoxState::Checked;
	}
	else
	{
		return ECheckBoxState::Unchecked;
	}
}

FUDApplicationSettings FUDApplicationSettings::GetDefault()
{
	return FUDApplicationSettings(FIntPoint(1280, 720), EUDWindowModeType::Windowed);
}
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
		unimplemented()
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
		unimplemented()
		return EWindowMode::Fullscreen;
	}
}
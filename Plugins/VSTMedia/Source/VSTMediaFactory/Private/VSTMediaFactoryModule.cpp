// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "IMediaPlayerFactory.h"
#include "IMediaModule.h"
#include "IMediaOptions.h"
#include "Internationalization/Internationalization.h"
#include "Misc/Paths.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UObject/Class.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/WeakObjectPtr.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "VSTMediaSettings.h"
#endif

#include "../../VSTMedia/Public/IVSTMediaModule.h"

#define LOCTEXT_NAMESPACE "FVSTMediaFactoryModule"

/**
 * Implements the VSTMediaFactory module.
 */
class FVSTMediaFactoryModule
	: public IMediaPlayerFactory
	, public IModuleInterface
{
public:

	//~ IMediaPlayerFactory interface

	virtual bool CanPlayUrl(const FString& Url, const IMediaOptions* Options, TArray<FText>* OutWarnings, TArray<FText>* OutErrors) const override
	{
		FString Scheme;
		FString Location;

		// check scheme
		if (Url.Split(TEXT("rtsp://"), &Scheme, &Location, ESearchCase::CaseSensitive)
			|| Url.Split(TEXT("rtmp://"), &Scheme, &Location, ESearchCase::CaseSensitive)
			|| Url.Split(TEXT("https://"), &Scheme, &Location, ESearchCase::CaseSensitive)
			|| Url.Split(TEXT("http://"), &Scheme, &Location, ESearchCase::CaseSensitive)
			)
		{
			return true;
		}

		return false;
	}

	virtual FGuid GetPlayerPluginGUID() const override
	{
		return FGuid();
	}

	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) override
	{
		auto VSTMediaModule = FModuleManager::LoadModulePtr<IVSTMediaModule>("VSTMedia");
		return (VSTMediaModule != nullptr) ? VSTMediaModule->CreatePlayer(EventSink) : nullptr;
	}

	virtual FText GetDisplayName() const override
	{
		return LOCTEXT("MediaPlayerDisplayName", "VST Player");
	}

	virtual FName GetPlayerName() const override
	{
		static FName PlayerName(TEXT("VSTMedia"));
		return PlayerName;
	}

	virtual const TArray<FString>& GetSupportedPlatforms() const override
	{
		return SupportedPlatforms;
	}

	virtual bool SupportsFeature(EMediaFeature Feature) const override
	{
		return (Feature == EMediaFeature::VideoSamples);
	}

public:

	//~ IModuleInterface interface

	virtual void StartupModule() override
	{
		// supported platforms

		SupportedPlatforms.Add(TEXT("Windows"));

		// supported schemes
		SupportedUriSchemes.Add(TEXT("http"));
		SupportedUriSchemes.Add(TEXT("https"));
		SupportedUriSchemes.Add(TEXT("realrtsp"));
		SupportedUriSchemes.Add(TEXT("rtmp"));
		SupportedUriSchemes.Add(TEXT("rtp"));
		SupportedUriSchemes.Add(TEXT("rtsp"));

#if WITH_EDITOR
		// register settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Game", "VSTMedia",
				LOCTEXT("VSTMediaSettingsName", "VST Media"),
				LOCTEXT("VSTMediaSettingsDescription", "Configure the VST Media plug-in."),
				GetMutableDefault<UVSTMediaSettings>()
			);
		}
#endif //WITH_EDITOR

		// register player factory
		auto MediaModule = FModuleManager::LoadModulePtr<IMediaModule>("Media");

		if (MediaModule != nullptr)
		{
			MediaModule->RegisterPlayerFactory(*this);
		}
	}

	virtual void ShutdownModule() override
	{
		// unregister player factory
		auto MediaModule = FModuleManager::GetModulePtr<IMediaModule>("Media");

		if (MediaModule != nullptr)
		{
			MediaModule->UnregisterPlayerFactory(*this);
		}

#if WITH_EDITOR
		// unregister settings
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Project", "Game", "VSTMedia");
		}
#endif //WITH_EDITOR
	}

private:

	/** List of supported media file types. */
	TArray<FString> SupportedFileExtensions;

	/** List of platforms that the media player support. */
	TArray<FString> SupportedPlatforms;

	/** List of supported URI schemes. */
	TArray<FString> SupportedUriSchemes;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVSTMediaFactoryModule, VSTMediaFactory);
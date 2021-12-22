#include "VSTMediaBPFunLibrary.h"
#include "MediaSource.h"
#include "FileMediaSource.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VSTStreamMediaSource.h"
/*#if WITH_EDITOR
//#include "LicenseManager.h"
#endif*/
bool UVSTMediaBPFunLibrary::LoadStreamMediaSource(
	UObject* WorldContext, 
	UMediaSoundComponent* MediaSoundComponent, 
	const FString& Url, 
	UMediaPlayer*& Player,
	UMediaTexture*& Texture, 
	UMediaSource*& MediaSource,
	UMaterialInstanceDynamic*& DynamicMat, 
	FLinearColor ClearColor,
	bool bIsAutoColor/* = true*/,
	const bool bIsLocal/*=true*/, 
	const bool PlayOnOpen/*=true*/,
	const bool Loop/*=true*/)
{
	Player = NULL;
	Texture = NULL;

	UMediaPlayer* LoadPlayer = NewObject<UMediaPlayer>();
	Player = LoadPlayer;
	Player->SetLooping(Loop);
	Player->PlayOnOpen = PlayOnOpen;

	Texture = NewObject<UMediaTexture>();
	Texture->SetMediaPlayer(Player);
	Texture->AutoClear = bIsAutoColor;
	Texture->ClearColor = ClearColor;
	Texture->UpdateResource();

	//MediaSoundComponent = NewObject<UMediaSoundComponent>();

	if (MediaSoundComponent != nullptr)
	{
		MediaSoundComponent->SetMediaPlayer(Player);
	}

	if (bIsLocal)
	{
		MediaSource = NewObject<UFileMediaSource>();
		Cast<UFileMediaSource>(MediaSource)->SetFilePath(Url);
	}
	else
	{
		MediaSource = NewObject<UVSTStreamMediaSource>();
		if (MediaSource)
		{
#if WITH_EDITOR
			//FLicenseManagerModule::Get().ObjectLoaded.Broadcast(MediaSource);
#endif
			Cast<UVSTStreamMediaSource>(MediaSource)->StreamUrl = Url;
		}
	}

	FSoftObjectPath MI_UIMediaMatIns(TEXT("MaterialInstanceConstant'/VSTMedia/Materials/MI_UIVideoMat_Inst.MI_UIVideoMat_Inst'"));

	if (MI_UIMediaMatIns.IsAsset())
	{
		UObject*MatObject = MI_UIMediaMatIns.TryLoad();
		UMaterialInterface* MatInterface = Cast<UMaterialInterface>(MatObject);
		if (MatInterface)
		{
			DynamicMat = UMaterialInstanceDynamic::Create(MatInterface, WorldContext, "None");
		}
	}
	if (MediaSource&&LoadPlayer->OpenSource(MediaSource))
	{
		if (DynamicMat)DynamicMat->SetTextureParameterValue("MediaTex", Texture);
		return true;
	}
	return false;
}
#include "VSTVideo.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FVST_VideoModule"

TSharedPtr<FSlateStyleSet> FVideoUIStyle::VideoUIStyle = nullptr;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( VideoUIStyle->RootToContentDir( RelativePath, TEXT(".png") ), FVector2D(16.0f, 16.0f),FLinearColor(1, 1, 1, 1.0f),__VA_ARGS__ )

void FVST_VideoModule::StartupModule()
{
	FVideoUIStyle::Initialize();
}

void FVST_VideoModule::ShutdownModule()
{
	FVideoUIStyle::Shutdown();
}

void FVideoUIStyle::Initialize()
{
	if (!VideoUIStyle.IsValid())
	{
		VideoUIStyle = MakeShareable(new FSlateStyleSet("VideoUIStyle"));
		VideoUIStyle->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("VSTMedia/Resources"));
		VideoUIStyle->Set("ClassIcon.VideoPlayer", new IMAGE_BRUSH("Icon128"));
		FSlateStyleRegistry::RegisterSlateStyle(*VideoUIStyle);
	}
}
#undef IMAGE_BRUSH
void FVideoUIStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*VideoUIStyle);
	ensure(VideoUIStyle.IsUnique());
	VideoUIStyle.Reset();
}

void FVideoUIStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FVideoUIStyle::Get()
{
	return *VideoUIStyle;
}

FName FVideoUIStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("VideoUIStyle"));
	return StyleSetName;
}

IMPLEMENT_MODULE(FVST_VideoModule, VST_Video);
#undef LOCTEXT_NAMESPACE

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "VSTMediaBPFunLibrary.generated.h"

/**
 *
 */
UCLASS()
class VST_VIDEO_API UVSTMediaBPFunLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category = "VST Media Fun Library")
	static bool LoadStreamMediaSource(
			UObject* WorldContext,
			UMediaSoundComponent* MediaSoundComponent,
			const FString& Url,
			UMediaPlayer*& Player,
			UMediaTexture*& Texture,
			UMediaSource*& MediaSource,
			class UMaterialInstanceDynamic*& DynamicMat,
			FLinearColor ClearColor,
			bool bIsAutoColor=true,
			const bool bIsLocal = true,
			const bool PlayOnOpen = true,
			const bool Loop = true);
};

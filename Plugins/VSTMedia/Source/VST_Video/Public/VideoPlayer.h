// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaTexture.h"
#include "UObject/ObjectMacros.h"
#include "Misc/Attribute.h"
#include "Styling/SlateBrush.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "Components/Widget.h"
#include "UObject/ScriptInterface.h"
#include "Slate/SlateTextureAtlasInterface.h"
#include "Engine/StreamableManager.h"
#include "Slate/SlateBrushAsset.h"
#include "VideoPlayer.generated.h"

/**
 * 
 */
UCLASS()
class VST_VIDEO_API UVideoPlayer : public UWidget
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="VideoPlayer")
	FString Url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VideoPlayer")
	FLinearColor ClearColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VideoPlayer")
	bool bIsAutoColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VideoPlayer")
	bool bIsLocal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VideoPlayer")
	bool PlayOnOpen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VideoPlayer")
	bool Loop;
public:
	UFUNCTION(BlueprintCallable, Category = "VideoPlayer")
	void Play();


public:

#if WITH_EDITORONLY_DATA
	/** Image to draw */
	UPROPERTY()
		USlateBrushAsset* Image_DEPRECATED;
#endif

	/** Image to draw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
		FSlateBrush Brush;

	/** A bindable delegate for the Image. */
	UPROPERTY()
		FGetSlateBrush BrushDelegate;

	/** Color and opacity */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance, meta = (sRGB = "true"))
		FLinearColor ColorAndOpacity;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
		FGetLinearColor ColorAndOpacityDelegate;

	/** Flips the image if the localization's flow direction is RightToLeft */
	UPROPERTY(EditAnywhere, Category = "Localization")
		bool bFlipForRightToLeftFlowDirection;

public:

	UPROPERTY(EditAnywhere, Category = Events, meta = (IsBindableEvent = "True"))
		FOnPointerEvent OnMouseButtonDownEvent;

public:

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetBrushSize(FVector2D DesiredSize);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetBrushTintColor(FSlateColor TintColor);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		void SetBrushResourceObject(UObject* ResourceObject);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrush(const FSlateBrush& InBrush);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromAsset(USlateBrushAsset* Asset);

	/**
	* Sets the Brush to the specified Texture.
	*
	*   @param Texture Texture to use to set on Brush.
	*	@param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);

	/**
	* Sets the Brush to the specified Atlas Region.
	*
	*   @param AtlasRegion Region of the Atlas to use to set on Brush.
	*	@param bMatchSize If true, image will change its size to atlas region size. If false, atlas region will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize = false);

	/**
	* Sets the Brush to the specified Dynamic Texture.
	*
	*   @param Texture Dynamic Texture to use to set on Brush.
	*	@param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromMaterial(UMaterialInterface* Material);

	/**
	* Sets the Brush to the specified Soft Texture.
	*
	*   @param SoftTexture Soft Texture to use to set on Brush.
	*	@param bMatchSize If true, image will change its size to texture size. If false, texture will be stretched to image size.
	*/
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize = false);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		virtual void SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
		UMaterialInstanceDynamic* GetDynamicMaterial();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITORONLY_DATA
	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface
#endif

#if WITH_EDITOR
	//~ Begin UWidget Interface
	virtual const FText GetPaletteCategory() override;
	//~ End UWidget Interface
#endif

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	/** Translates the bound brush data and assigns it to the cached brush used by this widget. */
	const FSlateBrush* ConvertImage(TAttribute<FSlateBrush> InImageAsset) const;

	// Called when we need to stream in content.
	void RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, TFunction<void()>&& Callback);
	virtual void RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, FStreamableDelegate DelegateToCall);

	// Called when we need to abort the texture being streamed in.
	virtual void CancelImageStreaming();

	// Called when the image streaming starts, after the other one was cancelled.
	virtual void OnImageStreamingStarted(TSoftObjectPtr<UObject> SoftObject);

	// Called when the image streaming completes.
	virtual void OnImageStreamingComplete(TSoftObjectPtr<UObject> LoadedSoftObject);

	//
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);

#if WITH_ACCESSIBILITY
	virtual TSharedPtr<SWidget> GetAccessibleWidget() const override;
#endif

protected:
	TSharedPtr<SImage> MyImage;

	TSharedPtr<FStreamableHandle> StreamingHandle;
	FSoftObjectPath StreamingObjectPath;

protected:

	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
private:
	UMediaSoundComponent* MediaSoundComponent;

	UMediaPlayer* Player;
	UMediaTexture* MediaTexture;
	class UMediaSource* MediaSource;
	UMaterialInstanceDynamic* DynamicMat;
};

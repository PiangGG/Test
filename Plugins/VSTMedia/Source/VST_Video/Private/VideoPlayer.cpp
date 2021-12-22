// Fill out your copyright notice in the Description page of Project Settings.


#include "VideoPlayer.h"
#include "VSTMediaBPFunLibrary.h"
#include "MediaSource.h"
#include "FileMediaSource.h"
#include "StreamMediaSource.h"
#include "Slate/SlateBrushAsset.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
/*
#if WITH_EDITOR
	//#include "LicenseManager.h"
#endif
*/
#define LOCTEXT_NAMESPACE "VideoPlayer"

void UVideoPlayer::Play()
{
	UVSTMediaBPFunLibrary::LoadStreamMediaSource(
		this,
		MediaSoundComponent,
		Url,
		Player,
		MediaTexture,
		MediaSource,
		DynamicMat,
		ClearColor,
		bIsAutoColor,
		bIsLocal,
		PlayOnOpen,
		Loop);
	if (MyImage)
	{
		SetBrushFromMaterial(DynamicMat);
	}
}

UVideoPlayer::UVideoPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ColorAndOpacity(FLinearColor::White)
	, ClearColor(FLinearColor::Black)
	,bIsLocal(1)
	, PlayOnOpen(1)
	,Loop(1)
{
}

#if WITH_EDITORONLY_DATA
void UVideoPlayer::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS && Image_DEPRECATED != nullptr)
	{
		Brush = Image_DEPRECATED->Brush;
		Image_DEPRECATED = nullptr;
	}
}
#endif

void UVideoPlayer::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyImage.Reset();
}

TSharedRef<SWidget> UVideoPlayer::RebuildWidget()
{
#if WITH_EDITOR
	//FLicenseManagerModule::Get().ObjectLoaded.Broadcast(this);
#endif
	MyImage = SNew(SImage)
		.FlipForRightToLeftFlowDirection(bFlipForRightToLeftFlowDirection);

	return MyImage.ToSharedRef();
}

void UVideoPlayer::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	TAttribute<const FSlateBrush*> ImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Brush, const FSlateBrush*, ConvertImage);

	if (MyImage.IsValid())
	{
		MyImage->SetImage(ImageBinding);
		MyImage->SetColorAndOpacity(ColorAndOpacityBinding);
		MyImage->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
	}
}

void UVideoPlayer::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyImage.IsValid())
	{
		MyImage->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UVideoPlayer::SetOpacity(float InOpacity)
{
	ColorAndOpacity.A = InOpacity;
	if (MyImage.IsValid())
	{
		MyImage->SetColorAndOpacity(ColorAndOpacity);
	}
}

const FSlateBrush* UVideoPlayer::ConvertImage(TAttribute<FSlateBrush> InImageAsset) const
{
	UVideoPlayer* MutableThis = const_cast<UVideoPlayer*>(this);
	MutableThis->Brush = InImageAsset.Get();

	return &Brush;
}

void UVideoPlayer::SetBrush(const FSlateBrush& InBrush)
{
	if (Brush != InBrush)
	{
		Brush = InBrush;

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushSize(FVector2D DesiredSize)
{
	if (Brush.ImageSize != DesiredSize)
	{
		Brush.ImageSize = DesiredSize;

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushTintColor(FSlateColor TintColor)
{
	if (Brush.TintColor != TintColor)
	{
		Brush.TintColor = TintColor;

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushResourceObject(UObject* ResourceObject)
{
	if (Brush.GetResourceObject() != ResourceObject)
	{
		Brush.SetResourceObject(ResourceObject);

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushFromAsset(USlateBrushAsset* Asset)
{
	if (!Asset || Brush != Asset->Brush)
	{
		CancelImageStreaming();
		Brush = Asset ? Asset->Brush : FSlateBrush();

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize)
{
	CancelImageStreaming();

	if (Brush.GetResourceObject() != Texture)
	{
		Brush.SetResourceObject(Texture);

		if (Texture) // Since this texture is used as UI, don't allow it affected by budget.
		{
			Texture->bForceMiplevelsToBeResident = true;
			Texture->bIgnoreStreamingMipBias = true;
		}

		if (bMatchSize)
		{
			if (Texture)
			{
				Brush.ImageSize.X = Texture->GetSizeX();
				Brush.ImageSize.Y = Texture->GetSizeY();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushFromAtlasInterface(TScriptInterface<ISlateTextureAtlasInterface> AtlasRegion, bool bMatchSize)
{
	if (Brush.GetResourceObject() != AtlasRegion.GetObject())
	{
		CancelImageStreaming();
		Brush.SetResourceObject(AtlasRegion.GetObject());

		if (bMatchSize)
		{
			if (AtlasRegion)
			{
				FSlateAtlasData AtlasData = AtlasRegion->GetSlateAtlasData();
				Brush.ImageSize = AtlasData.GetSourceDimensions();
			}
			else
			{
				Brush.ImageSize = FVector2D(0, 0);
			}
		}

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushFromTextureDynamic(UTexture2DDynamic* Texture, bool bMatchSize)
{
	if (Brush.GetResourceObject() != Texture)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Texture);

		if (bMatchSize && Texture)
		{
			Brush.ImageSize.X = Texture->SizeX;
			Brush.ImageSize.Y = Texture->SizeY;
		}

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::SetBrushFromMaterial(UMaterialInterface* Material)
{
	if (Brush.GetResourceObject() != Material)
	{
		CancelImageStreaming();
		Brush.SetResourceObject(Material);

		//TODO UMG Check if the material can be used with the UI

		if (MyImage.IsValid())
		{
			MyImage->SetImage(&Brush);
		}
	}
}

void UVideoPlayer::CancelImageStreaming()
{
	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}

	StreamingObjectPath.Reset();
}

void UVideoPlayer::RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, TFunction<void()>&& Callback)
{
	RequestAsyncLoad(SoftObject, FStreamableDelegate::CreateLambda(MoveTemp(Callback)));
}

void UVideoPlayer::RequestAsyncLoad(TSoftObjectPtr<UObject> SoftObject, FStreamableDelegate DelegateToCall)
{
	CancelImageStreaming();

	if (UObject* StrongObject = SoftObject.Get())
	{
		DelegateToCall.ExecuteIfBound();
		return;  // No streaming was needed, complete immediately.
	}

	OnImageStreamingStarted(SoftObject);

	TWeakObjectPtr<UVideoPlayer> WeakThis(this);
	StreamingObjectPath = SoftObject.ToSoftObjectPath();
	StreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StreamingObjectPath,
		[WeakThis, DelegateToCall, SoftObject]() {
		if (UVideoPlayer* StrongThis = WeakThis.Get())
		{
			// If the object paths don't match, then this delegate was interrupted, but had already been queued for a callback
			// so ignore everything and abort.
			if (StrongThis->StreamingObjectPath != SoftObject.ToSoftObjectPath())
			{
				return; // Abort!
			}

			// Call the delegate to do whatever is needed, probably set the new image.
			DelegateToCall.ExecuteIfBound();

			// Note that the streaming has completed.
			StrongThis->OnImageStreamingComplete(SoftObject);
		}
	},
		FStreamableManager::AsyncLoadHighPriority);
}

void UVideoPlayer::OnImageStreamingStarted(TSoftObjectPtr<UObject> SoftObject)
{
	// No-Op
}

void UVideoPlayer::OnImageStreamingComplete(TSoftObjectPtr<UObject> LoadedSoftObject)
{
	// No-Op
}

void UVideoPlayer::SetBrushFromSoftTexture(TSoftObjectPtr<UTexture2D> SoftTexture, bool bMatchSize)
{
	TWeakObjectPtr<UVideoPlayer> WeakThis(this); // using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftTexture,
		[WeakThis, SoftTexture, bMatchSize]() {
		if (UVideoPlayer* StrongThis = WeakThis.Get())
		{
			ensureMsgf(SoftTexture.Get(), TEXT("Failed to load %s"), *SoftTexture.ToSoftObjectPath().ToString());
			StrongThis->SetBrushFromTexture(SoftTexture.Get(), bMatchSize);
		}
	}
	);
}

void UVideoPlayer::SetBrushFromSoftMaterial(TSoftObjectPtr<UMaterialInterface> SoftMaterial)
{
	TWeakObjectPtr<UVideoPlayer> WeakThis(this); // using weak ptr in case 'this' has gone out of scope by the time this lambda is called

	RequestAsyncLoad(SoftMaterial,
		[WeakThis, SoftMaterial]() {
		if (UVideoPlayer* StrongThis = WeakThis.Get())
		{
			ensureMsgf(SoftMaterial.Get(), TEXT("Failed to load %s"), *SoftMaterial.ToSoftObjectPath().ToString());
			StrongThis->SetBrushFromMaterial(SoftMaterial.Get());
		}
	}
	);
}

UMaterialInstanceDynamic* UVideoPlayer::GetDynamicMaterial()
{
	UMaterialInterface* Material = NULL;

	UObject* Resource = Brush.GetResourceObject();
	Material = Cast<UMaterialInterface>(Resource);

	if (Material)
	{
		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Brush.SetResourceObject(DynamicMaterial);

			if (MyImage.IsValid())
			{
				MyImage->SetImage(&Brush);
			}
		}

		return DynamicMaterial;
	}

	//TODO UMG can we do something for textures?  General purpose dynamic material for them?

	return NULL;
}

FReply UVideoPlayer::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (OnMouseButtonDownEvent.IsBound())
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UVideoPlayer::GetAccessibleWidget() const
{
	return MyImage;
}
#endif

#if WITH_EDITOR

const FText UVideoPlayer::GetPaletteCategory()
{
	return LOCTEXT("DTUI", "DTUI");
}

#endif


/////////////////////////////////////////////////////
#undef LOCTEXT_NAMESPACE
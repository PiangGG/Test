// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTFileMediaSourceFactory.h"

#include "FileMediaSource.h"
#include "Misc/Paths.h"


/* UVSTFileMediaSourceFactory structors
 *****************************************************************************/

UVSTFileMediaSourceFactory::UVSTFileMediaSourceFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	SupportedClass = UFileMediaSource::StaticClass();
	bEditorImport = true;
}


/* UFactory overrides
 *****************************************************************************/

bool UVSTFileMediaSourceFactory::FactoryCanImport(const FString& Filename)
{
	// @hack: disable file extensions that are used in other factories
	// @todo gmp: add support for multiple factories per file extension
	

	return false;
}


UObject* UVSTFileMediaSourceFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	UFileMediaSource* MediaSource = NewObject<UFileMediaSource>(InParent, InClass, InName, Flags);
	MediaSource->SetFilePath(CurrentFilename);

	return MediaSource;
}

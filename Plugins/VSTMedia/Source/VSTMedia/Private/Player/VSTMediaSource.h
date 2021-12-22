// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


/**
 * Implements a media source, such as a movie file or URL.
 */
class FVSTMediaSource
{
public:

	/**
	 * Create and initialize a new instance.
	 *
	 * @param InInstance The LibVLC instance to use.
	 */
	FVSTMediaSource();

public:

	/** Get the media object. */
	void* GetMedia() const
	{
		return VSTMediaPlayer;
	}

	/** Get the URL of the currently open media source. */
	const FString& GetCurrentUrl() const
	{
		return CurrentUrl;
	}

	/**
	 * Get the duration of the media source.
	 *
	 * @return Media duration.
	 */
	FTimespan GetDuration() const;



	/**
	 * Open a media source from the specified URL.
	 *
	 * You must call Close() if this media source is open prior to calling this method.
	 *
	 * @param Url The media resource locator.
	 * @return The media object.
	 * @see OpenArchive, Close
	 */
	bool OpenUrl(const FString& Url);

	/**
	 * Close the media source.
	 *
	 * @see OpenArchive, OpenUrl
	 */
	void Close();


private:

	/** The file or memory archive to stream from (for local media only). */
	TSharedPtr<FArchive, ESPMode::ThreadSafe> Data;

	/** Currently opened media. */
	FString CurrentUrl;

	void* VSTMediaPlayer;
};

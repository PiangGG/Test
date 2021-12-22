// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMediaView.h"


/**
 * Implements VLC video view settings.
 */
class FVSTMediaView
	: public IMediaView
{
public:

	/** Default constructor. */
	FVSTMediaView();

	/** Virtual destructor. */
	virtual ~FVSTMediaView() {}

public:

	/**
	 * Initialize this object for the specified VLC media player.
	 *
	 * @param InPlayer The VLC media player.
	 */
	void Initialize();

	/** Shut down this object. */
	void Shutdown();

public:

	//~ IMediaView interface

	virtual bool GetViewField(float& OutHorizontal, float& OutVertical) const override;
	virtual bool GetViewOrientation(FQuat& OutOrientation) const override;
	virtual bool SetViewField(float Horizontal, float Vertical, bool Absolute);
	virtual bool SetViewOrientation(const FQuat& Orientation, bool Absolute);

private:

	/** The current field of view (horizontal & vertical). */
	float CurrentFieldOfView;

	/** The current view orientation. */
	FQuat CurrentOrientation;

};

// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VSTMediaView.h"
#include "VSTMediaPrivate.h"

#include "VST.h"


/* FVSTMediaView structors
*****************************************************************************/

FVSTMediaView::FVSTMediaView()
	: CurrentFieldOfView(90.0f)
	, CurrentOrientation(EForceInit::ForceInit)
{
	
}




/* FVSTMediaView interface
*****************************************************************************/

void FVSTMediaView::Initialize()
{
	
}


void FVSTMediaView::Shutdown()
{
	
}


/* IMediaView interface
*****************************************************************************/

bool FVSTMediaView::GetViewField(float& OutHorizontal, float& OutVertical) const
{
	OutHorizontal = CurrentFieldOfView;
	OutVertical = CurrentFieldOfView;

	return true;
}


bool FVSTMediaView::GetViewOrientation(FQuat& OutOrientation) const
{
	OutOrientation = CurrentOrientation;

	return true;
}


bool FVSTMediaView::SetViewField(float Horizontal, float Vertical, bool Absolute)
{
	
	return true;
}


bool FVSTMediaView::SetViewOrientation(const FQuat& Orientation, bool Absolute)
{


	return true;
}

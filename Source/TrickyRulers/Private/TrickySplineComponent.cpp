// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickySplineComponent.h"


UTrickySplineComponent::UTrickySplineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsEditorOnly = true;
	Mobility = EComponentMobility::Static;
}

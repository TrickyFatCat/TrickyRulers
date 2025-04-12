// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "TrickySplineComponent.generated.h"


UCLASS(HideCategories = (Collision, Activation, Navigation, Cooking, LOD, TextureStreaming, HLOD, Tags, RayTracing,
	AssetUserData, ComponentTick, ComponentReplication, Events, Replication, Sockets, Variable, Transform))
class TRICKYRULERS_API UTrickySplineComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	UTrickySplineComponent();
};

﻿// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "TrickyRulerProperties.generated.h"

UENUM()
enum class ERulerType : uint8
{
	Line,
	Circle,
	Sphere,
	Cylinder,
	Capsule,
	Box,
};

USTRUCT(BlueprintType)
struct FLineRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Length = 100;

	UPROPERTY(EditAnywhere,
		Category="LineRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	float GetLengthInMeters() const
	{
		return static_cast<float>(Length) / 100.f;
	}
};

USTRUCT(BlueprintType)
struct FCircleRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="CircleRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="CircleRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Radius = 100;

	UPROPERTY(EditAnywhere,
		Category="CircleRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="CircleRuler", meta=(ClampMin=8, UIMin=8, ClampMax=256, UIMax=256))
	int32 Segments = 32;

	float GetRadiusInMeters() const
	{
		return static_cast<float>(Radius) / 100.f;
	}
};

USTRUCT(BlueprintType)
struct FSphereRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="SphereRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="SphereRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Radius = 100;

	UPROPERTY(EditAnywhere,
		Category="SphereRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="SphereRuler", meta=(ClampMin=8, UIMin=8, ClampMax=256, UIMax=256))
	int32 Segments = 32;

	float GetRadiusInMeters() const
	{
		return static_cast<float>(Radius) / 100.f;
	}
};

USTRUCT(BlueprintType)
struct FCylinderRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="CylinderRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="CylinderRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Radius = 100;

	UPROPERTY(EditAnywhere, Category="CylinderRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Height = 100;

	UPROPERTY(EditAnywhere,
		Category="CylinderRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="CylinderRuler", meta=(ClampMin=8, UIMin=8, ClampMax=256, UIMax=256))
	int32 Segments = 32;

	FVector2D GetDimensionsInMeters() const
	{
		const float RadiusMeters = static_cast<float>(Radius) / 100.f;
		const float HeightMeters = static_cast<float>(Height) / 100.f;
		return FVector2D(RadiusMeters, HeightMeters);
	}
};

USTRUCT(BlueprintType)
struct FBoxRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="BoxRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 LengthX = 100;

	UPROPERTY(EditAnywhere, Category="BoxRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 LengthY = 100;

	UPROPERTY(EditAnywhere, Category="BoxRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 LengthZ = 100;

	UPROPERTY(EditAnywhere,
		Category="LineRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="LineRuler", meta=(HideAlphaChannel))
	bool bIsFilled = false;

	UPROPERTY(EditAnywhere,
		Category="LineRuler",
		meta=(EditCondition="bIsFilled == true", ClampMin=0.01, UIMin=0.01, ClampMax=1.0, UIMax=1.0, Delta=0.05))
	float FillAlpha = 0.25f;

	FVector GetLengthInMeters() const
	{
		const float LengthXMeters = static_cast<float>(LengthX) / 100.f;
		const float LengthYMeters = static_cast<float>(LengthY) / 100.f;
		const float LengthZMeters = static_cast<float>(LengthZ) / 100.f;
		return FVector(LengthXMeters, LengthYMeters, LengthZMeters);
	}

	FColor GetFillColor() const
	{
		FColor FillColor = Color;
		FillColor.A = static_cast<uint8>(FillAlpha * 255.f);
		return FillColor;
	}
};

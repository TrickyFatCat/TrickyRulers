// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

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

	UPROPERTY(VisibleAnywhere, Category="LineRuler")
	FString Meters = TEXT("1 m");

	UPROPERTY(EditAnywhere,
		Category="LineRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	void UpdateLength()
	{
		const float LengthMeters = static_cast<float>(Length) / 100.f;
		Meters = FString::Printf(TEXT("%.2f m"), LengthMeters);
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

	UPROPERTY(VisibleAnywhere, Category="CircleRuler")
	FString Meters = TEXT("1 m");

	UPROPERTY(EditAnywhere,
		Category="CircleRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="CircleRuler", meta=(ClampMin=8, UIMin=8, ClampMax=256, UIMax=256))
	int32 Segments = 32;

	void UpdateRadius()
	{
		const float RadiusMeters = static_cast<float>(Radius) / 100.f;
		Meters = FString::Printf(TEXT("%.2f m"), RadiusMeters);
	}
};

USTRUCT(BlueprintType)
struct FSphereRulerProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="ShpereRuler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;
	
	UPROPERTY(EditAnywhere, Category="SphereRuler", meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Radius = 100;

	UPROPERTY(VisibleAnywhere, Category="SphereRuler")
	FString Meters = TEXT("1 m");

	UPROPERTY(EditAnywhere,
		Category="SphereRuler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, Category="SphereRuler", meta=(ClampMin=8, UIMin=8, ClampMax=256, UIMax=256))
	int32 Segments = 32;

	void UpdateRadius()
	{
		const float RadiusMeters = static_cast<float>(Radius) / 100.f;
		Meters = FString::Printf(TEXT("%.2f m"), RadiusMeters);
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

	UPROPERTY(VisibleAnywhere, Category="LineRuler")
	FString Meters = TEXT("X: 1 m\nY: 1 m\nZ: 1m");

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

	void UpdateLength()
	{
		const float LengthXMeters = static_cast<float>(LengthX) / 100.f;
		const float LengthYMeters = static_cast<float>(LengthY) / 100.f;
		const float LengthZMeters = static_cast<float>(LengthZ) / 100.f;
		Meters = FString::Printf(TEXT("X: %.2f m\nY: %.2f m\nZ: %.2f m"), LengthXMeters, LengthYMeters, LengthZMeters);
	}

	FColor GetFillColor() const
	{
		FColor FillColor = Color;
		FillColor.A = static_cast<uint8>(FillAlpha * 255.f);
		return FillColor;
	}
};

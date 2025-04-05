﻿// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyRuler.h"


ATrickyRuler::ATrickyRuler()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsEditorOnlyActor = true;

	RootComponent = CreateEditorOnlyDefaultSubobject<USceneComponent>(TEXT("Root"));
}

bool ATrickyRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ATrickyRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	switch (RulerType)
	{
	case ERulerType::Line:
		Dimensions = FString::Printf(TEXT("Length: %.2f m"), LineRuler.GetLengthInMeters());
		break;

	case ERulerType::Circle:
		Dimensions = FString::Printf(TEXT("Radius: %.2f m"), CircleRuler.GetRadiusInMeters());
		break;

	case ERulerType::Sphere:
		Dimensions = FString::Printf(TEXT("Radius: %.2f m"), SphereRuler.GetRadiusInMeters());
		break;

	case ERulerType::Box:
		const FVector Length = BoxRuler.GetLengthInMeters();
		Dimensions = FString::Printf(TEXT("X: %.2f m\nY: %.2f m\nZ: %.2f m"), Length.X, Length.Y, Length.Z);
		break;
	}
}

void ATrickyRuler::BeginPlay()
{
	Super::BeginPlay();
}

void ATrickyRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (RulerType)
	{
	case ERulerType::Line:
		DrawLineRuler(GetActorForwardVector(), LineRuler.Length, LineRuler.Color, LineRuler.Thickness);
		break;
	case ERulerType::Circle:
		DrawCircleRuler();
		break;
	case ERulerType::Sphere:
		DrawSphereRuler();
		break;
	case ERulerType::Cylinder:
		DrawCylinderRuler();
		break;
	case ERulerType::Capsule:
		DrawCapsuleRuler();
		break;
	case ERulerType::Box:
		DrawBoxRuler();
		break;
	default:
		break;
	}
}

void ATrickyRuler::DrawLineRuler(const FVector& Direction,
                                 const float Length,
                                 const FColor& Color,
                                 const float Thickness) const
{
	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Direction * Length;
	DrawDebugLine(GetWorld(), LineStart, LineEnd, Color, false, 0.f, 0, Thickness);
}

void ATrickyRuler::DrawCircleRuler() const
{
	DrawDebugCircle(GetWorld(),
	                GetActorLocation(),
	                CircleRuler.Radius,
	                CircleRuler.Segments,
	                CircleRuler.Color,
	                false,
	                0.f,
	                0,
	                CircleRuler.Thickness,
	                GetActorRightVector(),
	                GetActorForwardVector());
}

void ATrickyRuler::DrawSphereRuler() const
{
	DrawDebugSphere(GetWorld(),
	                GetActorLocation(),
	                SphereRuler.Radius,
	                SphereRuler.Segments,
	                SphereRuler.Color,
	                false,
	                0.f,
	                0,
	                SphereRuler.Thickness);
}

void ATrickyRuler::DrawCylinderRuler() const
{
}

void ATrickyRuler::DrawCapsuleRuler() const
{
}

void ATrickyRuler::DrawBoxRuler() const
{
	const UWorld* World = GetWorld();
	const FVector Center = GetActorLocation();
	const FQuat Rotation = GetActorRotation().Quaternion();
	const FVector Extent = FVector(BoxRuler.LengthX, BoxRuler.LengthY, BoxRuler.LengthZ) * 0.5f;
	DrawDebugBox(World,
	             Center,
	             Extent,
	             Rotation,
	             BoxRuler.Color,
	             false,
	             0.f,
	             0,
	             BoxRuler.Thickness);

	if (BoxRuler.bIsFilled)
	{
		DrawDebugSolidBox(World,
		                  Center,
		                  Extent - 0.1,
		                  Rotation,
		                  BoxRuler.GetFillColor(),
		                  false,
		                  0.f,
		                  0);
	}
}

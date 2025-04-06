// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyRuler.h"

#include "Components/BillboardComponent.h"


ATrickyRuler::ATrickyRuler()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsEditorOnlyActor = true;

	RootComponent = CreateEditorOnlyDefaultSubobject<USceneComponent>(TEXT("Root"));

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	if (Billboard)
	{
		Billboard->SetupAttachment(GetRootComponent());

		struct FConstructorStatics
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture;
			FName ID_Misc;
			FText NAME_Misc;

			FConstructorStatics()
				: SpriteTexture(TEXT("/Engine/EditorResources/S_TargetPoint"))
				  , ID_Misc(TEXT("Misc"))
				  , NAME_Misc(NSLOCTEXT("SpriteCategory", "Misc", "Misc"))
			{
			}
		};

		static FConstructorStatics ConstructorStatics;
		Billboard->SetSprite(ConstructorStatics.SpriteTexture.Object);
		SpriteScale = 0.5;
	}
}

bool ATrickyRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ATrickyRuler::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetActorScale3D(FVector::One());
	UpdateDimensions();
}

void ATrickyRuler::PostInitProperties()
{
	Super::PostInitProperties();

	UpdateDimensions();
}

void ATrickyRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (RulerType)
	{
	case ERulerType::Line:
		DrawLineRuler();
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
	case ERulerType::Cone:
		DrawConeRuler();
		break;
	default:
		break;
	}
}

void ATrickyRuler::UpdateDimensions()
{
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

	case ERulerType::Cylinder:
		const FVector2D CylinderDimensions = CylinderRuler.GetDimensionsInMeters();
		Dimensions =
			FString::Printf(TEXT("Radius: %.2f m\nHeight: %.2f m"), CylinderDimensions.X, CylinderDimensions.Y);
		break;

	case ERulerType::Capsule:
		CapsuleRuler.ClampHeight();
		const FVector2D CapsuleDimensions = CapsuleRuler.GetDimensionsInMeters();
		Dimensions =
			FString::Printf(TEXT("Radius: %.2f m\nHeight: %.2f m"), CapsuleDimensions.X, CapsuleDimensions.Y);
		break;

	case ERulerType::Box:
		const FVector Length = BoxRuler.GetLengthInMeters();
		Dimensions = FString::Printf(TEXT("X: %.2f m\nY: %.2f m\nZ: %.2f m"), Length.X, Length.Y, Length.Z);
		break;

	case ERulerType::Cone:
		Dimensions = FString::Printf(
			TEXT("Length: %.2f m\nAngle: %d deg"), ConeRuler.GetLengthInMeters(), ConeRuler.Angle);
		break;
	}
}

void ATrickyRuler::DrawLineRuler() const
{
	const FVector Direction = GetActorForwardVector();
	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Direction * LineRuler.Length;
	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              LineRuler.Color,
	              false,
	              0.f,
	              0,
	              LineRuler.Thickness);

	DrawMarker(LineStart);
	DrawMarker(LineEnd);
}

void ATrickyRuler::CalculateMarkerPositions(const FVector& Origin,
                                            const FVector& Direction,
                                            FVector& StartPos,
                                            FVector& EndPos) const
{
	StartPos = Origin - Direction * LineRuler.MarkLength;
	EndPos = StartPos + Direction * (LineRuler.MarkLength * 2);
}

void ATrickyRuler::DrawMarker(const FVector& Origin) const
{
	const FVector Direction = GetActorForwardVector();
	FVector MarkerDirection = Direction.RotateAngleAxis(-90.f, GetActorRightVector());
	FVector MarkerStart = FVector::ZeroVector;
	FVector MarkerEnd = FVector::ZeroVector;
	CalculateMarkerPositions(Origin, MarkerDirection, MarkerStart, MarkerEnd);
	DrawDebugLine(GetWorld(),
	              MarkerStart,
	              MarkerEnd,
	              LineRuler.Color,
	              false,
	              0.f,
	              0,
	              LineRuler.Thickness);

	MarkerDirection = Direction.RotateAngleAxis(90.f, GetActorUpVector());
	CalculateMarkerPositions(Origin, MarkerDirection, MarkerStart, MarkerEnd);
	DrawDebugLine(GetWorld(),
	              MarkerStart,
	              MarkerEnd,
	              LineRuler.Color,
	              false,
	              0.f,
	              0,
	              LineRuler.Thickness);
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
	const FVector Location = GetActorLocation();
	const FVector UpVector = GetActorUpVector();
	const FVector StartLocation = CylinderRuler.bCenterOrigin
		                              ? Location - UpVector * CylinderRuler.GetHalfHeight()
		                              : Location;
	const FVector EndLocation = StartLocation + UpVector * CylinderRuler.Height;
	DrawDebugCylinder(GetWorld(),
	                  StartLocation,
	                  EndLocation,
	                  CylinderRuler.Radius,
	                  CylinderRuler.Segments,
	                  CylinderRuler.Color,
	                  false,
	                  0.f,
	                  0,
	                  CylinderRuler.Thickness);
}

void ATrickyRuler::DrawCapsuleRuler() const
{
	const FVector Center = CapsuleRuler.bCenterOrigin
		                       ? GetActorLocation()
		                       : GetActorLocation() + GetActorUpVector() * CapsuleRuler.GetHalfHeight();
	DrawDebugCapsule(GetWorld(),
	                 Center,
	                 CapsuleRuler.GetHalfHeight(),
	                 CapsuleRuler.Radius,
	                 GetActorRotation().Quaternion(),
	                 CapsuleRuler.Color,
	                 false,
	                 0.f,
	                 0,
	                 CapsuleRuler.Thickness);
}

void ATrickyRuler::DrawBoxRuler() const
{
	const UWorld* World = GetWorld();
	const FVector Center = BoxRuler.bCenterOrigin
		                       ? GetActorLocation()
		                       : GetActorLocation() + GetActorUpVector() * BoxRuler.LengthZ * 0.5f;
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

void ATrickyRuler::DrawConeRuler() const
{
	const float AngleRad = FMath::DegreesToRadians(ConeRuler.GetHalfAngle());
	DrawDebugCone(GetWorld(),
	              GetActorLocation(),
	              GetActorForwardVector(),
	              ConeRuler.Length,
	              AngleRad,
	              AngleRad * !ConeRuler.bIsFlat,
	              ConeRuler.Segments,
	              ConeRuler.Color,
	              false,
	              0.f,
	              0,
	              ConeRuler.Thickness);
}

// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyRuler.h"

#include "TrickyDebugTextComponent.h"
#include "Components/BillboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"


ATrickyRuler::ATrickyRuler()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsEditorOnlyActor = true;

	RootComponent = CreateEditorOnlyDefaultSubobject<USceneComponent>(TEXT("Root"));

	TrickyDebugTextComponent = CreateEditorOnlyDefaultSubobject<UTrickyDebugTextComponent>(TEXT("DebugText"));
	TrickyDebugTextComponent->SetupAttachment(GetRootComponent());
	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));

	if (BillboardComponent)
	{
		BillboardComponent->SetupAttachment(GetRootComponent());
		UTexture2D* BillboardTexture = LoadObject<UTexture2D>(this, TEXT("/Engine/EditorResources/S_TargetPoint"));
		BillboardComponent->SetSprite(BillboardTexture);
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

void ATrickyRuler::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
	UpdateDimensions();
}

void ATrickyRuler::PostLoad()
{
	Super::PostLoad();
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
		DebugTextData.Color = LineRuler.Color;
		break;

	case ERulerType::Circle:
		Dimensions = FString::Printf(TEXT("Radius: %.2f m"), CircleRuler.GetRadiusInMeters());
		DebugTextData.Color = CircleRuler.Color;
		break;

	case ERulerType::Sphere:
		Dimensions = FString::Printf(TEXT("Radius: %.2f m"), SphereRuler.GetRadiusInMeters());
		DebugTextData.Color = SphereRuler.Color;
		break;

	case ERulerType::Cylinder:
		const FVector2D CylinderDimensions = CylinderRuler.GetDimensionsInMeters();
		Dimensions =
			FString::Printf(TEXT("Radius: %.2f m\nHeight: %.2f m"), CylinderDimensions.X, CylinderDimensions.Y);
		DebugTextData.Color = CylinderRuler.Color;
		break;

	case ERulerType::Capsule:
		CapsuleRuler.ClampHeight();
		const FVector2D CapsuleDimensions = CapsuleRuler.GetDimensionsInMeters();
		Dimensions =
			FString::Printf(TEXT("Radius: %.2f m\nHeight: %.2f m"), CapsuleDimensions.X, CapsuleDimensions.Y);
		DebugTextData.Color = CapsuleRuler.Color;
		break;

	case ERulerType::Box:
		const FVector Length = BoxRuler.GetLengthInMeters();
		Dimensions = FString::Printf(TEXT("X: %.2f m\nY: %.2f m\nZ: %.2f m"), Length.X, Length.Y, Length.Z);
		DebugTextData.Color = BoxRuler.Color;
		break;

	case ERulerType::Cone:
		Dimensions = FString::Printf(
			TEXT("Length: %.2f m\nAngle: %d deg"), ConeRuler.GetLengthInMeters(), ConeRuler.Angle);
		DebugTextData.Color = ConeRuler.Color;
		break;
	}

	const FString Delimiter = TEXT("\n==========\n");
	const FString DebugText = FString::Printf(TEXT("%s%s%s%s%s"),
	                                          *Delimiter,
	                                          *GetActorNameOrLabel(),
	                                          *Delimiter,
	                                          *Dimensions,
	                                          *Delimiter);
	DebugTextData.Text = DebugText;
	DebugTextData.TextScale = DebugTextScale;
	TrickyDebugTextComponent->SetDebugLabel(DebugTextData);
	TrickyDebugTextComponent->SetDrawDebug(bShowDebugText);
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

	if (LineRuler.bShowMarkers)
	{
		const int32 MarkLength = LineRuler.MarkerLength;
		const int32 MarkersAmount = MarkLength > 0 ? (LineRuler.Length / LineRuler.MarkersDistance) + 1 : 0;

		if (MarkersAmount <= 0)
		{
			return;
		}
		
		for (int32 i = 1; i < MarkersAmount; ++i)
		{
			FVector MarkerLocation = LineStart + Direction * (i * LineRuler.MarkersDistance);
			DrawMarker(MarkerLocation);
		}
	}
}

void ATrickyRuler::CalculateMarkerPositions(const FVector& Origin,
                                            const FVector& Direction,
                                            FVector& StartPos,
                                            FVector& EndPos) const
{
	StartPos = Origin - Direction * LineRuler.MarkerLength;
	EndPos = StartPos + Direction * (LineRuler.MarkerLength * 2);
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
	const FVector Center = GetActorLocation();
	DrawDebugSphere(GetWorld(),
	                Center,
	                SphereRuler.Radius,
	                SphereRuler.Segments,
	                SphereRuler.Color,
	                false,
	                0.f,
	                0,
	                SphereRuler.Thickness);

	DrawRadiusLines(Center,
	                GetActorForwardVector(),
	                SphereRuler.Radius,
	                SphereRuler.Color,
	                SphereRuler.Thickness);
	DrawRadiusLines(Center,
	                GetActorRightVector(),
	                SphereRuler.Radius,
	                SphereRuler.Color,
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

	DrawRadiusLines(Location,
	                GetActorForwardVector(),
	                CylinderRuler.Radius,
	                CylinderRuler.Color,
	                CylinderRuler.Thickness);
	DrawRadiusLines(Location,
	                GetActorRightVector(),
	                CylinderRuler.Radius,
	                CylinderRuler.Color,
	                CylinderRuler.Thickness);
}

void ATrickyRuler::DrawRadiusLines(const FVector& Origin,
                                   const FVector& Direction,
                                   const float Radius,
                                   const FColor& Color,
                                   const float Thickness) const
{
	FVector LineStart = Origin - Direction * Radius;
	FVector LineEnd = Origin + Direction * Radius;
	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              0,
	              0,
	              Thickness);
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

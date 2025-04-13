// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickySplineRuler.h"

#include "TrickyDebugTextComponent.h"
#include "TrickySplineComponent.h"


ATrickySplineRuler::ATrickySplineRuler()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsEditorOnlyActor = true;

	SplineComponent = CreateEditorOnlyDefaultSubobject<UTrickySplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);
	SplineComponent->SetUnselectedSplineSegmentColor(FColor::Purple);
	SplineComponent->SetSelectedSplineSegmentColor(FColor::Magenta);
	SplineComponent->SetTangentColor(FColor::Yellow);

	DebugTextComponent = CreateEditorOnlyDefaultSubobject<UTrickyDebugTextComponent>(TEXT("DebugTextComponent"));
	DebugTextComponent->SetupAttachment(GetRootComponent());
}

bool ATrickySplineRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ATrickySplineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetSplineProperties();
	UpdateDebugText();
}

inline void ATrickySplineRuler::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (int32 i = 0; i < GetLastSplinePoint(); ++i)
	{
		const float InputKey = static_cast<float>(i) + 0.5;
		const FVector UpVector = SplineComponent->GetUpVectorAtSplineInputKey(InputKey, ESplineCoordinateSpace::World);
		const FVector SectionDirection = SplineComponent->GetDirectionAtSplineInputKey(
			InputKey, ESplineCoordinateSpace::World).RotateAngleAxis(180.f, UpVector);
		const FVector SectionLocation = SplineComponent->GetLocationAtSplineInputKey(
			InputKey, ESplineCoordinateSpace::World);
		constexpr float Angle = FMath::DegreesToRadians(ArrowAngleDeg);
		const FColor Color = SplineComponent->EditorUnselectedSplineSegmentColor.ToFColor(true);
		DrawDebugCone(GetWorld(),
		              SectionLocation - SectionDirection * (ArrowLength * 0.5f),
		              SectionDirection,
		              ArrowLength,
		              Angle,
		              Angle,
		              32,
		              Color,
		              false,
		              0.f,
		              0,
		              2.f);
	}
}

void ATrickySplineRuler::SetTypeToLinear() const
{
	SetSplinePointsType(ESplinePointType::Linear);
}

void ATrickySplineRuler::SetTypeToCurve() const
{
	SetSplinePointsType(ESplinePointType::Curve);
}

void ATrickySplineRuler::SetSplinePointsType(const ESplinePointType::Type NewType) const
{
	const int32 PointsNum = SplineComponent->GetNumberOfSplinePoints();

	if (PointsNum <= 0)
	{
		return;
	}

	for (int32 i = 0; i < PointsNum; ++i)
	{
		SplineComponent->SetSplinePointType(i, NewType);
	}
}

void ATrickySplineRuler::SetSplineProperties()
{
	SplineComponent->SetLocationAtSplinePoint(0, FVector::Zero(), ESplineCoordinateSpace::Local, true);
}

void ATrickySplineRuler::UpdateDebugText()
{
	DebugTextData.Empty();
	UpdatePointsDebugText();
	UpdateSectionsDebugText();
	DebugTextComponent->SetDebugLabels(DebugTextData);
}

void ATrickySplineRuler::UpdatePointsDebugText()
{
	float Distance = GetDistanceAtSplinePoint(GetLastSplinePoint());
	FTrickyDebugTextData PointDebugText;
	PointDebugText.Color = DebugTextColor;
	PointDebugText.Text = FString::Printf(
		TEXT("==========\n%s\n==========\nLength: %.2f m\n=========="), *GetActorNameOrLabel(), Distance / 100.f);
	DebugTextData.Add(PointDebugText);

	if (bShowDistancePerPoint)
	{
		PointDebugText.bUseCustomLocation = true;

		for (int32 i = 1; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
		{
			Distance = GetDistanceAtSplinePoint(i);
			PointDebugText.Text = FString::Printf(
				TEXT("----------\nPoint %d\n----------\nLength: %.2f m\n----------"), i, Distance / 100.f);
			PointDebugText.Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			DebugTextData.Add(PointDebugText);
		}
	}
}

void ATrickySplineRuler::UpdateSectionsDebugText()
{
	if (!bShowSectionsLength)
	{
		return;
	}

	FTrickyDebugTextData SectionDebugText;
	SectionDebugText.Color = DebugTextColor;
	SectionDebugText.bUseCustomLocation = true;

	for (int32 i = 0; i < GetLastSplinePoint(); ++i)
	{
		const float Length = GetDistanceAtSplinePoint(i + 1) - GetDistanceAtSplinePoint(i);
		const float Distance = SplineComponent->GetDistanceAlongSplineAtSplineInputKey(static_cast<float>(i) + 0.5);
		FVector SectionLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
			Distance, ESplineCoordinateSpace::World);
		SectionDebugText.Text = FString::Printf(
			TEXT("----------\nSection %d\n----------\nLength: %.2f m\n----------"), i + 1, Length / 100.f);
		SectionDebugText.Location = SectionLocation;
		DebugTextData.Add(SectionDebugText);
	}
}

int32 ATrickySplineRuler::GetLastSplinePoint() const
{
	const int32 PointsNum = SplineComponent->GetNumberOfSplinePoints();
	return SplineComponent->IsClosedLoop() ? PointsNum : PointsNum - 1;
}

float ATrickySplineRuler::GetDistanceAtSplinePoint(const int32 PointIndex) const
{
	return SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointIndex);
}

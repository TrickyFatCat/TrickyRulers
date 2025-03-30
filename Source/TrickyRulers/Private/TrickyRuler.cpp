// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


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

	LineRuler.UpdateLength();
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

// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickyRuler.generated.h"

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

	UPROPERTY(EditAnywhere,
		Category="Ruler",
		meta=(Units="cm", ClampMin=1, UIMin=1))
	int32 Length = 100;

	UPROPERTY(VisibleAnywhere, Category="Ruler")
	FString Meters = TEXT("1 m");

	UPROPERTY(EditAnywhere,
		Category="Ruler",
		meta=(ClampMin=1.0f, ClampMax=10.0f, UIMin=1.0f, UIMax=10.0f, Delta=1.0f))
	float Thickness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ruler", meta=(HideAlphaChannel))
	FColor Color = FColor::Red;

	void UpdateLength()
	{
		const float LengthMeters = static_cast<float>(Length) / 100.f;
		Meters = FString::Printf(TEXT("%.2f m"), LengthMeters);
	}
};

UCLASS(HideCategories=(Collision, Actor, Input, Rendering, Replication, Cooking, HLOD, LevelInstance, DataLayers, Networking, WorldPartition, Physics))
class TRICKYRULERS_API ATrickyRuler : public AActor
{
	GENERATED_BODY()

public:
	ATrickyRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Ruler")
	ERulerType RulerType = ERulerType::Line;

	// Line ruler properties
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Line", EditConditionHides))
	FLineRulerProperties LineRuler;

private:
	UFUNCTION()
	void DrawLineRuler(const FVector& Direction, const float Length, const FColor& Color, const float Thickness) const;
};

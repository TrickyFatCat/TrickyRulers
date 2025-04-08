// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickyRulerProperties.h"
#include "TrickyRuler.generated.h"

UCLASS(
	HideCategories=(Collision, Actor, Input, Rendering, Replication, Cooking, HLOD, LevelInstance, DataLayers,
		Networking, WorldPartition, Physics, Events, "Actor Tick"))
class TRICKYRULERS_API ATrickyRuler : public AActor
{
	GENERATED_BODY()

public:
	ATrickyRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category="Ruler")
	bool bLockEditing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ruler", meta=(EditCondition="!bLockEditing"))
	ERulerType RulerType = ERulerType::Line;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ruler")
	FString Dimensions = TEXT("TO BE CALCULATED");

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Line && !bLockEditing", EditConditionHides))
	FLineRulerProperties LineRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Circle && !bLockEditing", EditConditionHides))
	FCircleRulerProperties CircleRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Sphere && !bLockEditing", EditConditionHides))
	FSphereRulerProperties SphereRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Cylinder && !bLockEditing", EditConditionHides))
	FCylinderRulerProperties CylinderRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Capsule && !bLockEditing", EditConditionHides))
	FCapsuleRulerProperties CapsuleRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Box && !bLockEditing", EditConditionHides))
	FBoxRulerProperties BoxRuler;

	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Cone && !bLockEditing", EditConditionHides))
	FConeRulerProperties ConeRuler;

private:
	UPROPERTY()
	TObjectPtr<UBillboardComponent> Billboard = nullptr;

	UFUNCTION()
	void UpdateDimensions();

	UFUNCTION()
	void DrawLineRuler() const;

	void CalculateMarkerPositions(const FVector& Origin,
	                              const FVector& Direction,
	                              FVector& StartPos,
	                              FVector& EndPos) const;

	void DrawMarker(const FVector& Origin) const;

	UFUNCTION()
	void DrawCircleRuler() const;

	UFUNCTION()
	void DrawSphereRuler() const;

	UFUNCTION()
	void DrawCylinderRuler() const;

	UFUNCTION()
	void DrawRadiusLines(const FVector& Origin, const FVector& Direction) const;

	UFUNCTION()
	void DrawCapsuleRuler() const;

	UFUNCTION()
	void DrawBoxRuler() const;

	UFUNCTION()
	void DrawConeRuler() const;
};

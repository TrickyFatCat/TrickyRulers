// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "TrickyDebugTextComponent.h"
#include "GameFramework/Actor.h"
#include "TrickyRulerProperties.h"
#include "TrickyRuler.generated.h"

/**
 * Represents a ruler actor that can calculate and visualize different measurement shapes,
 * such as line, circle, sphere, cylinder, capsule, box, and cone.
 *
 * @warning this is Editor Only actor
 */
UCLASS(HideCategories=(Collision, Actor, Input, Rendering, Replication, Cooking, HLOD, LevelInstance,
	DataLayers, Networking, WorldPartition, Physics, Events, "Actor Tick"))
class TRICKYRULERS_API ATrickyRuler : public AActor
{
	GENERATED_BODY()

public:
	ATrickyRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;

	virtual void PostEditMove(bool bFinished) override;

	virtual void PostLoad() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * Determines whether the editing of the ruler properties is locked.
	 */
	UPROPERTY(EditAnywhere, Category="Ruler")
	bool bLockEditing = false;

	/**
	 * Determines whether the debug text with dimensions should be shown in viewport.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ruler", meta=(EditCondition="!bLockEditing"))
	bool bShowDebugText = true;

	/**
	 * Determines current ruler type.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ruler", meta=(EditCondition="!bLockEditing"))
	ERulerType RulerType = ERulerType::Line;

	/**
	 * Scale of the debug text.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		AdvancedDisplay,
		meta=(EditCondition="bShowDebugText", ClampMin=1, UIMin=1, ClampMax=2, UIMax=2, Delta=0.1))
	float DebugTextScale = 1.f;

	/**
	 * Dimensions of the current ruler type in meters.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ruler")
	FString Dimensions = TEXT("TO BE CALCULATED");

	UPROPERTY()
	FTrickyDebugTextData DebugTextData{Dimensions, false, FVector::Zero(), FColor::Magenta};

	/**
	 * Defines the properties specific to the line ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Line && !bLockEditing", EditConditionHides))
	FLineRulerProperties LineRuler;

	/**
	 * Defines the properties specific to the circle ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Circle && !bLockEditing", EditConditionHides))
	FCircleRulerProperties CircleRuler;

	/**
	 * Defines the properties specific to the sphere ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Sphere && !bLockEditing", EditConditionHides))
	FSphereRulerProperties SphereRuler;

	/**
	 * Defines the properties specific to the cylinder ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Cylinder && !bLockEditing", EditConditionHides))
	FCylinderRulerProperties CylinderRuler;

	/**
	 * Defines the properties specific to the capsule ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Capsule && !bLockEditing", EditConditionHides))
	FCapsuleRulerProperties CapsuleRuler;

	/**
	 * Defines the properties specific to the box ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Box && !bLockEditing", EditConditionHides))
	FBoxRulerProperties BoxRuler;

	/**
	 * Defines the properties specific to the cone ruler type.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Cone && !bLockEditing", EditConditionHides))
	FConeRulerProperties ConeRuler;

private:
	UPROPERTY()
	TObjectPtr<UBillboardComponent> BillboardComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UTrickyDebugTextComponent> TrickyDebugTextComponent = nullptr;

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
	void DrawRadiusLines(const FVector& Origin,
	                     const FVector& Direction,
	                     const float Radius,
	                     const FColor& Color,
	                     const float Thickness) const;

	UFUNCTION()
	void DrawCapsuleRuler() const;

	UFUNCTION()
	void DrawBoxRuler() const;

	UFUNCTION()
	void DrawConeRuler() const;
};

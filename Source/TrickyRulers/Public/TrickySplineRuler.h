// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickySplineRuler.generated.h"

class UTrickySplineComponent;
struct FTrickyDebugTextData;
class UTrickyDebugTextComponent;

namespace ESplinePointType
{
	enum Type : int;
}

UCLASS(HideCategories=(Collision, Actor, Input, Rendering, Replication, Cooking, HLOD, LevelInstance,
	DataLayers, Networking, WorldPartition, Physics, Events, "Actor Tick"))
class TRICKYRULERS_API ATrickySplineRuler : public AActor
{
	GENERATED_BODY()

public:
	ATrickySplineRuler();

protected:
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTrickySplineComponent> SplineComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UTrickyDebugTextComponent> DebugTextComponent = nullptr;

	/**
	 * Determines if length per point should be shown.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DebugText")
	bool bShowDistancePerPoint = false;

	/**
	 * Determines if length of each sector should be shown.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DebugText")
	bool bShowSectionsLength = false;

	/**
	 * Color of the debug text.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DebugText")
	FColor DebugTextColor = FColor::Magenta;

private:
	constexpr static float ArrowLength = 50.f;
	constexpr static float ArrowAngleDeg = 12.f;
	
	UPROPERTY()
	TArray<FTrickyDebugTextData> DebugTextData;

	/**
	 * Sets all points type to linear.
	 */
	UFUNCTION(CallInEditor, Category = "Spline", meta = (DisplayAfter="Transform"))
	void SetTypeToLinear() const;

	/**
	 * Sets all points type to curve.
	 */
	UFUNCTION(CallInEditor, Category = "Spline", meta = (DisplayAfter="Transform"))
	void SetTypeToCurve() const;

	UFUNCTION()
	void SetSplinePointsType(ESplinePointType::Type NewType) const;

	UFUNCTION()
	void SetSplineProperties();

	UFUNCTION()
	void UpdateDebugText();
	
	UFUNCTION()
	void UpdatePointsDebugText();

	UFUNCTION()
	void UpdateSectionsDebugText();

	UFUNCTION()
	int32 GetLastSplinePoint() const;

	UFUNCTION()
	float GetDistanceAtSplinePoint(const int32 PointIndex) const;

};


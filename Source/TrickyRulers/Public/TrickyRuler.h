// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickyRulerProperties.h"
#include "TrickyRuler.generated.h"

UCLASS(HideCategories=(Collision, Actor, Input, Rendering, Replication, Cooking, HLOD, LevelInstance, DataLayers, Networking, WorldPartition, Physics, Events, ActorTick))
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
		meta=(EditCondition="RulerType==ERulerType::Box && !bLockEditing", EditConditionHides))
	FBoxRulerProperties BoxRuler;
	

private:
	UFUNCTION()
	void DrawLineRuler(const FVector& Direction, const float Length, const FColor& Color, const float Thickness) const;

	UFUNCTION()
	void DrawCircleRuler() const;

	UFUNCTION()
	void DrawSphereRuler() const;

	UFUNCTION()
	void DrawCylinderRuler() const;

	UFUNCTION()
	void DrawCapsuleRuler() const;

	UFUNCTION()
	void DrawBoxRuler() const;
};

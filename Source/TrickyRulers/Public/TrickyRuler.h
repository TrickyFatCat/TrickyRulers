// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrickyRulerProperties.h"
#include "TrickyRuler.generated.h"

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

	UPROPERTY(EditInstanceOnly,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Line", EditConditionHides))
	FLineRulerProperties LineRuler;

	UPROPERTY(EditInstanceOnly,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Circle", EditConditionHides))
	FCircleRulerProperties CircleRuler;
	
	UPROPERTY(EditInstanceOnly,
		BlueprintReadOnly,
		Category="Ruler",
		meta=(EditCondition="RulerType==ERulerType::Box", EditConditionHides))
	FBoxRulerProperties BoxRuler;
	

private:
	UFUNCTION()
	void DrawLineRuler(const FVector& Direction, const float Length, const FColor& Color, const float Thickness) const;

	UFUNCTION()
	void DrawCircleRuler() const;

	UFUNCTION()
	void DrawBoxRuler() const;
};

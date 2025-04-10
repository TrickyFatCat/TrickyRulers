// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Debug/DebugDrawComponent.h"
#include "TrickyDebugTextComponent.generated.h"

USTRUCT(BlueprintType)
struct FTrickyDebugTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	FString Text = "DebugText";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	bool bUseCustomLocation = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="DebugText",
		meta=(EditCondition="bUseCustomLocation", EditConditionHides))
	FVector Location{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	FLinearColor Color{FColor::Magenta};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText")
	float TextScale = 1.f;
};

struct FDebugSceneProxyData
{
	struct FDebugText
	{
		FString Text;
		FVector Location;
		FColor Color;
		float Scale;

		FDebugText(): Location(FVector::ZeroVector), Color(FColor::Magenta), Scale(1)
		{
		}

		FDebugText(const FTrickyDebugTextData& Label, const FVector& InLocation)
			: Text(Label.Text), Location(InLocation), Color(Label.Color.ToFColor(false)), Scale(Label.TextScale)
		{
		}
	};

	bool bDrawInGame = false;
	TArray<FDebugText> DebugLabels;
};

class FDebugSceneProxy : public FDebugRenderSceneProxy
{
public:
	FDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugSceneProxyData* ProxyData);

	FDebugSceneProxyData ProxyData;
};

class FDebugTextDelegateHelper : public FDebugDrawDelegateHelper
{
public:
	virtual void DrawDebugLabels(UCanvas* Canvas, APlayerController*) override;

	void SetupFromProxy(const FDebugSceneProxy* InSceneProxy);

	bool bDrawDebug = true;

	TArray<FDebugSceneProxyData::FDebugText> DebugLabels;
};


UCLASS(ClassGroup=(TrickyProptotyping), meta=(BlueprintSpawnableComponent))
class TRICKYRULERS_API UTrickyDebugTextComponent : public UDebugDrawComponent
{
	GENERATED_BODY()

public:
	UTrickyDebugTextComponent();

protected:
	FDebugTextDelegateHelper DebugDrawDelegateManager;

	/**
	 * Determines if the debug text should be drawn in viewport.
	 */
	UPROPERTY(EditAnywhere, Category="DebugText")
	bool bDrawDebug = true;

	/**
	 * Current debug text data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DebugText",
		meta=(EditCondition = "!bDrawOneLabel && bDrawDebug", EditConditionHides))
	TArray<FTrickyDebugTextData> DebugLabels;

	/**
	 * Determines if the debug text should be shown in game.
	 */
	UPROPERTY(EditAnywhere, Category="DebugText", meta=(EditCondition="bDrawDebug"))
	bool bDrawInGame = false;

	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy() override;

	virtual FDebugDrawDelegateHelper& GetDebugDrawDelegateHelper() override { return DebugDrawDelegateManager; }

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

public:
	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDebugLabel(const FTrickyDebugTextData& LabelData);

	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDebugLabels(const TArray<FTrickyDebugTextData>& LabelsData);

	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDrawDebug(const bool Value);

	UFUNCTION(BlueprintCallable, Category="DebugText")
	void SetDrawInGame(const bool Value);
};

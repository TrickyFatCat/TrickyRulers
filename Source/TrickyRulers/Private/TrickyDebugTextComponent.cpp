// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "TrickyDebugTextComponent.h"

#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

FDebugSceneProxy::FDebugSceneProxy(const UPrimitiveComponent* InComponent,
                                   FDebugSceneProxyData* ProxyData) : FDebugRenderSceneProxy(InComponent)
{
	this->ProxyData = *ProxyData;

	DrawType = EDrawType::SolidAndWireMeshes;
	ViewFlagName = "Editor";

	if (ProxyData->bDrawInGame && (InComponent->GetWorld() && InComponent->GetWorld()->IsGameWorld()))
	{
		ViewFlagName = "Game";
	}


	for (const auto& Text : ProxyData->DebugLabels)
	{
		this->Texts.Add({
			Text.Text,
			Text.Location,
			Text.Color,
		});
	}
}

void FDebugTextDelegateHelper::DrawDebugLabels(UCanvas* Canvas, APlayerController* PlayerController)
{
	if (!Canvas || !bDrawDebug)
	{
		return;
	}

	const FDebugSceneProxyData::FDebugText* DebugText = DebugLabels.GetData();
	const FColor OldDrawColor = Canvas->DrawColor;
	Canvas->SetDrawColor(DebugText->Color);
	const FSceneView* View = Canvas->SceneView;
	const UFont* Font = GEngine->GetSmallFont();

	FFontRenderInfo FontRenderInfo;
	FontRenderInfo.bEnableShadow = true;

	for (int32 i = 0; i < DebugLabels.Num(); ++i, ++DebugText)
	{
		if (View->ViewFrustum.IntersectSphere(DebugText->Location, 1.0f))
		{
			const FVector ScreenLoc = Canvas->Project(DebugText->Location);
			Canvas->DrawText(Font,
			                 DebugText->Text,
			                 ScreenLoc.X,
			                 ScreenLoc.Y,
			                 DebugText->Scale,
			                 DebugText->Scale,
			                 FontRenderInfo);
		}
	}

	Canvas->SetDrawColor(OldDrawColor);
}

void FDebugTextDelegateHelper::SetupFromProxy(const FDebugSceneProxy* InSceneProxy)
{
	DebugLabels.Reset();
	DebugLabels.Append(InSceneProxy->ProxyData.DebugLabels);
}

UTrickyDebugTextComponent::UTrickyDebugTextComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsEditorOnly = true;
}

FDebugRenderSceneProxy* UTrickyDebugTextComponent::CreateDebugSceneProxy()
{
	FDebugSceneProxyData ProxyData;
	ProxyData.bDrawInGame = bDrawInGame;

	auto AddLabelData = [&](const FTrickyDebugTextData& Label)-> void
	{
		FVector Location = Label.bUseCustomLocation ? Label.Location : GetComponentLocation();
		ProxyData.DebugLabels.Add({Label, Location});
	};

	if (DebugLabels.Num() == 0)
	{
		return nullptr;
	}

	for (const FTrickyDebugTextData& Label : DebugLabels)
	{
		AddLabelData(Label);
	}

	FDebugSceneProxy* DebugSceneProxy = new FDebugSceneProxy(this, &ProxyData);

	DebugDrawDelegateManager.bDrawDebug = bDrawDebug;

	if (DebugSceneProxy)
	{
		DebugDrawDelegateManager.SetupFromProxy(DebugSceneProxy);
	}

	return DebugSceneProxy;
}

FBoxSphereBounds UTrickyDebugTextComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FBox(FVector(-1000, -1000, -1000), FVector(1000, 1000, 1000)));
}

void UTrickyDebugTextComponent::SetDebugLabel(const FTrickyDebugTextData& LabelData)
{
	DebugLabels.Empty();
	DebugLabels.Add(LabelData);
	MarkRenderStateDirty();
}

void UTrickyDebugTextComponent::SetDebugLabels(const TArray<FTrickyDebugTextData>& LabelsData)
{
	DebugLabels.Empty();
	DebugLabels = LabelsData;
	MarkRenderStateDirty();
}

void UTrickyDebugTextComponent::SetDrawDebug(const bool Value)
{
	bDrawDebug = Value;
	MarkRenderStateDirty();
}

void UTrickyDebugTextComponent::SetDrawInGame(const bool Value)
{
	bDrawInGame = Value;
	bIsEditorOnly = !bDrawInGame;
	MarkRenderStateDirty();
}

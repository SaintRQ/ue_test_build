// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Visualizers/InteractVisualizer.h"

#include "Game/Misc/InteractWidgetComponent.h"
#include "MoonFunctionLibrary.h"
#include "Game/Widgets/Visualizer/InteractVisualizerWidget.h"


// Sets default values
AInteractVisualizer::AInteractVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	interactWidgetClassAssetID = "InteractVisualizerWidgetAssetID";
	interactWidgetClass = UMoonFunctionLibrary::GetWidgetByID(interactWidgetClassAssetID);


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WidgetComponent = CreateDefaultSubobject<UInteractWidgetComponent>(TEXT("WidgetComponent"));

	WidgetComponent->SetupAttachment(RootComponent);

	WidgetComponent->SetWidgetClass(interactWidgetClass);


	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(100, 100));
	
	WidgetComponent->SetPivot(FVector2D(0.5f, 1.0f));
	WidgetComponent->SetGeometryMode(EWidgetGeometryMode::Plane);
	WidgetComponent->SetTickMode(ETickMode::Enabled);

}


void AInteractVisualizer::InitializeVisualizer(int32 VisualizerType)
{
	if (!GetWorld()) return;

	if(!interactWidgetReference)
	{
		if(!WidgetComponent || !WidgetComponent->GetWidget())
		{
			UE_LOG(LogTemp, Error, TEXT("InteractVisualizer: WidgetComponent or WidgetComponent->GetWidget() is not valid"));
			return;
		}
		interactWidgetReference = Cast<UInteractVisualizerWidget>(WidgetComponent->GetWidget());
	}

	SetVisualizerType(VisualizerType);	
}

void AInteractVisualizer::SetVisualizerTargets(TArray<AActor*> VisualizerTargets)
{
	visualizerTargets = VisualizerTargets;
	if (interactWidgetReference) interactWidgetReference->SetWidgetTargets(visualizerTargets);
}

void AInteractVisualizer::SetVisualizerType(int32 VisualizerType)
{
	if (!interactWidgetReference) return;

	visualizerType = FMath::Clamp(VisualizerType, 0, 3);

	interactWidgetReference->SwitchVisualizator(visualizerType);
}




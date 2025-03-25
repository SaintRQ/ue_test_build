// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Visualizers/BridgeVisualizer.h"
#include "MoonFunctionLibrary.h"

// Sets default values
ABridgeVisualizer::ABridgeVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	visMeshAssetID = "VisualizerLineAssetID";
	UStaticMesh* visMesh = nullptr;

	auto loadedObject = UMoonFunctionLibrary::GetAssetByID(visMeshAssetID);
	if (loadedObject) visMesh = Cast<UStaticMesh>(loadedObject);
	else UE_LOG(LogTemp, Error, TEXT("BridgeVisualizer::ABridgeVisualizer loaded visualizer mesh is not valid."))


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

	if(visMesh)
	{
		Mesh->SetStaticMesh(visMesh);
	}

}

void ABridgeVisualizer::InitializeVisualizer()
{
	if (Mesh)
	{
		UMaterialInterface* material = Mesh->GetMaterial(0);

		if (material)
		{
			dynamicMaterial = UMaterialInstanceDynamic::Create(material, this);

			if (dynamicMaterial)
			{
				Mesh->SetMaterial(0, dynamicMaterial);
				SetVisualizerColor(FLinearColor::Green * 2.f);
			}
		}
	}
}

void ABridgeVisualizer::SetVisualizerColor(FLinearColor Color)
{
	if (!dynamicMaterial) return;

	dynamicMaterial->SetVectorParameterValue(FName("Emissive"), Color);
}



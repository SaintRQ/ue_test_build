// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Visualizers/BuildVisualizer.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

// Sets default values
ABuildVisualizer::ABuildVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(RootComponent);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

}

void ABuildVisualizer::InitializeVisualizer(FBuildDataTableRow BuildingData)
{
	buildingData = BuildingData;

	if (buildingData.BuildMesh.ToSoftObjectPath().IsAsset())
	{
		FStreamableManager& streamable = UAssetManager::GetStreamableManager();

		streamable.RequestAsyncLoad(buildingData.BuildMesh.ToSoftObjectPath(), [this]()
		{
			if (Mesh)
			{
				auto loadedMesh = buildingData.BuildMesh.Get();

				if (loadedMesh)
				{
					Mesh->SetStaticMesh(loadedMesh);

					FBox MeshBox = loadedMesh->GetBoundingBox();
					FVector Extent = MeshBox.GetExtent();
					FVector Scale = GetActorScale3D();
					FVector ScaledExtent = Extent * Scale;

					meshRadius = FMath::Max3(ScaledExtent.X, ScaledExtent.Y, ScaledExtent.Z);

					dynamicMaterials.Empty();
					int32 MaterialCount = Mesh->GetNumMaterials();
					for (int32 i = 0; i < MaterialCount; i++)
					{
						UMaterialInterface* Material = Mesh->GetMaterial(i);
						if (Material)
						{
							UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
							if (DynamicMaterial)
							{
								Mesh->SetMaterial(i, DynamicMaterial);
								dynamicMaterials.Add(DynamicMaterial);
							
								SetVisualizerColor(FLinearColor::Green * 2.f);

							}
						}
					}


				}
			}
		});
	}
}

void ABuildVisualizer::SetVisualizerColor(FLinearColor Color)
{
	if(dynamicMaterials.Num() > 0)
	{
		for(auto mat : dynamicMaterials)
		{
			if(mat) mat->SetVectorParameterValue(FName("Emissive"), Color);
		}
	}
}





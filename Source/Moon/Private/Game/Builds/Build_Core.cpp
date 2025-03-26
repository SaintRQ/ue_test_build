// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Builds/Build_Core.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Components/SphereComponent.h"
#include "Managers/MeshesInstanceManager.h"
#include "MoonFunctionLibrary.h"

// Sets default values
ABuild_Core::ABuild_Core()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));		
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));

	Collider->SetupAttachment(RootComponent);

	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Collider->SetGenerateOverlapEvents(true);

}


void ABuild_Core::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (instanceManager) instanceManager->RemoveInstance(meshInstanceID);

	Super::EndPlay(EndPlayReason);
}


void ABuild_Core::InitializeBuilding(FName DataTableID, FBuildDataTableRow BuildingData)
{
	isBuilded = true;

	dataTableID = DataTableID;
	buildingData = BuildingData;

	instanceManager = UMoonFunctionLibrary::GetMeshesInstanceManager(this);

	if (!instanceManager || !buildingData.BuildMesh.ToSoftObjectPath().IsAsset())
	{
		UE_LOG(LogTemp, Error, TEXT("Build_Core: InitializeBuilding: BuildMesh or instanceManager is not valid"));
		return;
	}

	FStreamableManager& streamable = UAssetManager::GetStreamableManager();

	streamable.RequestAsyncLoad(buildingData.BuildMesh.ToSoftObjectPath(), [this]()
	{
		auto loadedMesh = buildingData.BuildMesh.Get();

		if (loadedMesh)
		{
			FBox meshBounds;
			instanceManager->InstanceMesh(loadedMesh, GetActorTransform(), meshInstanceID, meshRadius, meshBounds);

			if(Collider)
			{
				Collider->SetSphereRadius(meshRadius);
			}
		}
		else UE_LOG(LogTemp, Error, TEXT("Build_Core: InitializeBuilding: LoadedMesh is not valid"));
			
	});
}







// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Bridges/Bridge_Core.h"

#include "Components/BoxComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Managers/MeshesInstanceManager.h"
#include "MoonFunctionLibrary.h"


// Sets default values
ABridge_Core::ABridge_Core()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = false;

   RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
   Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));

   Collider->SetupAttachment(RootComponent);
   Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   Collider->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
   Collider->SetGenerateOverlapEvents(true);
}

void ABridge_Core::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (instanceManager) instanceManager->RemoveInstance(meshInstanceID);
	Super::EndPlay(EndPlayReason);
}


void ABridge_Core::InitializeBridge(FName DataTableID, FBridgeDataTableRow BridgeData)
{
   if (isBuilded) return;

   dataTableID = DataTableID;
   bridgeData = BridgeData;

   isBuilded = true;

   instanceManager = UMoonFunctionLibrary::GetMeshesInstanceManager(this);

   if (!instanceManager || !bridgeData.BridgeMesh.ToSoftObjectPath().IsAsset())
   {
	   UE_LOG(LogTemp, Error, TEXT("Bridge_Core: InitializeBridge: BridgeMesh or instanceManager is not valid"));
	   return;
   }
   
   FStreamableManager& streamable = UAssetManager::GetStreamableManager();

   streamable.RequestAsyncLoad(bridgeData.BridgeMesh.ToSoftObjectPath(), [this]()
   {
	   auto loadedMesh = bridgeData.BridgeMesh.Get();

	   if (loadedMesh)
	   {
		   float radius = 0;
		   FBox meshBounds;
		   instanceManager->InstanceMesh(loadedMesh, GetActorTransform(), meshInstanceID, radius, meshBounds);

		   Collider->SetBoxExtent(meshBounds.GetExtent());
	   }
	   else
	   {
		   UE_LOG(LogTemp, Error, TEXT("Bridge_Core: InitializeBridge: Loaded Mesh is not valid"));
	   }

   });
 
}



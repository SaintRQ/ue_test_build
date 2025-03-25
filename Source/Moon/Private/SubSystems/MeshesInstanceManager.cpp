// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/MeshesInstanceManager.h"
#include "Game/Misc/WorldMeshesInstance.h"

void UMeshesInstanceManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UMeshesInstanceManager::OnWorldInitialized);

    IDCounter = -1;
}

void UMeshesInstanceManager::Deinitialize()
{
	Super::Deinitialize();
}

void UMeshesInstanceManager::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
    instancePool.Empty();
    IDCounter = -1;

    if (WorldInstance) 
    {
        WorldInstance->Destroy();
        WorldInstance = nullptr;
    }
}

void UMeshesInstanceManager::InstanceMesh(UStaticMesh* Mesh, FTransform Transform, int32& InstanceID, float& MeshRadius, FBox& MeshBoundingBox)
{
    IDCounter++;
   
    if (!Mesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshesInstanceManager: Mesh is not valid!")); 
        return;
    }

    FInstanceMeshData localData;
    localData.meshRef = Mesh;
    
    TArray<FInstanceMeshData> values;
    instancePool.GenerateValueArray(values);
    
    int32 index = values.Find(localData);
    
    if(values.IsValidIndex(index))  localData.instance = values[index].instance;
    else localData.instance = CreateNewISM(Mesh);
    
    if(!localData.instance)
    {
        UE_LOG(LogTemp, Error, TEXT("MeshesInstanceManager: New InstancedStaticMeshComponent is not valid"));
        return;
    }
    
    localData.instanceID = localData.instance->AddInstanceById(Transform);
    

    FBox MeshBox = Mesh->GetBoundingBox();
    FVector Extent = MeshBox.GetExtent();
    FVector Scale = Transform.GetScale3D();
    FVector ScaledExtent = Extent * Scale;
    
    float radius = FMath::Max3(ScaledExtent.X, ScaledExtent.Y, ScaledExtent.Z);
    
    instancePool.Add(IDCounter, localData);

    InstanceID = IDCounter;
    MeshRadius = radius;
    MeshBoundingBox = MeshBox;
  
}

bool UMeshesInstanceManager::RemoveInstance(int32 InstanceID)
{
    if(instancePool.Contains(InstanceID))
    { 
        if (instancePool[InstanceID].instance)
        {
            if(instancePool[InstanceID].instance->IsValidId(instancePool[InstanceID].instanceID))
            {
                instancePool[InstanceID].instance->RemoveInstanceById(instancePool[InstanceID].instanceID);
            }

            if (instancePool[InstanceID].instance->GetInstanceCount() < 1)
            {
                if (WorldInstance) WorldInstance->DestroyISM(instancePool[InstanceID].instance);
            }
        }

        instancePool.Remove(InstanceID);

        return true;
    }

    return false;
}

UInstancedStaticMeshComponent* UMeshesInstanceManager::CreateNewISM(UStaticMesh* Mesh)
{	
    if (!WorldInstance) CreateNewWorldMeshesInstance();

    if (!WorldInstance) return nullptr;

    return WorldInstance->CreateNewlSM(Mesh);

}

void UMeshesInstanceManager::CreateNewWorldMeshesInstance()
{
    if (WorldInstance || !GetWorld()) return;

    FActorSpawnParameters spawnParameters;
    spawnParameters.bNoFail = true;

    WorldInstance = GetWorld()->SpawnActor<AWorldMeshesInstance>(spawnParameters);
}

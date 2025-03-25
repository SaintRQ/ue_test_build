// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Structs/InstanceMesh/InstanceMeshData.h"
#include "MeshesInstanceManager.generated.h"

/**
 * 
 */

class AWorldMeshesInstance;

UCLASS()
class MOON_API UMeshesInstanceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	int32 IDCounter;

	AWorldMeshesInstance* WorldInstance;

	TMap<int32, FInstanceMeshData> instancePool;

	UInstancedStaticMeshComponent* CreateNewISM(UStaticMesh* Mesh);

	void CreateNewWorldMeshesInstance();

	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);


public:
	
	//Добавить меш к пулу мешей.
	void InstanceMesh(UStaticMesh* Mesh, FTransform Transform, int32& InstanceID, float& MeshRadius, FBox& MeshBoundingBox);

	bool RemoveInstance(int32 InstanceID);

	AWorldMeshesInstance* GetWorldMeshesInstance() const { return WorldInstance; }

	
};

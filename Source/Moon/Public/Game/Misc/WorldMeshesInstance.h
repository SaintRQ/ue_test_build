// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMeshesInstance.generated.h"

UCLASS()
class MOON_API AWorldMeshesInstance : public AActor
{
	GENERATED_BODY()
	
	AWorldMeshesInstance();

	TArray<UInstancedStaticMeshComponent*> poolSM;

public:	
	// Sets default values for this actor's properties
	
	UInstancedStaticMeshComponent* CreateNewlSM(UStaticMesh* Mesh);

	bool DestroyISM(UInstancedStaticMeshComponent* ISM);


};

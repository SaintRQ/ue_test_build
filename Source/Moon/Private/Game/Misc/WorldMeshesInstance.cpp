// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Misc/WorldMeshesInstance.h"
#include "Engine/InstancedStaticMesh.h"


// Sets default values
AWorldMeshesInstance::AWorldMeshesInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}

UInstancedStaticMeshComponent* AWorldMeshesInstance::CreateNewlSM(UStaticMesh* Mesh)
{
   
    UInstancedStaticMeshComponent* NewISM = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());
 
    NewISM->RegisterComponent();  

    NewISM->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

    NewISM->SetStaticMesh(Mesh);
    NewISM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewISM->SetGenerateOverlapEvents(false);

    poolSM.Add(NewISM);

    return NewISM;
}

bool AWorldMeshesInstance::DestroyISM(UInstancedStaticMeshComponent* ISM)
{
	if(ISM && poolSM.Contains(ISM))
	{
		poolSM.Remove(ISM);
		ISM->DestroyComponent();
		return true;
	}

	return false;
}




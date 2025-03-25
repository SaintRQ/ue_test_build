// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/Build/BuildDataTableRow.h"
#include "Build_Core.generated.h"


class USphereComponent;
class UMeshesInstanceManager;

UCLASS()
class MOON_API ABuild_Core : public AActor
{
	GENERATED_BODY()
	
	ABuild_Core();

	int32 meshInstanceID;
	float meshRadius = 200.f;
	UMeshesInstanceManager* instanceManager;

	FName dataTableID;
	FBuildDataTableRow buildingData;
	bool isBuilded;


protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)  override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* Collider;


public:	
	
	virtual void InitializeBuilding(FName DataTableID, FBuildDataTableRow BuildingData);

	float GetMeshRadius() const { return meshRadius; }

	FName GetDataTableID()  const { return dataTableID; }

	FBuildDataTableRow GetBuildingData() const { return buildingData; }

	//Построено ли здание.
	bool IsBuilded() const { return isBuilded; }


};

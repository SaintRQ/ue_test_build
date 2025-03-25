// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/Bridge/BridgeDataTableRow.h"
#include "Bridge_Core.generated.h"

class UBoxComponent;
class UMeshesInstanceManager;

UCLASS()
class MOON_API ABridge_Core : public AActor
{
	GENERATED_BODY()

	ABridge_Core();

protected:

	int32 meshInstanceID;
	UMeshesInstanceManager* instanceManager;

	FName dataTableID;
	FBridgeDataTableRow bridgeData;
	bool isBuilded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* Collider;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)  override;

public:	

	virtual void InitializeBridge(FName DataTableID, FBridgeDataTableRow BridgeData);

	inline FName GetDataTableID() const { return dataTableID; }

	//Построен ли мост.
	inline bool IsBuilded() const { return isBuilded; }


};

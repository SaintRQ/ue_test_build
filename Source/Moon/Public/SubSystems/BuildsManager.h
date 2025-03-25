// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/Build/BuildResourceData.h"
#include "Structs/Build/BuildDataTableRow.h"
#include "BuildsManager.generated.h"

/**
 * 
 */
class ABuild_Core;
class ABridge_Core;

DECLARE_MULTICAST_DELEGATE_OneParam(FEventOnResourcesUpdated, TArray<FBuildResourceData>);

UCLASS()
class MOON_API UBuildsManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:

//------------------------------------------------------------- Settings
	float minDistanceForBridges = 100.0f;
	float maxDistanceForBridges = 1000.0f;
//-------------------------------------------------------------

	FName buildsDataTableAssetID;
	FName bridgesDataTableAssetID;

	UDataTable* buildsDataTable;
	UDataTable* bridgesDataTable;

	TMap<ABuild_Core*, TArray<ABridge_Core*>> buildingsPool;
	TArray<FBuildResourceData> calculatedResources;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	FName DefineBridgeDataTableID(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;

	float GetBuildingRadius(ABuild_Core* Building) const;

	void RecalculateAllResources();

public:

	bool CreateBuilding(FName DataTableID, FTransform Transform, bool RecalculateResources = true);

	bool CreateBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding, bool RecalculateResources = true);

	bool DestroyBuilding(ABuild_Core* Building, bool RecalculateResources = true);

	bool DestroyBridge(ABridge_Core* Bridge, bool RecalculateResources = true);


	FTransform CalculateBridgeTransform(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;
	FTransform CalculateBridgeTransform(FVector StartLocation, FVector EndLocation, float StartOffset, float EndOffset) const;

	// Можно ли начать строительство нового здания? Проверка на кол-во ресурсов и т.д.
	bool CanCreateNewBuilding(FName BuildingTableID) const;

	// Можно ли построить мост между зданиями?
	bool CanCreateNewBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding);

	// Валидное ли место для постойки здания?
	bool IsValidPlace(FVector Location, float CheckRadius) const;

	bool IsValidDistanceForBridge(FVector StartLocation, FVector EndLocation, float DistanceOffset) const;
	bool IsValidDistanceForBridge(float Distance) const;
	bool IsValidDistanceForBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;

	// Возвращает длину моста в метрах.
	float CalculateBridgeSize(FVector StartLocation, FVector EndLocation, float StartOffset, float EndOffset) const;
	// Возвращает длину моста в метрах.
	float CalculateBridgeSize(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;


	// Если ли помеха между зданиями? При любых ошибках, также возвращает true.
	bool HasObstacleBetweenBuildings(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;
	// Если ли помеха между двумя точками?
	bool HasObstacleBetweenLocations(FVector StartLocation, FVector EndLocation, TArray<AActor*> IgnoreList) const;

	// Если ли мост между зданиями? При любых ошибках, также возвращает true.
	bool HasBridgeBetweenBuildings(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const;

	TArray<FBuildResourceData> GetCalculatedResources() const { return calculatedResources; }

	FBuildDataTableRow GetBuildingData(FName DataTableID);

	FVector2D GetMinMaxBridgeDistance() const { return FVector2D(minDistanceForBridges, maxDistanceForBridges); }

	FVector GetBridgeSize() const;

//----------------------------------------------------------- Events

	// Событие на обновление всех ресурсов. Возвращает массив из ресурсов TArray<FBuildResourceData>
	FEventOnResourcesUpdated OnResourcesUpdated;

};

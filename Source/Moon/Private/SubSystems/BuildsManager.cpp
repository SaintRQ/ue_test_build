// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/BuildsManager.h"
#include "MoonFunctionLibrary.h"
#include "Game/Builds/Build_Core.h"
#include "Game/Bridges/Bridge_Core.h"
#include "Enums/Build/BuildResource.h"
#include "ProjectSettings/MoonProjectSettings.h"


void UBuildsManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

    buildsDataTableAssetID = "BuildsDataTable";
    bridgesDataTableAssetID = "BridgesDataTable";

	//-------------------- Загрузка BuildsDataTable --------------------
    auto loadedBuildsDataTableObject = UMoonFunctionLibrary::GetAssetByID(buildsDataTableAssetID);
	if (loadedBuildsDataTableObject) buildsDataTable = Cast<UDataTable>(loadedBuildsDataTableObject);
	else UE_LOG(LogTemp, Error, TEXT("UBuildsManager: AssetBuildsDataTable not found."));

	//-------------------- Загрузка BridgesDataTable --------------------
	auto loadedBridgesDataTableObject = UMoonFunctionLibrary::GetAssetByID(bridgesDataTableAssetID);
	if (loadedBridgesDataTableObject) bridgesDataTable = Cast<UDataTable>(loadedBridgesDataTableObject);
	else UE_LOG(LogTemp, Error, TEXT("UBuildsManager: AssetBridgesDataTable not found."));

    auto settings = UMoonFunctionLibrary::GetGameProjectSettings();
    if(settings)
    {
        minDistanceForBridges = settings->minBridgeDistance;
        maxDistanceForBridges = settings->maxBridgeDistance;
    }
		
}

void UBuildsManager::Deinitialize()
{
	Super::Deinitialize();
}

bool UBuildsManager::CreateBuilding(FName DataTableID, FTransform Transform, bool RecalculateResources)
{
    if (!buildsDataTable || !GetWorld()) return false;

    UWorld* world = GetWorld();

    FBuildDataTableRow* buildingData = buildsDataTable->FindRow<FBuildDataTableRow>(DataTableID, TEXT(""));
    if (!buildingData) return false;   


    ABuild_Core* newBuilding = world->SpawnActor<ABuild_Core>(buildingData->BuildClass, Transform);
    if (newBuilding)
    {
        newBuilding->InitializeBuilding(DataTableID, *buildingData);
        buildingsPool.Add(newBuilding, TArray<ABridge_Core*>());
        if (RecalculateResources) RecalculateAllResources();
        return true;
    }

    return false;

}

bool UBuildsManager::CreateBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding, bool RecalculateResources)
{
    if (!bridgesDataTable && CanCreateNewBridge(FirstBuilding, SecondBuilding)) return false;
    
    FName bridgeTableID = DefineBridgeDataTableID(FirstBuilding, SecondBuilding);

    FBridgeDataTableRow* bridgeData = bridgesDataTable->FindRow<FBridgeDataTableRow>(bridgeTableID, TEXT(""));
    if (!bridgeData) return false;

    UWorld* world = GetWorld();
    if (!world) return false;

    FTransform bridgeTransform = CalculateBridgeTransform(FirstBuilding, SecondBuilding);

    ABridge_Core* newBridge = world->SpawnActor<ABridge_Core>(bridgeData->BridgeClass, bridgeTransform);
    if (newBridge)
    {
        newBridge->InitializeBridge(bridgeTableID, *bridgeData);
        buildingsPool[FirstBuilding].Add(newBridge);
        buildingsPool[SecondBuilding].Add(newBridge);
        if (RecalculateResources) RecalculateAllResources();
        return true;
    }

    return false;
}

bool UBuildsManager::DestroyBuilding(ABuild_Core* Building, bool RecalculateResources)
{
    if (!Building || !buildingsPool.Contains(Building)) return false;

    auto copyBridges = buildingsPool[Building];

    for (ABridge_Core* bridge : copyBridges)
    {
        if (bridge)
        {
            for (auto& el : buildingsPool)
            {
                el.Value.Remove(bridge);
            }
            bridge->Destroy();
        }
    }

    buildingsPool.Remove(Building);
    Building->Destroy();

    if (RecalculateResources)
        RecalculateAllResources();

    return true;
}

bool UBuildsManager::DestroyBridge(ABridge_Core* Bridge, bool RecalculateResources)
{
    if (!Bridge) return false;


    for (auto& el : buildingsPool)
    {
        el.Value.Remove(Bridge);
    }

    Bridge->Destroy();
    if (RecalculateResources) RecalculateAllResources();
    return true;
}


//-------------------------------------------------

float UBuildsManager::CalculateBridgeSize(FVector StartLocation, FVector EndLocation, float StartOffset, float EndOffset) const
{
    float length = (StartLocation - EndLocation).Size();

    return FMath::Clamp(length - (StartOffset + EndOffset), minDistanceForBridges, maxDistanceForBridges) * 0.01f;
}


float UBuildsManager::CalculateBridgeSize(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    if (!FirstBuilding || !SecondBuilding) return minDistanceForBridges * 0.01f;

    float buildingRadiusFirst = GetBuildingRadius(FirstBuilding);
    float buildingRadiusSecond = GetBuildingRadius(SecondBuilding);

    return CalculateBridgeSize(FirstBuilding->GetActorLocation(), SecondBuilding->GetActorLocation(), buildingRadiusFirst, buildingRadiusSecond);

}


FTransform UBuildsManager::CalculateBridgeTransform(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    if (!FirstBuilding || !SecondBuilding) return FTransform();

    FVector start = FirstBuilding->GetActorLocation();
    FVector end = SecondBuilding->GetActorLocation();
    FVector mid = (start + end) * 0.5f;

    float scaleX = CalculateBridgeSize(FirstBuilding, SecondBuilding);
    FVector scale = FVector(scaleX, 1, 1);

    FRotator rot = (end - start).Rotation();

    return FTransform(FRotator(0.f, rot.Yaw, 0.f), mid, scale);
}

FTransform UBuildsManager::CalculateBridgeTransform(FVector StartLocation, FVector EndLocation, float StartOffset, float EndOffset) const
{
    FVector mid = (StartLocation + EndLocation) * 0.5f;
    float scaleX = CalculateBridgeSize(StartLocation, EndLocation, StartOffset, EndOffset);
    FVector scale = FVector(scaleX, 1, 1);

    FRotator rot = (StartLocation - EndLocation).Rotation();

    return FTransform(FRotator(0.f, rot.Yaw, 0.f), mid, scale);
}

bool UBuildsManager::CanCreateNewBuilding(FName BuildingTableID) const
{
    //TODO CanCreateNewBuilding

    return true;
}

bool UBuildsManager::CanCreateNewBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding)
{
    //можно добавть еше свои проверки
    return !HasBridgeBetweenBuildings(FirstBuilding, SecondBuilding) && !HasObstacleBetweenBuildings(FirstBuilding, SecondBuilding) && IsValidDistanceForBridge(FirstBuilding, SecondBuilding);
}

bool UBuildsManager::IsValidPlace(FVector Location, float CheckRadius) const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    QueryParams.bReturnPhysicalMaterial = false;

    TArray<FHitResult> HitResults;

    FCollisionObjectQueryParams ObjectQueryParams;

    // Building
    ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1); 
    // Bridge
    ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2); 

    return !World->SweepMultiByObjectType(HitResults,Location, Location, FQuat::Identity,ObjectQueryParams, FCollisionShape::MakeSphere(CheckRadius), QueryParams);
}

bool UBuildsManager::IsValidDistanceForBridge(FVector StartLocation, FVector EndLocation, float DistanceOffset) const
{
    float length = (StartLocation - EndLocation).Size() - DistanceOffset;
    length = FMath::Clamp(length, 0, 100000.f);

    return IsValidDistanceForBridge(length);
}

bool UBuildsManager::IsValidDistanceForBridge(float Distance) const
{
    return Distance <= maxDistanceForBridges && Distance >= minDistanceForBridges;
}

bool UBuildsManager::IsValidDistanceForBridge(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    if(!FirstBuilding || !SecondBuilding) return false;

    float offsetFirst = FirstBuilding->GetMeshRadius();
    float offsetSeconsd = SecondBuilding->GetMeshRadius();

    float distance = (FirstBuilding->GetActorLocation() - SecondBuilding->GetActorLocation()).Size() - (offsetFirst + offsetSeconsd);

    FMath::Clamp(distance, 0, 100000.f);

    return IsValidDistanceForBridge(distance);
}

bool UBuildsManager::HasObstacleBetweenBuildings(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    if (!FirstBuilding || !SecondBuilding || !GetWorld()) return true;

    UWorld* world = GetWorld();


    FVector fakeBridgeHalfSize = GetBridgeSize();

    FVector offset = FVector(0, 0, fakeBridgeHalfSize.Z + 0.1f);
    FVector start = FirstBuilding->GetActorLocation() + offset;
    FVector end = SecondBuilding->GetActorLocation() + offset;

    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(FirstBuilding);
    queryParams.AddIgnoredActor(SecondBuilding);

    FCollisionObjectQueryParams objectQueryParams;
    //Build
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
    //Bridge
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
    //Ground
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);

    FHitResult hitResult;

    return world->SweepSingleByObjectType(hitResult, start, end, FQuat::Identity, objectQueryParams, FCollisionShape::MakeBox(fakeBridgeHalfSize), queryParams);
}

bool UBuildsManager::HasObstacleBetweenLocations(FVector StartLocation, FVector EndLocation, TArray<AActor*> IgnoreList) const
{
    if(!GetWorld()) return true;


    FVector fakeBridgeHalfSize = GetBridgeSize();
    FVector offset = FVector(0, 0, fakeBridgeHalfSize.Z + 0.1f);

    FCollisionQueryParams queryParams;
    if(!IgnoreList.IsEmpty())
    {     
       queryParams.AddIgnoredActors(IgnoreList);        
    }


    FCollisionObjectQueryParams objectQueryParams;
    //Build
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
    //Bridge
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
    //Ground
    objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);

    FHitResult hitResult;

    return GetWorld()->SweepSingleByObjectType(hitResult, StartLocation + offset, EndLocation + offset, FQuat::Identity, objectQueryParams, FCollisionShape::MakeBox(fakeBridgeHalfSize), queryParams);
}

bool UBuildsManager::HasBridgeBetweenBuildings(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    if (!FirstBuilding || !SecondBuilding || FirstBuilding == SecondBuilding) return true;

    if (buildingsPool.Contains(FirstBuilding))
    {
        for (ABridge_Core* bridge : buildingsPool[FirstBuilding])
        {
            if (buildingsPool.Contains(SecondBuilding) && buildingsPool[SecondBuilding].Contains(bridge)) return true;
        }
    }
    return false;
}

FBuildDataTableRow UBuildsManager::GetBuildingData(FName DataTableID)
{
    if(!buildsDataTable || DataTableID.IsNone()) return FBuildDataTableRow();

    FBuildDataTableRow* row = buildsDataTable->FindRow<FBuildDataTableRow>(DataTableID, TEXT(""));
    if (row) return *row;

    return FBuildDataTableRow();
}

FVector UBuildsManager::GetBridgeSize() const
{
    //1м в ширину и 10 см в высоту (такой меш моста)
    return FVector(0.5f, 50, 5);
}

FName UBuildsManager::DefineBridgeDataTableID(ABuild_Core* FirstBuilding, ABuild_Core* SecondBuilding) const
{
    //TODO DefineBridgeDataTableID

    return "test";
}

float UBuildsManager::GetBuildingRadius(ABuild_Core* Building) const
{
    if (Building) return Building->GetMeshRadius();

    // радиус здания/меша 200 юнитов
    return 200.0f;
}

void UBuildsManager::RecalculateAllResources()
{
    if (!buildsDataTable) return;

    TMap<EBuildResource, int32> resourceMap;
    TSet<ABridge_Core*> bridges;
    TArray<FBuildResourceData> allResources;

    for (const auto& el : buildingsPool)
    {
        ABuild_Core* building = el.Key;
        if (!building) continue;

        FName dataTableID = building->GetDataTableID();
        if(dataTableID.IsNone()) continue;

        FBuildDataTableRow* buildingData = buildsDataTable->FindRow<FBuildDataTableRow>(dataTableID, "");

        if (!buildingData) continue;

        //плюсуем
        for (const FBuildResourceData& resource : buildingData->GeneratedResources)
        {
            resourceMap.FindOrAdd(resource.Resource) += resource.Value;
        }
        //вычитаем
        for (const FBuildResourceData& resource : buildingData->ConsumedResources)
        {
            resourceMap.FindOrAdd(resource.Resource) -= resource.Value;
        }
        //собираем все мосты
        for (ABridge_Core* bridge : el.Value)
        {
            if (bridge)
            {
                bridges.Add(bridge);
            }
        }
    }

    resourceMap.FindOrAdd(EBuildResource::Energy) -= bridges.Num();

    //убрать все ресурсы, равные 0
    bool clearZero = true;


    for (const auto& el : resourceMap)
    {
        if(clearZero)
        {
            if (el.Value != 0)
            {
                allResources.Add({ el.Key, el.Value });
            }
        }
        else
        {
            allResources.Add({ el.Key, el.Value });
        }
       
    }

    calculatedResources = allResources;

    if (OnResourcesUpdated.IsBound())  OnResourcesUpdated.Broadcast(calculatedResources);
}

   


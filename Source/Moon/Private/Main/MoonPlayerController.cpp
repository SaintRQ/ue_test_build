// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/MoonPlayerController.h"

#include "MoonFunctionLibrary.h"
#include "Enums/GameModes.h"
#include "SubSystems/BuildsManager.h"
#include "SubSystems/HUDManager.h"
#include "Game/Builds/Build_Core.h"
#include "Game/Bridges/Bridge_Core.h"


AMoonPlayerController::AMoonPlayerController(const FObjectInitializer& ObjectInitializer) 
    : Super (ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    currentMode = EGameModes::None;
    buildsManager = UMoonFunctionLibrary::GetBuildsManager(this);
    hudManager = UMoonFunctionLibrary::GetHUDManager(this);
    
}

void AMoonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

    if(!InputComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MoonPlayerController: InputComponent not valid."));
        return;
    }

    InputComponent->BindAction("LeftMouseAction", IE_Pressed, this, &AMoonPlayerController::OnLeftMouseButtonPressed);
    InputComponent->BindAction("RightMouseAction", IE_Pressed, this, &AMoonPlayerController::OnRightMouseButtonPressed);
    InputComponent->BindAction("QuitMode", IE_Pressed, this, &AMoonPlayerController::OnQuitModePressed);
    InputComponent->BindAction("BridgeMode", IE_Pressed, this, &AMoonPlayerController::OnBridgeModePressed);
    InputComponent->BindAction("DestructionMode", IE_Pressed, this, &AMoonPlayerController::OnDestructionModePressed);
 
}

void AMoonPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (currentMode == EGameModes::None) return;

//----------------------------------------------------------------

    TraceMouseHit(currentMode);

    if (hudManager && currentMode == EGameModes::Build) hudManager->MoveBuildingVisualizer(lastMouseHitResult.ImpactPoint);
}

void AMoonPlayerController::OnLeftMouseButtonPressed()
{
    switch(currentMode)
    {
    case EGameModes::None:
        break;
    case EGameModes::Build:
        if(buildsManager && hudManager)
        {
            if(buildsManager->IsValidPlace(hudManager->GetLastBuildingVisualizerTransform().GetLocation(), hudManager->GetLastBuildingVisualizerRadius()))
            {
                if (buildsManager->CreateBuilding(hudManager->GetLastBuildingVisualizerID(), hudManager->GetLastBuildingVisualizerTransform(), true))
                {
                    SwitchGameMode(EGameModes::None);
                }
            }         
        }
        break;

    case EGameModes::Bridge:

        if (lastMouseHitResult.bBlockingHit)
        {
            if (hudManager)
            {
                hudManager->SelectTarget(lastMouseHitResult.GetActor(), currentMode);
            }
        }

        break;

    case EGameModes::Destruction:

        if(lastMouseHitResult.bBlockingHit)
        {
            if(hudManager)
            {
                hudManager->SelectTarget(lastMouseHitResult.GetActor(), currentMode);
            }
        }

        break;

    default: break;
    }
}

void AMoonPlayerController::OnRightMouseButtonPressed()
{
    switch (currentMode)
    {
    case EGameModes::None:
        break;

    case EGameModes::Build: 
        SwitchGameMode(EGameModes::None);

        break;

    case EGameModes::Bridge:

        if (hudManager)
        {
            hudManager->ClearAllSelectedTargets();
            hudManager->ClearAllVisualizers();
        }      
        break;

    case EGameModes::Destruction:
        if(hudManager)
        {
            hudManager->ClearAllSelectedTargets();
            hudManager->ClearAllVisualizers();
        }

        break;

    default: break;
    }
}

void AMoonPlayerController::OnQuitModePressed()
{
    SwitchGameMode(EGameModes::None);
}

void AMoonPlayerController::OnBridgeModePressed()
{
    SwitchGameMode(EGameModes::Bridge);
}

void AMoonPlayerController::OnDestructionModePressed()
{
    SwitchGameMode(EGameModes::Destruction);
}

void AMoonPlayerController::TraceMouseHit(EGameModes Mode)
{
    FVector WorldLocation, WorldDirection;
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);

        FHitResult HitResult;
        FCollisionQueryParams TraceParams("trace", true, this);
        TraceParams.bReturnPhysicalMaterial = false;
        TraceParams.AddIgnoredActor(GetPawn());

        FCollisionObjectQueryParams ObjectQueryParams;


        switch(Mode)
        {
        case EGameModes::Build:
            // Ground
            ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
            break;

        case EGameModes::Bridge:
            // Building
            ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
            break;

        case EGameModes::Destruction:
            // Building
            ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
            // Bridge
            ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
            break;
        default: break;
        }

        GetWorld()->LineTraceSingleByObjectType(HitResult, WorldLocation, TraceEnd, ObjectQueryParams, TraceParams);

        if (HitResult.bBlockingHit)
        {
            lastMouseHitResult = HitResult;
        }
        else if(Mode == EGameModes::Bridge || Mode == EGameModes::Destruction) 
        {
            lastMouseHitResult = HitResult;
        }
    }
}

void AMoonPlayerController::SwitchGameMode(EGameModes NewMode)
{
    if (NewMode == currentMode) return;

    currentMode = NewMode; 

    if (OnGameModeChanged.IsBound()) OnGameModeChanged.Broadcast(currentMode);

}

bool AMoonPlayerController::TryStartBuildMode(FName BuildTableID)
{
    if (!buildsManager || !hudManager || !buildsManager->CanCreateNewBuilding(BuildTableID)) return false;

    SwitchGameMode(EGameModes::Build);

    hudManager->CreateBuildingVisualizer(BuildTableID);

	return true;
}

void AMoonPlayerController::OnButtonBridgeConnectClicked(TArray<AActor*> InteractTarget)
{
   if(!hudManager || !buildsManager) 
   {
       UE_LOG(LogTemp, Error, TEXT("MoonPlayerController: OnButtonBridgeConnectClicked is failed."));
       return;
   }
   if(!InteractTarget.IsEmpty() && InteractTarget.Num() == 2)
   {
       TArray<ABuild_Core*> buildings;
       for (AActor* actor : InteractTarget)
       {
           if (actor) buildings.Add(Cast<ABuild_Core>(actor));
       }

       buildsManager->CreateBridge(buildings[0], buildings[1]);
   }

   hudManager->ClearAllVisualizers();
   hudManager->ClearAllSelectedTargets();

}

void AMoonPlayerController::OnButtonDestroyClicked(TArray<AActor*> InteractTarget)
{
    if (InteractTarget.IsEmpty() || !buildsManager || !hudManager) return;

    for(auto el : InteractTarget)
    {
        if (!el) continue;

        ABuild_Core* build = Cast<ABuild_Core>(el);
        if (build) buildsManager->DestroyBuilding(build);
        else
        {
            ABridge_Core* bridge = Cast<ABridge_Core>(el);
            if (bridge) buildsManager->DestroyBridge(bridge);        
        }
    }

    hudManager->ClearAllVisualizers();
    hudManager->ClearAllSelectedTargets();
}

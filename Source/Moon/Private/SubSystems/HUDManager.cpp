// Fill out your copyright notice in the Description page of Project Settings.

#include "SubSystems/HUDManager.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Enums/GameModes.h"
#include "Game/Widgets/GameHudWidget.h"
#include "MoonFunctionLibrary.h"
#include "Main/MoonPlayerController.h"
#include "SubSystems/BuildsManager.h"
#include "Game/Visualizers/BuildVisualizer.h"
#include "Game/Visualizers/BridgeVisualizer.h"
#include "Game/Visualizers/InteractVisualizer.h"
#include "Game/Builds/Build_Core.h"
#include "ProjectSettings/MoonProjectSettings.h"

void UHUDManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	mainHudWidgetAssetID = FName("GameHudAssetID");

	mainHudWidgetClass = UMoonFunctionLibrary::GetWidgetByID(mainHudWidgetAssetID);
	if (!mainHudWidgetClass) 
	{
		UE_LOG(LogTemp, Error, TEXT("HUDManager: MainHudWidgetClass not found."))
	} 

	auto settings = UMoonFunctionLibrary::GetGameProjectSettings();
	if (settings)
	{
		bridgeVisualizerLineSize = FVector2D(settings->visualizerLineWidth, settings->visualizerLineHeight);
	}

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UHUDManager::WaitFullIntialization);
}

void UHUDManager::Deinitialize()
{
	Super::Deinitialize();
}

//-------------------------------------------------------------------

void UHUDManager::WaitFullIntialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime)
	{
		static float time = 0;
		time += DeltaTime;
		if (time >= 0.01f)
		{
			if(GetWorld() && GetWorld()->GetFirstPlayerController())
			{
				playerController = UMoonFunctionLibrary::GetCurrentPlayerController(this);
				buildsManager = UMoonFunctionLibrary::GetBuildsManager(this);

				if(playerController) playerController->OnGameModeChanged.AddUObject(this, &UHUDManager::OnGameModeChanged);
				
				InitializeHUD();
				return false;
			}
		}
		return true; 
	}));
}

void UHUDManager::InitializeHUD()
{
	if (!GetWorld() || !playerController)
	{
		UE_LOG(LogTemp, Error, TEXT("HUDManager: Initialization failed. Invalid world or player."));
		return;
	}

	CreateHudWidget();	

	if (!mainHudWidget) 
	{
		UE_LOG(LogTemp, Error, TEXT("HUDManager: MainHudWidget not valid"));
		return;
	} 

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(mainHudWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);

	playerController->SetInputMode(InputMode);
	playerController->bShowMouseCursor = true;
}

void UHUDManager::CreateHudWidget()
{
	if (!playerController) return;

	if (mainHudWidget)
	{
		if (!mainHudWidget->IsInViewport())
		{
			mainHudWidget->AddToViewport(0);
			mainHudWidget->SetIsFocusable(true);
		}
		else return;
	}
	else if (mainHudWidgetClass)
	{
		UUserWidget* createdWidget = CreateWidget<UUserWidget>(playerController, mainHudWidgetClass, TEXT("GameHUDWidget"));
		if (createdWidget)
		{
			createdWidget->AddToViewport(0);
			createdWidget->SetIsFocusable(true);
			mainHudWidget = Cast<UGameHudWidget>(createdWidget);
		}
	}
}

void UHUDManager::OnGameModeChanged(EGameModes CurrentMode)
{
	ClearAllVisualizers();
	ClearAllSelectedTargets();
}

void UHUDManager::CreateBuildingVisualizer(FName BuildingTableRowID)
{
	if (!buildsManager || !GetWorld()) return;

	lastBuildingVisualizerTableID = BuildingTableRowID;

	if (lastBuildingVisualizer) lastBuildingVisualizer->Destroy();
	
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	lastBuildingVisualizer = GetWorld()->SpawnActor<ABuildVisualizer>(spawnParams);
	
	if (lastBuildingVisualizer)
	{
		lastBuildingVisualizer->InitializeVisualizer(buildsManager->GetBuildingData(BuildingTableRowID));
	}
}

void UHUDManager::MoveBuildingVisualizer(FVector MoveLocation)
{
	if (!lastBuildingVisualizer || !buildsManager || !GetWorld()) return;

	lastBuildingVisualizer->SetActorLocation(MoveLocation);

	bool isValidPlace = buildsManager->IsValidPlace(MoveLocation, lastBuildingVisualizer->GetMeshRadius());

	lastBuildingVisualizer->SetVisualizerColor(isValidPlace ? FLinearColor::Green * 2 : FLinearColor::Red * 2);

//--------------------------------------------------------- Bridges

    FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(lastBuildingVisualizer);

	FCollisionObjectQueryParams objectQueryParams;
	//Build
	objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	
	TArray<FHitResult> hitResults;
	TSet<ABuild_Core*> buildingsHits;
	TSet<ABuild_Core*> buildings;

	bool clearAll = true;

	if(GetWorld()->SweepMultiByObjectType(hitResults, MoveLocation, MoveLocation, FQuat::Identity, objectQueryParams, FCollisionShape::MakeSphere(buildsManager->GetMinMaxBridgeDistance().Y + lastBuildingVisualizer->GetMeshRadius()), queryParams))
	{
		for(auto &el : hitResults)
		{
			if (el.GetActor()) buildingsHits.Add(Cast<ABuild_Core>(el.GetActor()));
		}

		if (!buildingsHits.IsEmpty()) 
		{		
			for(auto el: buildingsHits)
			{
				if (!el || buildsManager->HasObstacleBetweenLocations(MoveLocation, el->GetActorLocation(), TArray<AActor*> {el})) continue;

				buildings.Add(el);
			}

			if(!buildings.IsEmpty()) 
			{
				clearAll = false;

				//-------------------------------------------------------

				// выравнием кол-во зданий и визулизаторв
				int32 visNum = lastBridgeVisualizers.Num();
				int32 buildsNum = buildings.Num();
				int32 it = 0;

				if (buildsNum > visNum)
				{
					it = buildsNum - visNum;

					for (int i = 0; i < it; i++)
					{
						CreateBridgeVisualizer();
					}
				}
				else if (buildsNum < visNum)
				{
					it = visNum - buildsNum;

					for (int i = 0; i < it; i++)
					{
						RemoveBridgeVisualizer();
					}

				}

				//------------------------------------------------------

				int32 visIndex = 0;

				for (auto& el : buildings)
				{
					if (el)
					{
						FTransform trans = buildsManager->CalculateBridgeTransform(MoveLocation, el->GetActorLocation(), el->GetMeshRadius(), lastBuildingVisualizer->GetMeshRadius());

						FVector scale = trans.GetScale3D();
						scale.Y = bridgeVisualizerLineSize.X;
						scale.Z = bridgeVisualizerLineSize.Y;
						trans.SetScale3D(scale);

						float offset = el->GetMeshRadius() + lastBuildingVisualizer->GetMeshRadius();

						FLinearColor color = buildsManager->IsValidDistanceForBridge(MoveLocation, el->GetActorLocation(), offset) ? FLinearColor::Green * 2.f : FLinearColor::Red * 2.f;

						UpdateBridgeVisualizer(visIndex, trans, color);
					}

					visIndex++;
				}
			}

		}
	}
	
	if (clearAll) 
	{
		ClearAllBridgeVisualizer();
	}

}

FTransform UHUDManager::GetLastBuildingVisualizerTransform() const
{
	if (lastBuildingVisualizer) return lastBuildingVisualizer->GetActorTransform();

	return FTransform();
}

float UHUDManager::GetLastBuildingVisualizerRadius()
{
	if (lastBuildingVisualizer) return lastBuildingVisualizer->GetMeshRadius();
	return 200.f;
}

void UHUDManager::CreateInteractVisualizer(AActor* Target, EGameModes VisualizerType)
{
	if (!Target) return;

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;

	AInteractVisualizer* vis = GetWorld()->SpawnActor<AInteractVisualizer>(spawnParams);
	if(vis)
	{
		vis->SetActorLocation(Target->GetActorLocation());

		switch(VisualizerType)
		{
		case EGameModes::Build:
			vis->InitializeVisualizer(0);
			vis->SetVisualizerTargets(TArray<AActor*>{Target});
			break;

		case EGameModes::Bridge:
			vis->InitializeVisualizer(2);
			vis->SetVisualizerTargets(TArray<AActor*>{Target});
			break;

		case EGameModes::Destruction:
			vis->InitializeVisualizer(3);
			vis->SetVisualizerTargets(TArray<AActor*>{Target});
			break;

		default: 
			vis->InitializeVisualizer(1);
			vis->SetVisualizerTargets(TArray<AActor*>{Target});
			break;
		}

		lastInteractVisualizers.Add(vis);

	}

}

void UHUDManager::CreateInteractVisualizer(FVector VisualizerLocation, EGameModes VisualizerType)
{
	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	auto vis = GetWorld()->SpawnActor<AInteractVisualizer>();
	if (vis)
	{
		vis->SetActorLocation(VisualizerLocation);

		switch (VisualizerType)
		{
		case EGameModes::Build:
			vis->InitializeVisualizer(0);
			break;

		case EGameModes::Bridge:
			vis->InitializeVisualizer(2);
			break;

		case EGameModes::Destruction:
			vis->InitializeVisualizer(3);
			break;

		default:
			vis->InitializeVisualizer(1);
			break;
		}
		lastInteractVisualizers.Add(vis);

	}
}

void UHUDManager::CreateBridgeVisualizer()
{
	if(!GetWorld()) return;

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	ABridgeVisualizer* newVisualizer = GetWorld()->SpawnActor<ABridgeVisualizer>(spawnParams);
	if(newVisualizer)
	{
		newVisualizer->InitializeVisualizer();
		lastBridgeVisualizers.Add(newVisualizer);
	}

}

void UHUDManager::UpdateBridgeVisualizer(int32 VisualizerIndex, FTransform Transform, FLinearColor Color) const
{
	if (!lastBridgeVisualizers.IsValidIndex(VisualizerIndex) || !lastBridgeVisualizers[VisualizerIndex]) return;

	lastBridgeVisualizers[VisualizerIndex]->SetActorTransform(Transform);
	lastBridgeVisualizers[VisualizerIndex]->SetVisualizerColor(Color);
}

void UHUDManager::RemoveBridgeVisualizer()
{
	if (lastBridgeVisualizers.IsEmpty()) return;

	if (lastBridgeVisualizers[0]) 
	{
		lastBridgeVisualizers[0]->Destroy();
	}
	lastBridgeVisualizers.RemoveAt(0);
}

void UHUDManager::ClearAllBridgeVisualizer()
{
	if (!lastBridgeVisualizers.IsEmpty())
	{
		for (auto &el : lastBridgeVisualizers)
		{
			if (el) el->Destroy();
		}

		lastBridgeVisualizers.Empty();
	}
}

void UHUDManager::ClearAllVisualizers()
{
	if(lastBuildingVisualizer) 
	{
		lastBuildingVisualizer->Destroy();
		lastBuildingVisualizer = nullptr;
	}

	if(!lastInteractVisualizers.IsEmpty())
	{
		for(auto &el : lastInteractVisualizers)
		{
			if (el) el->Destroy();
		}

		lastInteractVisualizers.Empty();
	}

	ClearAllBridgeVisualizer();

}

bool UHUDManager::SelectTarget(AActor* Target, EGameModes CurrentMode)
{
	int maxSelectedTargets = (CurrentMode == EGameModes::Bridge) ? 2 : 1;

	if (!Target || !buildsManager) return false;

	ClearAllVisualizers();

	bool result = false;

	if (!selectedActors.Contains(Target))
	{
		if (selectedActors.Num() >= maxSelectedTargets)
		{
			selectedActors.Add(Target);
			selectedActors.RemoveAt(0);
			result = true;
		}
		else
		{
			selectedActors.Add(Target);
			result = true;
		}
	}
	

	if (CurrentMode == EGameModes::Bridge)
	{
		EGameModes visualizerType = EGameModes::None;

		if (selectedActors.Num() == 2)
		{
			TArray<ABuild_Core*> buildings;
			for (AActor* actor : selectedActors)
			{
				if (actor) buildings.Add(Cast<ABuild_Core>(actor));	
			}

			bool canCreateBridge = (buildings.Num() == 2) && buildsManager->CanCreateNewBridge(buildings[0], buildings[1]);

			visualizerType = canCreateBridge ? EGameModes::Build : EGameModes::None;

			for (AActor* actor : selectedActors)
			{
				if (actor) CreateInteractVisualizer(actor, visualizerType);
			}

			if (canCreateBridge)
			{
				FTransform visualizerPosition = buildsManager->CalculateBridgeTransform(buildings[0], buildings[1]);
				CreateInteractVisualizer(visualizerPosition.GetLocation(), EGameModes::Bridge);

				if (lastInteractVisualizers.Num() == 3 && lastInteractVisualizers[2])
				{
					lastInteractVisualizers[2]->SetVisualizerTargets(selectedActors);
				}
			}
		}
		else
		{
			for (AActor* actor : selectedActors)
			{
				if (actor) CreateInteractVisualizer(actor, visualizerType);
			}
		}
	}
	else if (!selectedActors.IsEmpty())
	{
		for (AActor* actor : selectedActors)
		{
			if (actor) CreateInteractVisualizer(actor, CurrentMode);
		}
	}

	return result;
}


void UHUDManager::ClearAllSelectedTargets()
{
	selectedActors.Empty();
}



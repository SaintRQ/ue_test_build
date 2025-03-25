// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HUDManager.generated.h"

/**
 * 
 */

enum class EGameModes : uint8;

class UBuildsManager;
class UGameHudWidget;
class AMoonPlayerController;
class ABuildVisualizer;
class ABridgeVisualizer;
class AInteractVisualizer;


UCLASS()
class MOON_API UHUDManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FVector2D bridgeVisualizerLineSize = FVector2D (1, 1);

//--------------------------------------------------------------------------

	UBuildsManager* buildsManager;

	FName mainHudWidgetAssetID;
	TSubclassOf<UGameHudWidget> mainHudWidgetClass;
	UGameHudWidget* mainHudWidget;
	AMoonPlayerController* playerController;

	ABuildVisualizer* lastBuildingVisualizer;
	TArray<ABridgeVisualizer*> lastBridgeVisualizers;
	TArray<AInteractVisualizer*> lastInteractVisualizers;

	TArray<AActor*> selectedActors;

	FName lastBuildingVisualizerTableID;


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void WaitFullIntialization(UWorld* World, const UWorld::InitializationValues IVS);

	void InitializeHUD();

	void CreateHudWidget();

	void OnGameModeChanged(EGameModes CurrentMode);

public:

	UGameHudWidget* GetMainHudWidget() const { return mainHudWidget; }

	bool SelectTarget(AActor* Target, EGameModes CurrentMode);
	void ClearAllSelectedTargets();


//----------------------------------------- Building Visualizer

	void CreateBuildingVisualizer(FName BuildingTableRowID);
	void MoveBuildingVisualizer(FVector MoveLocation);

	FName GetLastBuildingVisualizerID() const { return lastBuildingVisualizerTableID; }
	FTransform GetLastBuildingVisualizerTransform() const;
	float GetLastBuildingVisualizerRadius();

//-----------------------------------------  Interact Visualizer

	// Создать визулизатор взаимодействий. None - красная точка, Build - зеленая точка, Bridge - коннетор для моста, Destruction - демонтаж.
	void CreateInteractVisualizer(AActor* Target, EGameModes VisualizerType);
	void CreateInteractVisualizer(FVector VisualizerLocation, EGameModes VisualizerType);

//----------------------------------------- Bridge Visualizer

private:

	void CreateBridgeVisualizer();
	void UpdateBridgeVisualizer(int32 VisualizerIndex, FTransform Transform, FLinearColor Color) const;
	void RemoveBridgeVisualizer();

	void ClearAllBridgeVisualizer();

//-----------------------------------------

public:

	void ClearAllVisualizers();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MoonPlayerController.generated.h"

/**
 * 
 */

enum class EGameModes : uint8;
class UBuildsManager;
class UHUDManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FEventOnGameModeChanged, EGameModes);


UCLASS()
class MOON_API AMoonPlayerController : public APlayerController
{
	GENERATED_BODY()

	EGameModes currentMode;
	UBuildsManager* buildsManager;
	UHUDManager* hudManager;

	FHitResult lastMouseHitResult;

	AMoonPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void SetupInputComponent() override;

	void OnLeftMouseButtonPressed();
	void OnRightMouseButtonPressed();

	void OnQuitModePressed();
	void OnBridgeModePressed();
	void OnDestructionModePressed();

	void TraceMouseHit(EGameModes Mode);

	void SwitchGameMode(EGameModes NewMode);

public:

	virtual void Tick(float DeltaTime) override;

	bool TryStartBuildMode(FName BuildTableID);

	void OnButtonBridgeConnectClicked(TArray<AActor*> InteractTarget);

	void OnButtonDestroyClicked(TArray<AActor*> InteractTarget);


//----------------------------------------------------- Events;

	// Событие, которое вызывается при смене игрового режима. Возвращает EGameModes
	FEventOnGameModeChanged OnGameModeChanged;

};

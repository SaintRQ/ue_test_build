// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractVisualizerWidget.generated.h"

/**
 * 
 */
class UWidgetSwitcher;
class UButton;
class AMoonPlayerController;

UCLASS()
class MOON_API UInteractVisualizerWidget : public UUserWidget
{
	GENERATED_BODY()

	AMoonPlayerController* playerController;
	TArray<AActor*> widgetTargets;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_BridgeConnect;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Destroy;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnButtonClicked_BridgeConnect();
	UFUNCTION()
	void OnButtonClicked_Destroy();

	void ClearButtonBinds();


public:
	
	//0 - Select success, 1- Select fail, 2 - Bridge Connect, 3 - Destroy
	void SwitchVisualizator(int32 ID);


	void SetWidgetTargets(TArray<AActor*> WidgetTarget);
};

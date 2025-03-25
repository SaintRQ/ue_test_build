// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Widgets/Visualizer/InteractVisualizerWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Main/MoonPlayerController.h"
#include "MoonFunctionLibrary.h"


void UInteractVisualizerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	playerController = UMoonFunctionLibrary::GetCurrentPlayerController(this);
}

void UInteractVisualizerWidget::NativeDestruct()
{
	ClearButtonBinds();
	Super::NativeDestruct();
}

void UInteractVisualizerWidget::OnButtonClicked_BridgeConnect()
{
	if (playerController) playerController->OnButtonBridgeConnectClicked(widgetTargets);
}

void UInteractVisualizerWidget::OnButtonClicked_Destroy()
{
	if (playerController) playerController->OnButtonDestroyClicked(widgetTargets);
}

void UInteractVisualizerWidget::ClearButtonBinds()
{
	if (Button_BridgeConnect && Button_BridgeConnect->OnClicked.IsBound()) Button_BridgeConnect->OnClicked.RemoveAll(this);
	if (Button_Destroy && Button_Destroy->OnClicked.IsBound()) Button_Destroy->OnClicked.RemoveAll(this);
}

void UInteractVisualizerWidget::SwitchVisualizator(int32 ID)
{
	if (!Switcher) return;

	int32 index = FMath::Clamp(ID, 0, 3);
	Switcher->SetActiveWidgetIndex(index);
	ClearButtonBinds();

	if(Button_BridgeConnect && index == 2) Button_BridgeConnect->OnClicked.AddDynamic(this, &UInteractVisualizerWidget::OnButtonClicked_BridgeConnect);
	else if(Button_Destroy && index == 3) Button_Destroy->OnClicked.AddDynamic(this, &UInteractVisualizerWidget::OnButtonClicked_Destroy);
}

void UInteractVisualizerWidget::SetWidgetTargets(TArray<AActor*> WidgetTargets)
{
	widgetTargets = WidgetTargets;
}

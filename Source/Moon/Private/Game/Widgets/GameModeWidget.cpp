// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Widgets/GameModeWidget.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "MoonFunctionLibrary.h"
#include "Main/MoonPlayerController.h"
#include "Enums/GameModes.h"

void UGameModeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AMoonPlayerController* controller = UMoonFunctionLibrary::GetCurrentPlayerController(this);

	if(controller) controller->OnGameModeChanged.AddUObject(this, &UGameModeWidget::OnGameModeChanged);

}

void UGameModeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	AMoonPlayerController* controller = UMoonFunctionLibrary::GetCurrentPlayerController(this);

	if (controller) controller->OnGameModeChanged.RemoveAll(this);
	
}

void UGameModeWidget::OnGameModeChanged(EGameModes NewGameMode)
{
	if (!Mode_Border || !Mode_Text) return;

	FLinearColor color = FLinearColor::Black;
	bool borderIsVisible = true;

	FText finText = FText::GetEmpty();

	FText textBuild = NSLOCTEXT("UI", "ui_game_mode_build_text", "РЕЖИМ СТРОИТЕЛЬСТВА");
	FText textBridge = NSLOCTEXT("UI", "ui_game_mode_bridge_text", "РЕЖИМ МОСТОВ");
	FText textDestr = NSLOCTEXT("UI", "ui_game_mode_destr_text", "РЕЖИМ ДЕМОНТАЖА");

	switch(NewGameMode)
	{
	case EGameModes::None:
		borderIsVisible = false;
		break;
	case EGameModes::Build:
		color = BuildModeColor;
		finText = textBuild;
		break;
	case EGameModes::Bridge:
		color = BridgeModeColor;
		finText = textBridge;
		break;
	case EGameModes::Destruction:
		color = DestructionModeColor;
		finText = textDestr;
		break;

	default: break;
	}

	Mode_Text->SetColorAndOpacity(color);
	Mode_Text->SetText(finText);
	Mode_Border->SetVisibility(borderIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	
}

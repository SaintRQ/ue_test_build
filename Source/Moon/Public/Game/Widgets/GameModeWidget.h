// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModeWidget.generated.h"

/**
 * 
 */

class UBorder;
class UTextBlock;
enum class EGameModes : uint8;

UCLASS()
class MOON_API UGameModeWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void OnGameModeChanged(EGameModes NewGameMode);



	UPROPERTY(meta = (BindWidget))
	UBorder* Mode_Border;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Mode_Text;

protected:
	
	UPROPERTY(BlueprintReadWrite)
	FLinearColor BuildModeColor;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor BridgeModeColor;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor DestructionModeColor;

};

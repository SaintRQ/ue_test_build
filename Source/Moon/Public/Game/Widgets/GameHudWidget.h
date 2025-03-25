// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHudWidget.generated.h"

/**
 * 
 */

UCLASS()
class MOON_API UGameHudWidget : public UUserWidget
{
	GENERATED_BODY()


	virtual void NativeConstruct() override;
	
};

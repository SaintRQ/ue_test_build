// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildResourcesGridSlotWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;


UCLASS()
class MOON_API UBuildResourcesGridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Res;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Res;

public:

	void SetContent(UTexture* Texture, int32 Value);
	
};

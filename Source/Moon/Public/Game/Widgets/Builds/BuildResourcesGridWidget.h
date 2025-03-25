// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/Build/BuildResourceData.h"
#include "BuildResourcesGridWidget.generated.h"

/**
 * 
 */
class UWrapBox;
class UBuildResourcesGridSlotWidget;
enum class EBuildResource : uint8;

UCLASS()
class MOON_API UBuildResourcesGridWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:

	FName resourcesGridSlotAssetID;

protected:


	TSubclassOf<UBuildResourcesGridSlotWidget> gridSlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* Resources_Grid;

	//0 - Энергия, 1 - Вода, 2 - Воздух, 3 - Еда
	UPROPERTY(BlueprintReadWrite)
	TArray<UTexture2D*> ResourcesTexures;

	UTexture2D* GetResourceTexture(EBuildResource ResourceType);

//---------------------------------------------------------------- Events

	void OnResourcesUpdated(TArray<FBuildResourceData> Resources);

	

	
};

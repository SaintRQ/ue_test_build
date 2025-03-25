// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildGridWidget.generated.h"

/**
 * 
 */
class AMoonPlayerController;
class UWrapBox;
class UBuildGridSlotWidget;

UCLASS()
class MOON_API UBuildGridWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	FName buildGridSlotAssetID;
	FName buildsDataTableAssetID;

protected:

	AMoonPlayerController* playerController;
	UDataTable* buildsDataTable;
	TSubclassOf<UBuildGridSlotWidget> buildGridSlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* BuildingsGrid;

public:

	void UpdateBuildingsGrid();

	void OnGridSlotClicked(FName DataTableID);
	
};

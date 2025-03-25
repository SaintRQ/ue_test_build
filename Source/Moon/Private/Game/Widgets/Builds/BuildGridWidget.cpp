// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Widgets/Builds/BuildGridWidget.h"
#include "Game/Widgets/Builds/BuildGridSlotWidget.h"
#include "MoonFunctionLibrary.h"
#include "Components/WrapBox.h"
#include "Structs/Build/BuildDataTableRow.h"
#include "Main/MoonPlayerController.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UBuildGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	buildGridSlotAssetID = "BuildGridSlotAssetID";
	buildsDataTableAssetID = "BuildsDataTable";

	//-------------------- Загрузка BuildsDataTable --------------------
	auto loadedBuildsDataTableObject = UMoonFunctionLibrary::GetAssetByID(buildsDataTableAssetID);
	if (loadedBuildsDataTableObject) buildsDataTable = Cast<UDataTable>(loadedBuildsDataTableObject);
	else UE_LOG(LogTemp, Error, TEXT("UBuildGridWidget: AssetBuildsDataTable not found."));


	buildGridSlotWidgetClass = UMoonFunctionLibrary::GetWidgetByID(buildGridSlotAssetID);

	playerController = UMoonFunctionLibrary::GetCurrentPlayerController(this);


	UpdateBuildingsGrid();
}

void UBuildGridWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBuildGridWidget::UpdateBuildingsGrid()
{
	if (!BuildingsGrid) return;

	BuildingsGrid->ClearChildren();

	if (!buildGridSlotWidgetClass || !buildsDataTable) return;

	TArray<FName> rows = buildsDataTable->GetRowNames();
	TArray<FSoftObjectPath> iconsForLoad;

	FStreamableManager& streamable = UAssetManager::GetStreamableManager();

	for(const auto& row : rows)
	{
		FBuildDataTableRow* rowData = buildsDataTable->FindRow<FBuildDataTableRow>(row, "");
		if(rowData && rowData->BuildIcon.ToSoftObjectPath().IsAsset())
		{
			iconsForLoad.Add(rowData->BuildIcon.ToSoftObjectPath());
		}
	}

	streamable.RequestAsyncLoad(iconsForLoad, [this]()
	{
		TArray<FName> rows = buildsDataTable->GetRowNames();
		for (const auto& row : rows)
		{
			FBuildDataTableRow* rowData = buildsDataTable->FindRow<FBuildDataTableRow>(row, "");
			if (rowData && rowData->BuildIcon.Get())
			{
				UBuildGridSlotWidget* widget = CreateWidget<UBuildGridSlotWidget>(GetWorld(), buildGridSlotWidgetClass, "");
				if(widget)
				{		
					UTexture2D* icon = Cast<UTexture2D>(rowData->BuildIcon.Get());
					widget->SetContent(this, row, icon);
					BuildingsGrid->AddChild(widget);
				}

			}
		}
			
	});


}

void UBuildGridWidget::OnGridSlotClicked(FName DataTableID)
{
	if (DataTableID.IsNone() || !playerController) return;

	playerController->TryStartBuildMode(DataTableID);
}

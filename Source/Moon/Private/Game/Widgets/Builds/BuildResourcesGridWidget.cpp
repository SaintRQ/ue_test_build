// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Widgets/Builds/BuildResourcesGridWidget.h"

#include "Enums/Build/BuildResource.h"
#include "Blueprint/UserWidget.h"
#include "Managers/BuildsManager.h"
#include "MoonFunctionLibrary.h"
#include "Components/WrapBox.h"

void UBuildResourcesGridWidget::NativeConstruct()
{
	Super::NativeConstruct();

	resourcesGridSlotAssetID = FName("ResourceGridSlotAssetID");

	// получение бп виджета слотов для сетки ресурсов
	gridSlotWidgetClass = UMoonFunctionLibrary::GetWidgetByID(resourcesGridSlotAssetID);

	UBuildsManager* buildsManager = UMoonFunctionLibrary::GetBuildsManager(this);

	if(buildsManager && !buildsManager->OnResourcesUpdated.IsBoundToObject(this))
	{
		buildsManager->OnResourcesUpdated.AddUObject(this, &UBuildResourcesGridWidget::OnResourcesUpdated);
	}


}

void UBuildResourcesGridWidget::NativeDestruct()
{
	UBuildsManager* buildsManager = UMoonFunctionLibrary::GetBuildsManager(this);

    if (buildsManager && buildsManager->OnResourcesUpdated.IsBoundToObject(this)) buildsManager->OnResourcesUpdated.RemoveAll(this);

	Super::NativeDestruct();
}

UTexture2D* UBuildResourcesGridWidget::GetResourceTexture(EBuildResource ResourceType)
{
	if (ResourcesTexures.Num() <= 0 || ResourcesTexures.Num() < 4) return nullptr;

	switch(ResourceType)
	{
	   case EBuildResource::Energy: return ResourcesTexures[0];
	   case EBuildResource::Water: return ResourcesTexures[1];
	   case EBuildResource::Oxygen: return ResourcesTexures[2];
	   case EBuildResource::Food: return ResourcesTexures[3];
	   default: return nullptr;
	}
}

void UBuildResourcesGridWidget::OnResourcesUpdated(TArray<FBuildResourceData> Resources)
{
	if (!Resources_Grid) return;

	Resources_Grid->ClearChildren();

	if (Resources.Num() > 0 && gridSlotWidgetClass && GetWorld())
	{
		auto world = GetWorld();

		for (const auto& el : Resources)
		{
			UBuildResourcesGridSlotWidget* widget = CreateWidget<UBuildResourcesGridSlotWidget>(world, gridSlotWidgetClass);
			if (widget)
			{
				widget->SetContent(GetResourceTexture(el.Resource), el.Value);
				Resources_Grid->AddChild(widget);
			}
		}
	}
}


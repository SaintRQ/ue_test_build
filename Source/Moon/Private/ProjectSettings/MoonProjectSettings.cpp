// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectSettings/MoonProjectSettings.h"
#include "Blueprint/UserWidget.h"




UMoonProjectSettings::UMoonProjectSettings()
{
	CategoryName = TEXT("Moon");
	SectionName = TEXT("Assets");

}

UObject* UMoonProjectSettings::GetAssetByID(FName AssetID) const
{
    for (const FProjectSettingsAssetData& AssetData : Assets)
    {
        if (AssetData.AssetID == AssetID)
        {
            return AssetData.Asset.LoadSynchronous();
        }
    }

    return nullptr;
}

TSubclassOf<UUserWidget> UMoonProjectSettings::GetWidgetByID(FName AssetID) const
{
    for (const FProjecSettingsAssetWidgetData& AssetData : Widgets)
    {
        if (AssetData.AssetID == AssetID)
        {
            return AssetData.Widget.LoadSynchronous();
        }
    }

    return nullptr;
}

UMoonProjectSettingsGame::UMoonProjectSettingsGame()
{
    CategoryName = TEXT("Moon");
    SectionName = TEXT("Game");
}

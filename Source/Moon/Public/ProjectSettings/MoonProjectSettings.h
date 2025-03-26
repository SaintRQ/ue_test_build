// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MoonProjectSettings.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FProjectSettingsAssetData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
    FName AssetID = {};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
    TSoftObjectPtr<UObject> Asset = {};
};

USTRUCT(BlueprintType)
struct FProjecSettingsAssetWidgetData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
    FName AssetID = {};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
    TSoftClassPtr<UUserWidget> Widget = {};
};


UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Assets"))
class MOON_API UMoonProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UMoonProjectSettings();

    UPROPERTY(EditAnywhere, Config, Category = "Assets", meta = (DisplayName = "Assets References"))
    TArray<FProjectSettingsAssetData> Assets;

    UPROPERTY(EditAnywhere, Config, Category = "Assets", meta = (DisplayName = "Widgets References"))
    TArray<FProjecSettingsAssetWidgetData> Widgets;

public:

    UObject* GetAssetByID(FName AssetID) const;

    TSubclassOf<UUserWidget> GetWidgetByID(FName AssetID) const;
	
};


UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Game"))
class MOON_API UMoonProjectSettingsGame : public UDeveloperSettings
{
    GENERATED_BODY()

    UMoonProjectSettingsGame();

public:

    UPROPERTY(EditAnywhere, Config, Category = "Bridge", meta = (DisplayName = "Min bridge distance"))
    float minBridgeDistance = 100.f;

    UPROPERTY(EditAnywhere, Config, Category = "Bridge", meta = (DisplayName = "Max bridge distance"))
    float maxBridgeDistance = 1000.f;

    UPROPERTY(EditAnywhere, Config, Category = "Visualizer", meta = (DisplayName = "Bridge line width"))
    float visualizerLineWidth = 1.f;

    UPROPERTY(EditAnywhere, Config, Category = "Visualizer", meta = (DisplayName = "Bridge line height"))
    float visualizerLineHeight = 1.f;

};

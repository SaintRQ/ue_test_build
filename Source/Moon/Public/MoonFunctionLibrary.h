// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoonFunctionLibrary.generated.h"

class AMoonPlayerController;
class UBuildsManager;
class UHUDManager;
class UMeshesInstanceManager;
class UMoonProjectSettingsGame;


UCLASS()
class MOON_API UMoonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MoonLibrary")
	static UObject* GetAssetByID(FName AssetID);

	UFUNCTION(BlueprintCallable, Category = "MoonLibrary")
	static TSubclassOf<UUserWidget> GetWidgetByID(FName AssetID);

	UFUNCTION(BlueprintCallable, Category = "MoonLibrary")
	static const UMoonProjectSettingsGame* GetGameProjectSettings();


	// Получить менеджер зданий.
	UFUNCTION(BlueprintCallable, Category = "MoonLibrary", meta = (WorldContext = "Context", DefaultToSelf = "Context"))
	static UBuildsManager* GetBuildsManager(const UObject* Context);

	// Получить менеджер игрового интерфейса (UI / HUD).
	UFUNCTION(BlueprintCallable, Category = "MoonLibrary", meta = (WorldContext = "Context", DefaultToSelf = "Context"))
	static UHUDManager* GetHUDManager(const UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "MoonLibrary", meta = (WorldContext = "Context", DefaultToSelf = "Context"))
	static UMeshesInstanceManager* GetMeshesInstanceManager(const UObject* Context);



	// Получить контроллер игрока
	UFUNCTION(BlueprintCallable, Category = "MoonLibrary", meta = (WorldContext = "Context", DefaultToSelf = "Context"))
	static AMoonPlayerController* GetCurrentPlayerController(const UObject* Context);




	
};

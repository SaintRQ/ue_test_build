// Fill out your copyright notice in the Description page of Project Settings.


#include "MoonFunctionLibrary.h"
#include "ProjectSettings/MoonProjectSettings.h"
#include "Blueprint/UserWidget.h"
#include "SubSystems/MeshesInstanceManager.h"

UObject* UMoonFunctionLibrary::GetAssetByID(FName AssetID)
{
	const UMoonProjectSettings* ProjectSettings = GetDefault<UMoonProjectSettings>();
	if (ProjectSettings) return ProjectSettings->GetAssetByID(AssetID);
	return nullptr;
}

TSubclassOf<UUserWidget> UMoonFunctionLibrary::GetWidgetByID(FName AssetID)
{
	const UMoonProjectSettings* ProjectSettings = GetDefault<UMoonProjectSettings>();
	if (ProjectSettings) return ProjectSettings->GetWidgetByID(AssetID);
	return nullptr;
}

const UMoonProjectSettingsGame* UMoonFunctionLibrary::GetGameProjectSettings()
{
	return GetDefault<UMoonProjectSettingsGame>();
}


UBuildsManager* UMoonFunctionLibrary::GetBuildsManager(const UObject* Context)
{
	if(!Context || !Context->GetWorld()) return nullptr;

	UGameInstance* GameInstance = Context->GetWorld()->GetGameInstance();

	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UBuildsManager>();
}

UHUDManager* UMoonFunctionLibrary::GetHUDManager(const UObject* Context)
{
	if (!Context || !Context->GetWorld()) return nullptr;

	UGameInstance* GameInstance = Context->GetWorld()->GetGameInstance();
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UHUDManager>();
}

UMeshesInstanceManager* UMoonFunctionLibrary::GetMeshesInstanceManager(const UObject* Context)
{
	if (!Context || !Context->GetWorld()) return nullptr;

	UGameInstance* GameInstance = Context->GetWorld()->GetGameInstance();
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UMeshesInstanceManager>();
}

AMoonPlayerController* UMoonFunctionLibrary::GetCurrentPlayerController(const UObject* Context)
{
	if (!Context || !Context->GetWorld()) return nullptr;

	return Cast<AMoonPlayerController>(Context->GetWorld()->GetFirstPlayerController());
}

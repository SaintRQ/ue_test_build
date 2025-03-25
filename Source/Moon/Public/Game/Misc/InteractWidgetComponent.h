// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "UserInterface", hidecategories = (Object, Activation, "Components|Activation", Sockets, Base, Lighting, LOD, Mesh), editinlinenew, meta = (BlueprintSpawnableComponent))
class MOON_API UInteractWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	UInteractWidgetComponent(const FObjectInitializer& PCIP);

};

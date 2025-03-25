// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Misc/InteractWidgetComponent.h"


UInteractWidgetComponent::UInteractWidgetComponent(const FObjectInitializer& PCIP)
	: Super(PCIP)	
{
	bReceiveHardwareInput = true;
}



// Fill out your copyright notice in the Description page of Project Settings.



#include "Game/Widgets/Builds/BuildResourcesGridSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void UBuildResourcesGridSlotWidget::SetContent(UTexture* Texture, int32 Value)
{
	if (!Image_Res || !Text_Res) return;

	if(Texture) Image_Res->SetBrushFromTexture(Cast<UTexture2D>(Texture), false);
	Text_Res->SetText(FText::AsNumber(Value));
}

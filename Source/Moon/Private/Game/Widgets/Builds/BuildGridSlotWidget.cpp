// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Widgets/Builds/BuildGridSlotWidget.h"

#include "Game/Widgets/Builds/BuildGridWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"


void UBuildGridSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(Button_Slot) Button_Slot->OnClicked.AddDynamic(this, &UBuildGridSlotWidget::OnButtonClicked_Button_Slot);
	
}

void UBuildGridSlotWidget::NativeDestruct()
{
	if (Button_Slot) Button_Slot->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

void UBuildGridSlotWidget::OnButtonClicked_Button_Slot()
{
	if (buildGridWidget) buildGridWidget->OnGridSlotClicked(tableRowID);
}

void UBuildGridSlotWidget::SetContent(UBuildGridWidget* Parent, FName TableRowID, UTexture2D* Icon)
{
	buildGridWidget = Parent;
	tableRowID = TableRowID;

	if (Image_Slot) Image_Slot->SetBrushFromTexture(Icon, false);
	if (Button_Slot) Button_Slot->SetToolTipText(FText::FromName(tableRowID));
}

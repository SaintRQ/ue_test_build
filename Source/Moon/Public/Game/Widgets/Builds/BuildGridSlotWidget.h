// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildGridSlotWidget.generated.h"

/**
 * 
 */
class UBuildGridWidget;
class UButton;
class UImage;

UCLASS()
class MOON_API UBuildGridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;


protected:

	UPROPERTY(BlueprintReadOnly)
	FName tableRowID;

	UPROPERTY(BlueprintReadOnly)
	UBuildGridWidget* buildGridWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Slot;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Slot;

	UFUNCTION()
	void OnButtonClicked_Button_Slot();

public:

	void SetContent(UBuildGridWidget* Parent, FName TableRowID, UTexture2D* Icon);
	
};

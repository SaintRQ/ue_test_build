// Класс визуализатора для разных ваимодействий. Постройка мостов, разрушение мостов и зданий, пометка выбранных объектов

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractVisualizer.generated.h"

class UInteractVisualizerWidget;
class UInteractWidgetComponent;

UCLASS()
class MOON_API AInteractVisualizer : public AActor
{
	GENERATED_BODY()
	
	AInteractVisualizer();

	int32 visualizerType = -1;
	TArray<AActor*> visualizerTargets;


	FName interactWidgetClassAssetID;
	TSubclassOf<UInteractVisualizerWidget> interactWidgetClass;

	UInteractVisualizerWidget* interactWidgetReference;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UInteractWidgetComponent* WidgetComponent;

public:	

   // Инициализировать визуализатор. Типы: 0 - пометка успех , 1 - пометка провал,  2 - постройка мостов, 3 - снос зданий и мостов
   void InitializeVisualizer(int32 VisualizerType);

   void SetVisualizerTargets(TArray<AActor*> VisualizerTargets);

   //0 - пометка успех, 1 - пометка провал, 2 - постройка мостов, 3 - снос зданий и мостов
   void SetVisualizerType(int32 VisualizerType);

   //0 - пометка успех, 1 - пометка провал, 2 - постройка мостов, 3 - снос зданий и мостов
   int32 GetVisualizerType() const { return visualizerType; }


};

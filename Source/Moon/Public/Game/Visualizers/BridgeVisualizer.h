// Класс визуализатора для мостов. Используется во время размещения зданий.  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeVisualizer.generated.h"

UCLASS()
class MOON_API ABridgeVisualizer : public AActor
{
	GENERATED_BODY()

	ABridgeVisualizer();

	FName visMeshAssetID;
	UMaterialInstanceDynamic* dynamicMaterial;


protected:

	UStaticMeshComponent* Mesh;

public:

	void InitializeVisualizer();

	void SetVisualizerColor(FLinearColor Color);
};

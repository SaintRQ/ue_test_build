// Класс визуализатора для зданий. Используется во время размещения зданий.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/Build/BuildDataTableRow.h"
#include "BuildVisualizer.generated.h"

UCLASS()
class MOON_API ABuildVisualizer : public AActor
{
	GENERATED_BODY()
	

	FBuildDataTableRow buildingData;
	TArray<UMaterialInstanceDynamic*> dynamicMaterials;

	float meshRadius = 0;

	// Sets default values for this actor's properties
	ABuildVisualizer();

public:	

	void InitializeVisualizer(FBuildDataTableRow BuildingData);

	void SetVisualizerColor(FLinearColor Color);

	float GetMeshRadius() const { return meshRadius; }


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

};

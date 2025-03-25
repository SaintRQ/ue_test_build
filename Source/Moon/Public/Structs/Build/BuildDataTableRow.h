#pragma once

#include "Moon/Public/Structs/Build/BuildResourceData.h"
#include "BuildDataTableRow.generated.h"

class ABuild_Core;

USTRUCT(BlueprintType)
struct FBuildDataTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TSubclassOf<ABuild_Core> BuildClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TSoftObjectPtr<UStaticMesh> BuildMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TSoftObjectPtr<UTexture> BuildIcon;

    //Ресурсы, которые производит здание.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TArray<FBuildResourceData> GeneratedResources;

    //Ресурсы, которые потребляет здание.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TArray<FBuildResourceData> ConsumedResources;
  
};
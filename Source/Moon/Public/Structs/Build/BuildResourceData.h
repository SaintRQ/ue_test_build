#pragma once

#include "BuildResourceData.generated.h"

enum class EBuildResource : uint8;

USTRUCT(BlueprintType)
struct FBuildResourceData 
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    EBuildResource Resource = {};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    int32 Value = 0;

  
};
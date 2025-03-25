#pragma once

#include "BridgeDataTableRow.generated.h"

class ABridge_Core;

USTRUCT(BlueprintType)
struct FBridgeDataTableRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TSubclassOf<ABridge_Core> BridgeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build)
    TSoftObjectPtr<UStaticMesh> BridgeMesh;


};
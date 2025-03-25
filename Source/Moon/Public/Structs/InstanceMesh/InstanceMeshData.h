#pragma once

#include "InstanceMeshData.generated.h"


USTRUCT(BlueprintType)
struct FInstanceMeshData 
{
    GENERATED_USTRUCT_BODY()

    UInstancedStaticMeshComponent* instance = {};

    FPrimitiveInstanceId instanceID = FPrimitiveInstanceId();

    UStaticMesh* meshRef = {};


    bool operator==(const FInstanceMeshData& Struct) const
    {
        return meshRef == Struct.meshRef;
    }

    friend uint32 GetTypeHash(const FInstanceMeshData& Struc)
    {
        return GetTypeHash(Struc.meshRef);
    }

};
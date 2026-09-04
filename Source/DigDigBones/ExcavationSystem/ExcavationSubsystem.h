// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainHelpers.h"
#include "Subsystems/WorldSubsystem.h"
#include "ExcavationSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DIGDIGBONES_API UExcavationSubsystem : public UWorldSubsystem {
	GENERATED_BODY()
public:
	UPROPERTY()
	AExcavationTerrain* Terrain;
public:
	
	UFUNCTION(BlueprintCallable)
	void GenerateTerrain(const FTerrainParams& Params);
};

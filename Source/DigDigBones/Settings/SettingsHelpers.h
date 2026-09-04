// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DigDigBones/ExcavationSystem/TerrainHelpers.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SettingsHelpers.generated.h"

/**
 * 
 */
UCLASS()
class DIGDIGBONES_API USettingsHelpers : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetRarityBaseDigSpeed(TEnumAsByte<ERarity> Rarity);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DigDigBones/ExcavationSystem/TerrainHelpers.h"
#include "Engine/DeveloperSettings.h"
#include "PaleoLifeDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="DigDigBones Settings"))
class DIGDIGBONES_API UPaleoLifeDeveloperSettings : public UDeveloperSettings {
	GENERATED_BODY()
public:
	
	UPROPERTY(Config, EditAnywhere)
	TMap<TEnumAsByte<ERarity>, float> RarityDigSpeed;
};

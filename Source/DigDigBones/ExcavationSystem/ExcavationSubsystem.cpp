// Fill out your copyright notice in the Description page of Project Settings.


#include "ExcavationSubsystem.h"

#include "ExcavationTerrain.h"

void UExcavationSubsystem::GenerateTerrain(const FTerrainParams& Params) {
	if (Terrain != nullptr) {
		Terrain->Destroy();
	}
	else {
		Terrain = GetWorld()->SpawnActor<AExcavationTerrain>(Params.TerrainClass, Params.TerrainTransform.GetLocation(), Params.TerrainTransform.GetRotation().Rotator());
	}
	Terrain->GenerateTerrainDatas(Params.TerrainSize, Params.SpawnChances);
	Terrain->RefreshTerrain(Params.TerrainDepthVisibility);
}

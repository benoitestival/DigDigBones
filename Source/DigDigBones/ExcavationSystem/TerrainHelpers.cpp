// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainHelpers.h"

TMap<FVector, EDirection> UTerrainHelpers::NormalToDirectionDictionnary = {
	{{1.0f,0.0f,0.0f}, EDirection::ED_FRONT},
	{{-1.0f,0.0f,0.0f}, EDirection::ED_BACK},
	{{0.0f,1.0f,0.0f}, EDirection::ED_RIGHT},
	{{0.0f,-1.0f,0.0f}, EDirection::ED_LEFT},
	{{0.0f,0.0f,1.0f}, EDirection::ED_TOP},
	{{0.0f,0.0f,-1.0f}, EDirection::ED_BOTTOM}
};

EDirection UTerrainHelpers::ConvertNormalToDirection(const FVector& Normal) {
	EDirection NormalDirection = EDirection::ED_NONE;
	if (Normal.IsUnit()) {
		//Doesnt trust the equal operator in find so doing a for each with custom toelrance error;
		for (auto& Pair : NormalToDirectionDictionnary) {
			if (AreDirectionsNearlyEqual(Pair.Key, Normal)) {
				NormalDirection = Pair.Value;
			}
		}
	}
	return NormalDirection;
}

EDirection UTerrainHelpers::GetOppositeDirection(EDirection Direction) {
	EDirection OppositeDirection = EDirection::ED_NONE;
	if (Direction != EDirection::ED_NONE) {
		uint8 ByteDirection = Direction;
		if (ByteDirection % 2) {
			ByteDirection = ByteDirection - 1;
		}
		else {
			ByteDirection = ByteDirection + 1;
		}
		OppositeDirection = static_cast<EDirection>(ByteDirection);
	}
	return OppositeDirection;
}

bool UTerrainHelpers::AreDirectionsNearlyEqual(const FVector& Normal1, const FVector& Normal2, const float Tolerance) {
	return FMath::IsNearlyEqual(Normal1.X, Normal2.X, Tolerance) && FMath::IsNearlyEqual(Normal1.Y, Normal2.Y, Tolerance) && FMath::IsNearlyEqual(Normal1.Z, Normal2.Z, Tolerance); 
}

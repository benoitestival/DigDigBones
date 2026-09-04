// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainHelpers.h"

#include "DigDigBones/Settings/SettingsHelpers.h"

TMap<FVector, ENormalDirection> UTerrainHelpers::NormalToDirectionDictionnary = {
	{{1.0f,0.0f,0.0f}, ENormalDirection::ED_FRONT},
	{{-1.0f,0.0f,0.0f}, ENormalDirection::ED_BACK},
	{{0.0f,1.0f,0.0f}, ENormalDirection::ED_RIGHT},
	{{0.0f,-1.0f,0.0f}, ENormalDirection::ED_LEFT},
	{{0.0f,0.0f,1.0f}, ENormalDirection::ED_TOP},
	{{0.0f,0.0f,-1.0f}, ENormalDirection::ED_BOTTOM}
};

TMap<TEnumAsByte<ERarity>, float> UTerrainHelpers::RarityDigSpeed = {};

ENormalDirection UTerrainHelpers::ConvertNormalToDirection(const FVector& Normal) {
	ENormalDirection NormalDirection = ENormalDirection::ED_NONE;
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

ENormalDirection UTerrainHelpers::GetOppositeDirection(ENormalDirection Direction) {
	ENormalDirection OppositeDirection = ENormalDirection::ED_NONE;
	if (Direction != ENormalDirection::ED_NONE) {
		uint8 ByteDirection = Direction;
		if (ByteDirection % 2) {
			ByteDirection = ByteDirection - 1;
		}
		else {
			ByteDirection = ByteDirection + 1;
		}
		OppositeDirection = static_cast<ENormalDirection>(ByteDirection);
	}
	return OppositeDirection;
}

bool UTerrainHelpers::AreDirectionsNearlyEqual(const FVector& Normal1, const FVector& Normal2, const float Tolerance) {
	return FMath::IsNearlyEqual(Normal1.X, Normal2.X, Tolerance) && FMath::IsNearlyEqual(Normal1.Y, Normal2.Y, Tolerance) && FMath::IsNearlyEqual(Normal1.Z, Normal2.Z, Tolerance); 
}

FVector UTerrainHelpers::MaskVector(const FVector& Vector) {
	FVector MaskedVector = FVector();
	MaskedVector.X = Vector.X != 0 ? 0 : 1;
	MaskedVector.Y = Vector.Y != 0 ? 0 : 1;
	MaskedVector.Z = Vector.Z != 0 ? 0 : 1;
	return MaskedVector;
}

TArray<FVector> UTerrainHelpers::GenerateOffsets(const FVector& Axis) {
	TArray<FVector> PossibleAxis = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	};
	TArray<FVector> PlaneAxis = TArray<FVector>();
	for (int AxisIndex = 0; AxisIndex < NUM_AXIS; AxisIndex++) {
		if (Axis[AxisIndex] == 0) {
			PlaneAxis.Add(PossibleAxis[AxisIndex]);
		}
	}
	return {
		-PlaneAxis[0] - PlaneAxis[1],
		-PlaneAxis[0] + PlaneAxis[1],
		PlaneAxis[0] - PlaneAxis[1],
		PlaneAxis[0] + PlaneAxis[1],
	};
}

FVector UTerrainHelpers::GetAxisAtIndex(const FVector& Axis, int Index) {
	FVector FoundAxis = FVector::ZeroVector;
	FVector MaskedAxis = MaskVector(Axis);
	int AxisFound = 0;
	TArray<FVector> PossibleAxis = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	};
	for (int AxisIndex = 0; AxisIndex < NUM_AXIS; AxisIndex++) {
		if (MaskedAxis[AxisIndex] != 0) {
			if (AxisFound == Index) {
				FoundAxis = PossibleAxis[AxisIndex];
			}
			AxisFound++;
		}
	}
	return FoundAxis;
}

FRarityTable UTerrainHelpers::BuildLootTable(const TMap<TEnumAsByte<ERarity>, float>& SpawnChances) {
	return FRarityTable(SpawnChances);
}

void UTerrainHelpers::ComputeBaseRarityDigSpeed() {
	TArray<ERarity> DifferentRarities = {ERarity::ER_COMMON, ERarity::ER_UNCOMMON, ERarity::ER_RARE, ERarity::ER_EPIC, ERarity::ER_UNIQUE};
	for (auto& Rarity : DifferentRarities) {
		RarityDigSpeed.Add({Rarity, USettingsHelpers::GetRarityBaseDigSpeed(Rarity)});
	}
}

float UTerrainHelpers::GetRarityDigSpeed(TEnumAsByte<ERarity> Rarity, float DigSpeedMultiplier) {
	if (RarityDigSpeed.Num() == 0) {
		ComputeBaseRarityDigSpeed();
	}
	return *RarityDigSpeed.Find(Rarity) * DigSpeedMultiplier;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TerrainHelpers.generated.h"

#define NUM_AXIS 3

UENUM(BlueprintType)
enum ENormalDirection {
	ED_NONE = 0,
	ED_LEFT = 1,
	ED_RIGHT = 2,
	ED_FRONT = 3,
	ED_BACK = 4,
	ED_TOP = 5,
	ED_BOTTOM = 6,
};

UENUM(BlueprintType)
enum ERarity {
	ER_NONE = 0,
	ER_COMMON = 1,
	ER_UNCOMMON = 2,
	ER_RARE = 3,
	ER_EPIC = 4,
	ER_UNIQUE = 5,
};

USTRUCT(BlueprintType)
struct FVoxel {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Density;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ERarity> Rarity;
};

USTRUCT(BlueprintType)
struct FVoxelCoord {
	GENERATED_BODY()
	
	FVoxelCoord() = default;
	FVoxelCoord(int X, int Y, int Z): Position(X, Y, Z){};
	FVoxelCoord(const FIntVector& Pos): Position(Pos){};
	FVoxelCoord(float X, float Y, float Z): Position(X, Y, Z){};
	FVoxelCoord(const FVector& Pos): Position(Pos){};
	
	FVoxelCoord operator+(const FVoxelCoord& Other) const {
		return FVoxelCoord(Position + Other.Position);
	};
	FVoxelCoord operator+(const FVector& Other) const {
		return FVoxelCoord(Position + FIntVector(Other));
	};
	
	UPROPERTY()
	FIntVector Position;
};



USTRUCT(BlueprintType)
struct FTerrainParams {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExcavationTerrain> TerrainClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntVector TerrainSize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform TerrainTransform;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TerrainDepthVisibility;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TEnumAsByte<ERarity>, float> SpawnChances;
};

template<class T>
struct FAbstractLootTable {
	FAbstractLootTable() = default;
	FAbstractLootTable(const TMap<T, float>& TableDatas) : InternLootTable(TableDatas){
	}
	
	float GetMax() const {
		float Total = 0;
		for (const auto& Entry : InternLootTable) {
			Total += Entry.second;
		}
		return Total;
	}
	
	T operator<<(int Number) {
		T Result = T();
		for (const auto& Entry : InternLootTable) {
			if (Number < Entry.second) {
				return Entry.first;
			}
			Number -= Entry.second;
		}
		return Result;
	}
	
	TMap<T, float> InternLootTable;
};


USTRUCT(BlueprintType)
struct FRarityTable {
	
	FRarityTable() = default;
	FRarityTable(const TMap<TEnumAsByte<ERarity>, float>& TableDatas) {
		RarityTable = FAbstractLootTable<TEnumAsByte<ERarity>>(TableDatas);
	}
	
	GENERATED_BODY()
	FAbstractLootTable<TEnumAsByte<ERarity>> RarityTable;
	
	ERarity operator<<(int Number) {
		return RarityTable << Number;
	}
};


UCLASS()
class DIGDIGBONES_API UTerrainHelpers : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	static TMap<FVector, ENormalDirection> NormalToDirectionDictionnary;
	static TMap<TEnumAsByte<ERarity>, float> RarityDigSpeed;

	//This give a world direction thinking it get a world normal
	UFUNCTION(Blueprintable, BlueprintPure)
	static ENormalDirection ConvertNormalToDirection(const FVector& Normal);

	UFUNCTION(Blueprintable, BlueprintPure)
	static ENormalDirection GetOppositeDirection(ENormalDirection Direction);
	
	UFUNCTION(Blueprintable, BlueprintPure)
	static bool AreDirectionsNearlyEqual(const FVector& Normal1, const FVector& Normal2, const float Tolerance = 0.001f);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector MaskVector(const FVector& Vector);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector> GenerateOffsets(const FVector& Axis);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector GetAxisAtIndex(const FVector& Axis, int Index);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FRarityTable BuildLootTable(const TMap<TEnumAsByte<ERarity>, float>& SpawnChances);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void ComputeBaseRarityDigSpeed();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float GetRarityDigSpeed(TEnumAsByte<ERarity> Rarity, float DigSpeedMultiplier);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TerrainHelpers.generated.h"


UENUM(BlueprintType)
enum EDirection {
	ED_NONE = 0,
	ED_LEFT = 1,
	ED_RIGHT = 2,
	ED_FRONT = 3,
	ED_BACK = 4,
	ED_TOP = 5,
	ED_BOTTOM = 6,
};

USTRUCT(BlueprintType)
struct FVoxel {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TEnumAsByte<EDirection>, float> VoxelDensities;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Density;
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

UCLASS()
class DIGDIGBONES_API UTerrainHelpers : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	static TMap<FVector, EDirection> NormalToDirectionDictionnary;

	//This give a world direction thinking it get a world normal
	UFUNCTION(Blueprintable, BlueprintPure)
	static EDirection ConvertNormalToDirection(const FVector& Normal);

	UFUNCTION(Blueprintable, BlueprintPure)
	static EDirection GetOppositeDirection(EDirection Direction);
	
	UFUNCTION(Blueprintable, BlueprintPure)
	static bool AreDirectionsNearlyEqual(const FVector& Normal1, const FVector& Normal2, const float Tolerance = 0.001f);
};

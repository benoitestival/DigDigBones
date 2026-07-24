// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ExcavationTerrain.generated.h"

UENUM(BlueprintType)
enum EDirection {
	ED_NONE = 0,
	ED_LEFT = 1,
	ED_RIGHT = 2,
	ED_FRONT = 4,
	ED_BACK = 8,
	ED_TOP = 16,
	ED_BOTTOM = 32,
};

USTRUCT(BlueprintType)
struct FVoxel {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TEnumAsByte<EDirection>, float> VoxelDensities;
	
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


UCLASS(Blueprintable, BlueprintType)
class DIGDIGBONES_API AExcavationTerrain : public AActor {
	GENERATED_BODY()

public:
	
	//Terrain
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMeshComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FVoxel> TerrainDatas;
	
	UPROPERTY(BlueprintreadWrite, EditAnywhere, meta = (ClampMin="0", ClampMax="100"))
	int SizeX;
	
	UPROPERTY(BlueprintreadWrite, EditAnywhere, meta = (ClampMin="0", ClampMax="100"))
	int SizeY;
	
	UPROPERTY(BlueprintreadWrite, EditAnywhere, meta = (ClampMin="0", ClampMax="100"))
	int SizeZ;
	
	UPROPERTY(BlueprintreadWrite, EditAnywhere, meta = (ClampMin="0", ClampMax="100"))
	float VoxelHalfSize;
public:
	// Sets default values for this actor's properties
	AExcavationTerrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	
	UFUNCTION(BlueprintCallable)
	void RefreshTerrain();
	
	UFUNCTION()
	TArray<FVector> ComputeFaceVertices(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector ConvertVoxelCoordToWorld(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDensityDiffOnSide(const FVoxelCoord& VoxelCoord, const FVector& SideNormal);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFaceVisible(const FVoxelCoord& Voxel, const FVoxelCoord& NeighborVoxel, const FVector& FaceNormal);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int ConvertXYZToIndex(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOutOfBounds(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector MaskVector(const FVector& Vector);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FVector> GenerateOffsets(const FVector& Axis);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetAxisAtIndex(const FVector& Axis, int Index);
};

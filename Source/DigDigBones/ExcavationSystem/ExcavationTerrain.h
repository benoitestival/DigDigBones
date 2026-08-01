// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "TerrainHelpers.h"
#include "ExcavationTerrain.generated.h"


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

	UFUNCTION(BlueprintCallable)
	TArray<FVector> ComputeSideFaceVertices(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSideFaceVisible(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);

	FVector GetInterpVerticeOnVoxel(const FVoxelCoord& VoxelCoord);
	
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

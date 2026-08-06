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
private:
	void AppendFaceVerticesToTerrain(const FVoxelCoord& VoxelCoord, const TArray<FVector>& FaceVertices, TArray<FVector>& TerrainVertices, TArray<int>& TerrainTriangles, TArray<FVector>& TerrainNormals, TArray<FVector2D>& UVs, TArray<FColor>& VertexColors, TArray<FProcMeshTangent>& Tangents);
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> ComputeTopFaceVertices(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsTopFaceVisible(const FVoxelCoord& VoxelCoord);
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> ComputeSideFaceVertices(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSideFaceVisible(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
private:
	FVector ComputeVoxelFaceMiddlePosition(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
	FVector ComputeVoxelFaceHalfSize(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal);
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector ConvertVoxelCoordToWorld(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int ConvertXYZToIndex(const FVoxelCoord& VoxelCoord);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOutOfBounds(const FVoxelCoord& VoxelCoord);
};

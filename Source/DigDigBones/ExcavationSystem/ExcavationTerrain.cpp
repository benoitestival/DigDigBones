// Fill out your copyright notice in the Description page of Project Settings.


#include "ExcavationTerrain.h"

#define AVG_VERTICES 4
#define NUM_POSSIBLE_FACES 6
#define NUM_CORNERS 4

#define UP 1
#define RIGHT 0

#define BOTTOM_DIR FVector(0.0f, 0.0f, -1.0f)
#define TOP_DIR FVector(0.0f, 0.0f, 1.0f)

// Sets default values
AExcavationTerrain::AExcavationTerrain() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	RootComponent = ProceduralMeshComponent;
	
	
}

// Called when the game starts or when spawned
void AExcavationTerrain::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AExcavationTerrain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AExcavationTerrain::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	
	RefreshTerrain();
}

void AExcavationTerrain::RefreshTerrain() {
	if (ProceduralMeshComponent != nullptr) {
		
		ProceduralMeshComponent->ClearAllMeshSections();
		
		TArray<FVector> Vertices = TArray<FVector>();
		TArray<int> Triangles = TArray<int>();
		TArray<FVector> Normals = TArray<FVector>();
		TArray<FVector2D> UVs = TArray<FVector2D>();
		
		TArray<FColor> VertexColors = TArray<FColor>();
		TArray<FProcMeshTangent> Tangents = TArray<FProcMeshTangent>();
		
		Vertices.Reserve(SizeX * SizeY * SizeZ * AVG_VERTICES);
		Triangles.Reserve(SizeX * SizeY * SizeZ * AVG_VERTICES * 3);
		Normals.Reserve(SizeX * SizeY * SizeZ * AVG_VERTICES);
		UVs.Reserve(SizeX * SizeY * SizeZ * AVG_VERTICES);
		
		for (int X = 0; X < SizeX; X++) {
			for (int Y = 0; Y < SizeY; Y++) {
				for (int Z = 0; Z < SizeZ; Z++) {
					FVoxelCoord Coord = FVoxelCoord(X, Y, Z);
					if (!IsOutOfBounds(Coord)) {
						TArray<FVector> SideNormals = {
							{1.0f, 0.0f, 0.0f},
							{-1.0f, 0.0f, 0.0f},
							{0.0f, 1.0f, 0.0f},
							{0.0f, -1.0f, 0.0f},
						};
						for (auto& SideNormal : SideNormals) {
							TArray<FVector> SideVertices = ComputeSideFaceVertices(Coord, SideNormal);
							//TODO triangle etc
						}
						//todo top face
					}
				}
			}
		}
		
		ProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ProceduralMesh is not initialised"))
	}
}

TArray<FVector> AExcavationTerrain::ComputeSideFaceVertices(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal) {
	TArray<FVector> FaceVertices = TArray<FVector>();
	if (IsSideFaceVisible(VoxelCoord, FaceNormal)) {
		
		FVector FaceMiddlePosition = ComputeVoxelFaceMiddlePosition(VoxelCoord, FaceNormal);
		FVector FaceHalfSize = ComputeVoxelFaceHalfSize(VoxelCoord, FaceNormal);
		
		TArray<FVector> FaceOffsets = UTerrainHelpers::GenerateOffsets(FaceNormal);
		for (auto& FaceOffset : FaceOffsets) {
			FaceVertices.Add(FaceMiddlePosition + FaceHalfSize * FaceOffset);
		}
	}
	return FaceVertices;
}

bool AExcavationTerrain::IsSideFaceVisible(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal) {
	bool IsFaceVisible = false;
	const FVoxelCoord NeighborCoord = VoxelCoord + FaceNormal;

	if (IsOutOfBounds(NeighborCoord)) {
		IsFaceVisible = true;
	}

	if (!IsFaceVisible) {
		if (TerrainDatas[ConvertXYZToIndex(VoxelCoord)].Density > TerrainDatas[ConvertXYZToIndex(NeighborCoord)].Density) {
			IsFaceVisible = true;
		}
	}

	return IsFaceVisible;
}

FVector AExcavationTerrain::ComputeVoxelFaceMiddlePosition(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal) {
	const FVoxelCoord NeighborCoord = VoxelCoord + FaceNormal;
		
	float VoxelDensity = TerrainDatas[ConvertXYZToIndex(VoxelCoord)].Density;
	float NeighborVoxelDensity = !IsOutOfBounds(NeighborCoord) ? TerrainDatas[ConvertXYZToIndex(NeighborCoord)].Density : 0.0f;
	float DensityHeightDiff = (VoxelDensity - NeighborVoxelDensity) * 0.5f;
		
	FVector FaceBottomCenterPosition = ConvertVoxelCoordToWorld(VoxelCoord) + FaceNormal * VoxelHalfSize + BOTTOM_DIR * VoxelHalfSize;
	FVector FaceMiddlePosition = FaceBottomCenterPosition + TOP_DIR * (NeighborVoxelDensity + DensityHeightDiff) * VoxelHalfSize * 2.0f;

	return FaceMiddlePosition;
}

FVector AExcavationTerrain::ComputeVoxelFaceHalfSize(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal) {
	const FVoxelCoord NeighborCoord = VoxelCoord + FaceNormal;
		
	float VoxelDensity = TerrainDatas[ConvertXYZToIndex(VoxelCoord)].Density;
	float NeighborVoxelDensity = !IsOutOfBounds(NeighborCoord) ? TerrainDatas[ConvertXYZToIndex(NeighborCoord)].Density : 0.0f;
	float DensityHeightDiff = (VoxelDensity - NeighborVoxelDensity) * 0.5f;

	float HalfHeightDiff = DensityHeightDiff * VoxelHalfSize * 2.0f;
	float HalfSideDiff = VoxelHalfSize;
	FVector FaceHalfSize = UTerrainHelpers::GetAxisAtIndex(FaceNormal, 0) * HalfSideDiff + UTerrainHelpers::GetAxisAtIndex(FaceNormal, 1) * HalfHeightDiff;

	return FaceHalfSize;
}

FVector AExcavationTerrain::ConvertVoxelCoordToWorld(const FVoxelCoord& VoxelCoord) {
	return GetActorLocation() + FVector(VoxelCoord.Position.X * VoxelHalfSize, VoxelCoord.Position.Y * VoxelHalfSize, VoxelCoord.Position.Z * VoxelHalfSize);
}

int AExcavationTerrain::ConvertXYZToIndex(const FVoxelCoord& VoxelCoord) {
	return VoxelCoord.Position.X * SizeY * SizeZ + VoxelCoord.Position.Y * SizeZ + VoxelCoord.Position.Z;
}

bool AExcavationTerrain::IsOutOfBounds(const FVoxelCoord& VoxelCoord) {
	return VoxelCoord.Position.X < 0 || VoxelCoord.Position.X >= SizeX ||  VoxelCoord.Position.Y < 0 || VoxelCoord.Position.Y >= SizeY || VoxelCoord.Position.Z < 0 || VoxelCoord.Position.Z >= SizeZ;
}


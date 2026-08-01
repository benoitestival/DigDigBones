// Fill out your copyright notice in the Description page of Project Settings.


#include "ExcavationTerrain.h"

#define AVG_VERTICES 4
#define NUM_POSSIBLE_FACES 6
#define NUM_CORNERS 4

#define NUM_AXIS 3

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
						}
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
		float VoxelDensity = TerrainDatas[ConvertXYZToIndex(VoxelCoord)].Density;

		const FVoxelCoord NeighborCoord = VoxelCoord + FaceNormal;
		float NeighborVoxelDensity = TerrainDatas[ConvertXYZToIndex(NeighborCoord)].Density;

		float DensityHeightDiff = (VoxelDensity - NeighborVoxelDensity) * 0.5f;
		
		FVector FaceBottomCenterPosition = ConvertVoxelCoordToWorld(VoxelCoord) + FaceNormal * VoxelHalfSize - BOTTOM_DIR * VoxelHalfSize;
		FVector FaceMiddlePosition = (NeighborVoxelDensity + DensityHeightDiff) * (VoxelHalfSize * 2) * TOP_DIR;

		float HalfHeightDiff = DensityHeightDiff * VoxelHalfSize;
		float HalfSideDiff = VoxelHalfSize;
		FVector FaceHalfSize = GetAxisAtIndex(FaceNormal, 0) * HalfSideDiff + GetAxisAtIndex(FaceNormal, 1) * HalfHeightDiff;
		
		TArray<FVector> FaceOffsets = GenerateOffsets(FaceNormal);
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

FVector AExcavationTerrain::ConvertVoxelCoordToWorld(const FVoxelCoord& VoxelCoord) {
	//TODO
	return {};
}

int AExcavationTerrain::ConvertXYZToIndex(const FVoxelCoord& VoxelCoord) {
	return VoxelCoord.Position.X * SizeY * SizeZ + VoxelCoord.Position.Y * SizeZ + VoxelCoord.Position.Z;
}

bool AExcavationTerrain::IsOutOfBounds(const FVoxelCoord& VoxelCoord) {
	return VoxelCoord.Position.X < 0 || VoxelCoord.Position.X >= SizeX ||  VoxelCoord.Position.Y < 0 || VoxelCoord.Position.Y >= SizeY || VoxelCoord.Position.Z < 0 || VoxelCoord.Position.Z >= SizeZ;
}

FVector AExcavationTerrain::MaskVector(const FVector& Vector) {
	FVector MaskedVector = FVector();
	MaskedVector.X = Vector.X != 0 ? 0 : 1;
	MaskedVector.Y = Vector.Y != 0 ? 0 : 1;
	MaskedVector.Z = Vector.Z != 0 ? 0 : 1;
	return MaskedVector;
}

TArray<FVector> AExcavationTerrain::GenerateOffsets(const FVector& Axis) {
	TArray<FVector> PossibleAxis = {
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
	};
	TArray<FVector> PlaneAxis = TArray<FVector>();
	for (int AxisIndex = 0; AxisIndex < NUM_AXIS; AxisIndex++) {
		if (Axis[AxisIndex] != 0) {
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

FVector AExcavationTerrain::GetAxisAtIndex(const FVector& Axis, int Index) {
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
			if (MaskedAxis[AxisIndex] == Index) {
				FoundAxis = PossibleAxis[AxisIndex];
			}
			else {
				AxisFound++;
			}
		}
	}
	return FoundAxis;
}
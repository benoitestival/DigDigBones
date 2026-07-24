// Fill out your copyright notice in the Description page of Project Settings.


#include "ExcavationTerrain.h"

#define AVG_VERTICES 4
#define NUM_POSSIBLE_FACES 6
#define NUM_CORNERS 4

#define NUM_AXIS 3

#define UP 1
#define RIGHT 0

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
					int VoxelIndex = ConvertXYZToIndex(Coord);
					if (!IsOutOfBounds(Coord)) {
						TArray<FVector> NeighborsOffsets = {
							{1.0f, 0.0f, 0.0f},
							{-1.0f, 0.0f, 0.0f},
							{0.0f, 1.0f, 0.0f},
							{0.0f, -1.0f, 0.0f},
							{0.0f, 0.0f, 1.0f},
							{.0f, 0.0f, -1.0f},
						};
						for (int NeighborIndex = 0; NeighborIndex < NUM_POSSIBLE_FACES; NeighborIndex++) {
							FVoxelCoord NeighborCoord = Coord + NeighborsOffsets[NeighborIndex];
							int NeighborVoxelIndex = ConvertXYZToIndex(NeighborCoord);
							
							TArray<FVector> FaceVertices = ComputeFaceVertices(Coord, NeighborsOffsets[NeighborIndex]);//Actual Coord and actual voxel face normal to neighbor
							if (FaceVertices.Num() > 0) {
								//TODO triangle, normales and uv
							}
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

TArray<FVector> AExcavationTerrain::ComputeFaceVertices(const FVoxelCoord& VoxelCoord, const FVector& FaceNormal) {
	TArray<FVector> FaceVertices = TArray<FVector>();
	if (IsFaceVisible(VoxelCoord, VoxelCoord + FaceNormal, FaceNormal)) {
		FVector VoxelLocation = ConvertVoxelCoordToWorld(VoxelCoord);
		
		FVector FaceAxis = MaskVector(FaceNormal);
		FVector FaceLocalRight = GetAxisAtIndex(FaceAxis, RIGHT);//Here we get the local right of the face as a normal vector
		FVector FaceLocalUp = GetAxisAtIndex(FaceAxis, UP);//Here we get the local up of the face as a normal vector
		
		//The face doesn't render her back, so we only need to think of her local front, left, right, bottom and up
		//Whatever is the face rotation we just need the normal then identify LocalUp and LocalRight and calculate TopLeft, TopRight, BottomLeft, BottomRight depending of the density diff with his neighbors
		//
		//				 	  FaceLocalUp (+)
		//				 		   ^
		//				 		   |
		//				 /_________|__________/
		//				 |         |         |
		//				 |  TL     |     TR  |
		//				 |    *----|----*    |
		//				 |    |    |    |    |
		//				 |----|----+----|----|--> FaceLocalRight (+)
		//				 |    |    |    |    |
		//				 |    *----|----*    |
		//				 |  BL     |     BR  |
		//				 |_________|_________/
		//				 		   |
		//						   |
		//
		//Face Local Right difference in density
		float RightDensity = GetDensityDiffOnSide(VoxelCoord, FaceLocalRight);
		//Face Local Left difference in density
		float LeftDensity = GetDensityDiffOnSide(VoxelCoord, FaceLocalRight * -1);
		//Face Local Top difference in density
		float TopDensity = GetDensityDiffOnSide(VoxelCoord, FaceLocalUp);
		//Face Local Bottom difference in density
		float BottomDensity = GetDensityDiffOnSide(VoxelCoord, FaceLocalUp * -1);
		//Face Local Front difference in density
		float FrontDensity = GetDensityDiffOnSide(VoxelCoord, FaceNormal);
		
		//Local BottomLeft
		FaceVertices.Add(VoxelLocation + LeftDensity * VoxelHalfSize * FaceLocalRight * -1 + BottomDensity * VoxelHalfSize * FaceLocalUp * -1 + FrontDensity * VoxelHalfSize * FaceNormal);
		//Local TopLeft
		FaceVertices.Add(VoxelLocation + LeftDensity * VoxelHalfSize * FaceLocalRight * -1 + TopDensity * VoxelHalfSize * FaceLocalUp + FrontDensity * VoxelHalfSize * FaceNormal);
		//Local BottomRight
		FaceVertices.Add(VoxelLocation + RightDensity * VoxelHalfSize * FaceLocalRight + BottomDensity * VoxelHalfSize * FaceLocalUp * -1 + FrontDensity * VoxelHalfSize * FaceNormal);
		//Local TopRight
		FaceVertices.Add(VoxelLocation + RightDensity * VoxelHalfSize * FaceLocalRight + TopDensity * VoxelHalfSize * FaceLocalUp + FrontDensity * VoxelHalfSize * FaceNormal);
	}
	
	return FaceVertices;

}

float AExcavationTerrain::GetDensityDiffOnSide(const FVoxelCoord& VoxelCoord, const FVector& SideNormal) {
	//TODO should handle if is on the bound
	//TODO
	return 0.0f;
}

FVector AExcavationTerrain::ConvertVoxelCoordToWorld(const FVoxelCoord& VoxelCoord) {
	//TODO
	return {};
}


bool AExcavationTerrain::IsFaceVisible(const FVoxelCoord& Voxel, const FVoxelCoord& NeighborVoxel, const FVector& FaceNormal) {
	//TODO should handle if is on the bound
	return true;
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
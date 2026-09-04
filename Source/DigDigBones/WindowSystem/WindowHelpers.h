// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WindowHelpers.generated.h"


UENUM(BlueprintType)
enum EPlatform {
	P_NULL = 0,
	P_WINDOWS = 1,
	P_LINUX = 2,
	P_MAC = 3,
};

// UENUM(BlueprintType)
// enum EWindowPosition {
// 	EWP_NULL = 0,
// 	EWP_TOPLEFT = 1,
// 	EWP_TOP = 2,
// 	EWP_TOPRIGHT = 3,
// 	EWP_CENTERLEFT = 4,
// 	EWP_CENTER = 5,
// 	EWP_CENTERRIGHT = 6,
// 	EWP_BOTTOMLEFT = 7,
// 	EWP_BOTTOM = 8,
// 	EWP_BOTTOMRIGHT = 9,
// };

USTRUCT(Blueprintable, BlueprintType)
struct FWindowSettings {
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TEnumAsByte<EWindowPosition> Position = EWindowPosition::EWP_NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint DesiredSize = {1280, 720};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HideWindowBorder = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WindowForegroundState = true;
};

UCLASS()
class DIGDIGBONES_API UWindowHelpers : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
};

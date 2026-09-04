// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowHelpers.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WindowManager.generated.h"

/**
 * 
 */

UCLASS()
class DIGDIGBONES_API UWindowManager : public UGameInstanceSubsystem {
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void SetupWindowFromSettings(const FWindowSettings& Settings);
private:
	void HandleApplicationStateChange(bool IsFocused);
	void UpdateWindowForegroundState(bool BringForeground);
	void UpdateWindowBorderVisibility(bool HideBorder);

	void WindowMinimize();
	void WindowMaximize();


private:
	TSharedPtr<SWindow> GetActiveWindow() const;
	TSharedPtr<FGenericWindow> GetNativeWindow() const;
	EPlatform GetPlatform() const;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWindowMinimize);
	UPROPERTY(BlueprintAssignable)
	FOnWindowMinimize OnWindowMinimize;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWindowMaximize);
	UPROPERTY(BlueprintAssignable)
	FOnWindowMaximize OnWindowMaximize;
private:
	FWindowSettings WindowSettings;
	
};

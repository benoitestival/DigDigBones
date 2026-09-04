// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowManager.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/WindowsWindow.h"

//TODO add to use the pluggin
//#include "KB_Window_EBPLibrary.h"
//		PrivateDependencyModuleNames.AddRange(new string[] { "KB_Window_E" });

void UWindowManager::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &UWindowManager::HandleApplicationStateChange);
}

void UWindowManager::SetupWindowFromSettings(const FWindowSettings& Settings) {
	WindowSettings = Settings;

	//First force windowed
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	GameSettings->SetFullscreenMode(EWindowMode::Windowed);
	
	//Second add platform window specific options
	if (GetPlatform() == EPlatform::P_WINDOWS) {
		//Bring window to foreground
		UpdateWindowForegroundState(WindowSettings.WindowForegroundState);

		if (WindowSettings.HideWindowBorder) {
			UE_LOG(LogTemp, Warning, TEXT("dsfsdfsdfdsfsdfsfdsdfs"))
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("bnbnbnbbnbn"))
		}
		//Window Border
		UpdateWindowBorderVisibility(WindowSettings.HideWindowBorder); 
	}
	else {
		//TODO show message saying that foreground feature is not available
	}

	//Third force the desired résolution
	GameSettings->SetScreenResolution(WindowSettings.DesiredSize);
	GameSettings->ApplySettings(false);
}

void UWindowManager::HandleApplicationStateChange(bool IsFocused) {
	if (IsFocused) {
		WindowMaximize();
	}
	else {
		WindowMinimize();
	}
}

void UWindowManager::UpdateWindowForegroundState(bool BringForeground) {
#ifdef _WIN64
	if (BringForeground) {
		SetWindowPos(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		SetWindowPos(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
#endif
}

void UWindowManager::UpdateWindowBorderVisibility(bool HideBorder) {
#ifdef _WIN64
	long WindowMainStyleFlags = GetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_STYLE);
	if (HideBorder) {
		WindowMainStyleFlags = WindowMainStyleFlags & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	}
	else {
		WindowMainStyleFlags = WindowMainStyleFlags | (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	}
	SetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_STYLE, WindowMainStyleFlags);
#endif
}

void UWindowManager::WindowMinimize() {
	OnWindowMinimize.Broadcast();
}

void UWindowManager::WindowMaximize() {
	OnWindowMaximize.Broadcast();
}

TSharedPtr<SWindow> UWindowManager::GetActiveWindow() const {
	TSharedPtr<SWindow> GameWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	if (!GameWindow.IsValid()) {
		//In case pointer in SlateApplication is not valid anymore
		if (GEngine->GameViewport != nullptr) {
			GameWindow = GEngine->GameViewport->GetWindow();
		}
	}
	return GameWindow;
}

TSharedPtr<FGenericWindow> UWindowManager::GetNativeWindow() const {
	return GetActiveWindow()->GetNativeWindow();
}

EPlatform UWindowManager::GetPlatform() const {
	EPlatform Platform = P_NULL;
	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Windows") {
		Platform = P_WINDOWS;
	}
	return Platform;
}



// void UWindowManager::UpdateWindowBorderVisibility(bool HideBorder) {
// #ifdef _WIN64
// 	long WindowMainStyleFlags = GetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_STYLE);
// 	long WindowExtendedStyleFlags = GetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_EXSTYLE);
// 	if (HideBorder) {
// 		//WindowMainStyleFlags = WindowMainStyleFlags & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
// 		WindowMainStyleFlags = WindowMainStyleFlags & ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
// 		//WindowExtendedStyleFlags = WindowExtendedStyleFlags & ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
// 	}
// 	else {
// 		//WindowMainStyleFlags = WindowMainStyleFlags | (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
// 		WindowMainStyleFlags = WindowMainStyleFlags | (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
// 		WindowExtendedStyleFlags = WindowExtendedStyleFlags | (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
// 	}
// 	SetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_STYLE, WindowMainStyleFlags);
// 	SetWindowLong(((FWindowsWindow*)GetNativeWindow().Get())->GetHWnd(), GWL_EXSTYLE, WindowExtendedStyleFlags);
// #endif
// }

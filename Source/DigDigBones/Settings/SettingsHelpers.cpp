// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsHelpers.h"

#include "PaleoLifeDeveloperSettings.h"

float USettingsHelpers::GetRarityBaseDigSpeed(TEnumAsByte<ERarity> Rarity) {
	const UPaleoLifeDeveloperSettings* DefaultSettings = GetDefault<UPaleoLifeDeveloperSettings>();
	return *DefaultSettings->RarityDigSpeed.Find(Rarity);
}

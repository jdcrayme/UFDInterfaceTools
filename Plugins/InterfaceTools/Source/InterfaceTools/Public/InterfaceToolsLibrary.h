// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterfaceToolsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INTERFACETOOLS_API UInterfaceToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Calculates a "nice" number approximately equal to x.
	 * Nice numbers are of the form 1, 2, or 5 * 10^n.
	 * 
	 * @param x The number to make nice.
	 * @return A "nice" number close to x.
	 */
	UFUNCTION(BlueprintCallable, Category="Interface Tools")
	static float NiceNum(float x);

	/**
	 * Generates a "nice" range and tick spacing for graph axes.
	 * 
	 * @param min_val The minimum data value.
	 * @param max_val The maximum data value.
	 * @param num_ticks_approx The approximate desired number of tick marks.
	 * @param nice_min Output parameter for the calculated nice minimum.
	 * @param nice_max Output parameter for the calculated nice maximum.
	 * @param tick_spacing Output parameter for the calculated tick spacing.
	 */
	UFUNCTION(BlueprintCallable, Category="Interface Tools")
	static void GenerateNiceAxisSteps(float min_val, float max_val, int num_ticks_approx, 
							   float& nice_min, float& nice_max, float& tick_spacing);
	
};

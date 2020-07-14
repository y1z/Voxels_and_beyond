// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility.generated.h"



class AGrid3D;


/**
 * @brief A series of function used for utility.
 */
UCLASS()
class VOX_AND_BEYOND_API UUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

  /**
  * @brief Used to save the data in a 3D grid to a file.
  * @param[in] ptr_grid : the grid to extract data from.
  * @param[in] fileName : the name of file where the data is going to.
  */
  UFUNCTION(BlueprintCallable,Category = "saving and loading")
  static bool saveGridData(AGrid3D* const ptr_grid, FString fileName);

  
	
};

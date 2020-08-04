// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Utility.generated.h"



struct GridDataElement;
class AGrid3D;
class AbasePrimitive;


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
  static bool
  saveGridData(AGrid3D* const ptr_grid, FString fileName);

  /**
  * @brief 
  * @param[out] ptr_grid : The pointer to the grid that will contain the data
  * from the file.
  * @param[in] fileName : The name of the file.
  */
  UFUNCTION(BlueprintCallable,Category = "saving and loading")
  static bool
  loadGridData(AGrid3D* const ptr_grid, FString fileName);

private:


  /**
  * @brief parses the data from a string and returns it from a TArray
  * @param[in] data : contains the data for positions in the grid
  */
  static TArray<GridDataElement>
  parseData(const FString& dataSource);

  /**
  * @brief parses and puts the data .
  * @param[in] data : contains the data for positions in the grid
  */
  static FIntVector4
  parseDigitsInRegex(FRegexMatcher& matcher,
                     const FString& dataSource,
                     const bool threeDigits = true);
  
  /**
  * @returns The string with the relevant data.
  * @param[in] data : contains the relevant data.
  */
  static FString
  loadPositionData(const FString& data);

  /**
  * @returns The path to the save directory.
  */
  UFUNCTION(BlueprintGetter,Category = "saving and loading")
  static FString
  getSaveDirectory();

public:

  /**
  * @brief : creates a copy of a FString given a range.
  * @param[in] startPosition : The position to start from.
  * @param[in] endPosition : The position to start from.
  * @param[in] originalString : The string that being copied from. 
  */
  static FString
  createSubString(const int32 startPosition,
                  const int32 endPosition,
                  const FString& originalString);

  /**
  * @brief : creates a string-view from a string.
  * @param[in] startPosition : The position to start from.
  * @param[in] endPosition : The position to start from.
  * @param[in] originalString : The string that being copied from. 
  */
  static FStringView
  createSubStringView(const int32 startPosition,
                      const int32 endPosition,
                      const FString& originalString);

  /**
  * @returns true when all the character of the string are numbers.
  */
  static bool 
  isNumber(const TCHAR *TString);
};

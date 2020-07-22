// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"
#include "Grid3D.h"
#include "basePrimitive.h"
#include "Misc/FileHelper.h"
#include "Internationalization/Regex.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "HAL/PlatformFilemanager.h"

#include <cassert>

bool
UUtility::saveGridData(AGrid3D* const ptr_grid,
                       FString fileName)
{
  FString const fullDefaultPathForProject = getSaveDirectory();

  FString const fullPath = fullDefaultPathForProject + fileName;


  FText errorText;
  bool const isPathUsable = FFileHelper::IsFilenameValidForSaving(fullPath, errorText);

  if( GEngine && !errorText.IsEmpty() )
  {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, errorText.ToString());
  }


  if( isPathUsable )
  {
    FString Result = TEXT("v: ");
    for( auto& ptrPrimitive : ptr_grid->m_primitives )
    {
      auto const ID = ptrPrimitive->getId();
      Result.AppendInt(ID.X);
      Result.AppendChar(TEXT(' '));
      Result.AppendInt(ID.Y);
      Result.AppendChar(TEXT(' '));
      Result.AppendInt(ID.Z);
      Result.AppendChar(TEXT(','));
      Result.AppendChar(TEXT(' '));
    }

    Result.AppendChar(TEXT(';'));

    return FFileHelper::SaveStringToFile(Result, *fullPath);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("invalid save location"));
  }

  return  isPathUsable;
}

bool
UUtility::loadGridData(AGrid3D* const ptr_grid,
                       FString fileName)
{
  const FString saveDirectory = getSaveDirectory();

  FString const fullPath = saveDirectory + fileName;

  FText errorText;
  bool const isPathUsable = FFileHelper::IsFilenameValidForSaving(fullPath, errorText);

  if( GEngine && !errorText.IsEmpty() )
  {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, errorText.ToString());
  }

  FString fileData;
  bool const isLoaded = FFileHelper::LoadFileToString(fileData, *fullPath, FFileHelper::EHashOptions::None, 0);
  if( isLoaded && isPathUsable )
  {
    const FString unParsedPositionData = loadPositionData(fileData);

    GridData gridData;

    gridData.m_vectorIds = parsePositionData(unParsedPositionData);
  }

  return isLoaded;
}

TArray<FIntVector>
UUtility::parsePositionData(const FString& data)
{
  TArray<FIntVector> result;

  FRegexPattern const intVectorPattern(TEXT("(,?[0-9]+ [0-9]+ [0-9]+)"));
  FRegexMatcher integerVectorMatches(intVectorPattern, data);

  {

    FRegexPattern const individualDigit(TEXT("([0-9]+)"));

    FIntVector intermediateVector(0,0,0);

    int32 currentIndex = 0;
    currentIndex = integerVectorMatches.GetBeginLimit();
    int32 const LastIndex =  integerVectorMatches.GetEndLimit();
    while( currentIndex < LastIndex )
    {
      integerVectorMatches.FindNext();
      const auto startIndex = integerVectorMatches.GetMatchBeginning();
      const auto endIndex = integerVectorMatches.GetMatchEnding();
      if( -1 == startIndex || -1 == endIndex)
      {
        break;
      }

      const FString subString = createSubString(startIndex, endIndex, data);
      FRegexMatcher individualDigitMatch(individualDigit, subString);
      for(int32 i = 0; i < 3; ++i )
      {
        individualDigitMatch.FindNext();
        const auto valueInString = createSubString(individualDigitMatch.GetMatchBeginning(),
                                                   individualDigitMatch.GetMatchEnding(),
                                                   subString);
        int32 outVal;
        LexFromString(outVal, *valueInString);
        intermediateVector[i] = outVal;
      }
      result.Add(intermediateVector);
      ++currentIndex;
    } 
  }

  return  result;
}

FString
UUtility::loadPositionData(const FString& data)
{
  const int32 positionDataBeginIndex = data.Find(TEXT("v:"), ESearchCase::IgnoreCase,
                                                 ESearchDir::FromStart, 0);

  const int32 positionDataEndIndex = data.Find(TEXT(";"), ESearchCase::IgnoreCase,
                                               ESearchDir::FromStart, positionDataBeginIndex);

  return createSubString(positionDataBeginIndex, positionDataEndIndex, data); 
}

FString
UUtility::getSaveDirectory()
{
  return FPaths::ConvertRelativePathToFull(FPaths::GameUserDeveloperDir());
}

FString 
UUtility::createSubString(const int32 startPosition,
                          const int32 endPosition,
                          const FString& originalString)
{
  checkf(startPosition < endPosition,
         TEXT("startPosition = %d must be smaller than endPosition = %d"),
         startPosition,
         endPosition);

  FString result;
  result.Reserve(endPosition - startPosition);

  for(int32 i = startPosition; i <= endPosition; ++i )
  {
    result.AppendChar(originalString[i]);
  }

  return result;
}

bool
UUtility::isNumber(const TCHAR* TString)
{
  constexpr static TCHAR* validCharacter = TEXT("0123456789");
  bool result = true;
  int32 currentIndex = 0;
  while( TEXT('\0') != TString[currentIndex] )
  {
    for( int32 i = 0; i < 10; ++i )
    {
      result = false;
      if( TString[currentIndex] == validCharacter[i] )
      {
        result = true;
        break;
      }
    }

    ++currentIndex;
  }


  return  result;
}

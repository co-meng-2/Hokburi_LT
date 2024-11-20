// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CmSCG_DTPoolManager.generated.h"


struct FCmSCG_TableRowBase;

USTRUCT(BlueprintType)
struct FDTData
{
	GENERATED_BODY()
	
	TArray<FName> Arr_Name;
	TArray<FCmSCG_TableRowBase> Arr_Data;
};

USTRUCT()
struct FCmSCG_TableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Pool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMARTCHOICEGIVER_API UCmSCG_DTPoolManager : public UActorComponent /*GameMode*/
{
	GENERATED_BODY()

public:
	UCmSCG_DTPoolManager();

public:
	UFUNCTION(BlueprintCallable)
	FDTData& GetDTData(const UDataTable* InDataTable);
	
private:
	UPROPERTY()
	TMap<FName, FDTData> ArrM_DTData;
};

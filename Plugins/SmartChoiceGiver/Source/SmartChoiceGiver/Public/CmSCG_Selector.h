// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CmSCG_DTPoolManager.h"
#include "CmSCG_Selector.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FSelectorSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<const UDataTable> DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Policy")
	uint8 bUsePool : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Policy")
	uint8 bUseProbability : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Policy")
	uint8 bUseMulProbability : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Policy")
	uint8 bAllowDuplicate : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Block")
	TSet<FName> InBlocked;
};

USTRUCT(BlueprintType)
struct FSelectionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UDataTable> DataTable;
};

UCLASS(BlueprintType, Blueprintable)
class SMARTCHOICEGIVER_API UCmSCG_Selector : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FSelectionInfo> GetRandomlySelected();
	
	TArray<FSelectorSetting>& GetSelectorSettings(){return Settings;}
	UFUNCTION(BlueprintCallable)
	FSelectorSetting& GetSettingAt(int Idx){return Settings[Idx];}
	
private:
	UPROPERTY(EditAnywhere, Category ="Setting")
	TArray<FSelectorSetting> Settings;
	float CalcProbability(FName InRowName, FCmSCG_TableRowBase& InTableRow, const FSelectorSetting& InSetting, const TSet<FName>& InSelected);
};

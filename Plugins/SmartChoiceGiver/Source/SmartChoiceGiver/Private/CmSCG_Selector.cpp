// Fill out your copyright notice in the Description page of Project Settings.


#include "CmSCG_Selector.h"

#include "GameFramework/GameModeBase.h"

float UCmSCG_Selector::CalcProbability(FName InRowName, FCmSCG_TableRowBase& InTableRow, const FSelectorSetting& InSetting, const TSet<FName>& InSelected)
{
	if(InSetting.InBlocked.Contains(InRowName) || (!InSetting.bAllowDuplicate && InSelected.Contains(InRowName)))
	{
		return 0.f;
	}
	
	float Add{1.f};
	if(InSetting.bUseProbability)
	{
		Add = InTableRow.Probability;
	}
	if (InSetting.bUsePool)
	{
		Add *= InTableRow.Pool ? 1.f : 0.f;
		if(InSetting.bUseMulProbability)
		{
			Add *= InTableRow.Pool;  
		}
	}
	return Add;
}

TArray<FSelectionInfo> UCmSCG_Selector::GetRandomlySelected()
{
	AGameModeBase* GameMode{GetWorld()->GetAuthGameMode()};
	UCmSCG_DTPoolManager* DTPoolManager{GameMode->GetComponentByClass<UCmSCG_DTPoolManager>()};
	TArray<FSelectionInfo> Arr_Ret;
	TSet<FName> SelectedRowNames;
	for(const FSelectorSetting& Setting : Settings)
	{
		const UDataTable* DataTable{Setting.DataTable};
		FDTData& DTData{DTPoolManager->GetDTData(DataTable)};

		float SumProb{0.f};
		for (int j = 0; j < DTData.Arr_Data.Num(); ++j)
		{
			SumProb += CalcProbability(DTData.Arr_Name[j], DTData.Arr_Data[j], Setting, SelectedRowNames);
		}
		
		if(SumProb == 0.f)
		{
			Arr_Ret.Add(FSelectionInfo());
			continue;
		}

		float RandProb = FMath::FRandRange(0.0000001f, SumProb);
		float CurProb = 0.f;
		for(int j = 0; j < DTData.Arr_Data.Num(); ++j)
		{
			CurProb += CalcProbability(DTData.Arr_Name[j], DTData.Arr_Data[j], Setting, SelectedRowNames);
			if(CurProb >= RandProb)
			{
				if(Setting.bUsePool)
				{
					--DTData.Arr_Data[j].Pool;
				}
				Arr_Ret.Add({DTData.Arr_Name[j], DataTable});
				if(Setting.bAllowDuplicate)
				{
					SelectedRowNames.Add(DTData.Arr_Name[j]);
				}
				break;
			}
		}
	}
	
	return Arr_Ret;
}

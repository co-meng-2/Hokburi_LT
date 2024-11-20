#include "Components/CmSCG_DTPoolManager.h"

UCmSCG_DTPoolManager::UCmSCG_DTPoolManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FDTData& UCmSCG_DTPoolManager::GetDTData(const UDataTable* InDataTable)
{
	if(FDTData* DTDataPtr{ArrM_DTData.Find(InDataTable->GetFName())})
	{
		return *DTDataPtr;
	}
	else
	{
		FDTData& NewData = ArrM_DTData.Add(InDataTable->GetFName(), FDTData());

		TArray<FCmSCG_TableRowBase*> Arr_Out;
		TMap<FName, uint8*> RowMap{InDataTable->GetRowMap()};
		for(auto It{RowMap.CreateConstIterator()}; It; ++It)
		{
			NewData.Arr_Name.Add(It.Key());
			NewData.Arr_Data.Add(*reinterpret_cast<FCmSCG_TableRowBase*>(It.Value()));
		}
		return NewData; 
	}
}



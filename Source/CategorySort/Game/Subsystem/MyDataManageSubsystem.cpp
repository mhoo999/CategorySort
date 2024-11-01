// © 2024 Hoon. A touch of creativity in every line of code.


#include "Game/Subsystem/MyDataManageSubsystem.h"

UMyDataManageSubsystem::UMyDataManageSubsystem()
{
	// 컨테이너에 데이터 테이블을 추가하는 함수
	FString Dir = FPaths::ProjectContentDir();
	Dir.Append(TEXT("DataTable/"));

	FString FileExt = TEXT("uasset");
	FString PureFileName;
	TArray<FString> FoundFileNames;

	IFileManager::Get().FindFiles(FoundFileNames, *Dir, *FileExt);

	for (int32 i = 0; i < FoundFileNames.Num(); i++)
	{
		FString DTPath = TEXT("/Game/DataTable/");

		FPaths::Split(FoundFileNames[i], Dir, PureFileName, FileExt);

		DTPath.Append(PureFileName);
		DTPath.Append(TEXT("."));
		DTPath.Append(PureFileName);

		ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(*DTPath);

		if (DataTableRef.Object)
		{
			PureFileName.RemoveFromStart(TEXT("DT_"));
			DTContainer.Add(PureFileName, DataTableRef.Object);
		}
	}
}

UDataTable* UMyDataManageSubsystem::FindDataTable(const FString& DataTableName) const
{
	UDataTable* DataTable = DTContainer.FindRef(DataTableName);
	ensureMsgf(DataTable, TEXT("UMyDataManageSubsystem::FindDataTable : DataTableName[%s] is Wrong!!"), *DataTableName);
	return DataTable;
}

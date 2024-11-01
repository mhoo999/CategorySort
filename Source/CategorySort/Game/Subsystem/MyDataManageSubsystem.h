// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyDataManageSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyDataManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	UMyDataManageSubsystem();

private:
	UPROPERTY()
	TMap<FString, UDataTable*> DTContainer;

public:
	class UDataTable* FindDataTable(const FString& DataTableName) const;

};

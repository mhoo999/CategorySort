// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MyListRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyListRowWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> Number;

	UPROPERTY()
	TObjectPtr<class UTextBlock> Name;

	UPROPERTY()
	TObjectPtr<class UTextBlock> Strength;

	UPROPERTY()
	TObjectPtr<class UTextBlock> Intelligence;

	UPROPERTY()
	TObjectPtr<class UTextBlock> Vitality;

};

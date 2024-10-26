// © 2024 Hoon. A touch of creativity in every line of code.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCategorySortWidget.generated.h"

/**
 * 
 */
UCLASS()
class CATEGORYSORT_API UMyCategorySortWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UMyCategorySortWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY()
	TSubclassOf<class UMyListRowWidget> MyListRowWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMyButton> Number;

	UPROPERTY()
	TObjectPtr<class UMyButton> Name;

	UPROPERTY()
	TObjectPtr<class UMyButton> Strength;

	UPROPERTY()
	TObjectPtr<class UMyButton> Intelligence;

	UPROPERTY()
	TObjectPtr<class UMyButton> Vitality;

	UPROPERTY()
	TObjectPtr<class UListView> ListView;

private:
	void GenerateRows(int32 RowCount);


};

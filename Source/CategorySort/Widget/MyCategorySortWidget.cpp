// © 2024 Hoon. A touch of creativity in every line of code.


#include "Widget/MyCategorySortWidget.h"
#include "Widget/MyListRowWidget.h"
#include "Widget/Component/MyButton.h"

#include "Components/ListView.h"
#include "Components/HorizontalBox.h"

#include "Game/Subsystem/MyDataManageSubsystem.h"
#include "GameData/MyCharacterData.h"
#include "Engine/GameInstance.h"
#include "MyCategorySortWidget.h"


UMyCategorySortWidget::UMyCategorySortWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMyListRowWidget> MyListRowWidgetClassRef(TEXT("/Game/UI/WBP_ListRow.WBP_ListRow_C"));
	if (MyListRowWidgetClassRef.Class)
	{
		MyListRowWidgetClass = MyListRowWidgetClassRef.Class;
	}
}

void UMyCategorySortWidget::NativeConstruct()
{
	GenerateRows();

	for (int32 i = 0; i < ColumnContainer->GetChildrenCount(); i++)
	{
		UMyButton* SortColumn = Cast<UMyButton>(ColumnContainer->GetChildAt(i));
		SortColumn->SetStringParameter1(SortColumn->GetName());
		SortColumn->OnClickStringParamDelegate.BindUObject(this, &UMyCategorySortWidget::Sort);
		SortColumn->AddOnClickEvent();
	}
}

void UMyCategorySortWidget::Sort(FString& InSortCondition)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *InSortCondition);
}

void UMyCategorySortWidget::GenerateRows()
{
	UMyDataManageSubsystem* DataManageSubsystem = Cast<UGameInstance>(GetWorld()->GetGameInstance())->GetSubsystem<UMyDataManageSubsystem>();
	UDataTable* CharacterData = DataManageSubsystem->FindDataTable(TEXT("CharacterData"));

	if (CharacterData)
	{
		TArray<FMyCharacterData*> AllRows;
		CharacterData->GetAllRows(NULL, AllRows);

		for (int32 i = 0; i < AllRows.Num(); i++)
		{
			UMyListRowWidget* NewRow = CreateWidget<UMyListRowWidget>(this, MyListRowWidgetClass);
			NewRow->SetData(AllRows[i]);
			ListView->AddItem(NewRow);
		}
	}
}

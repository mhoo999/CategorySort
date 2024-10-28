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

		if (i == 0)
		{
			FString SortCondition = SortColumn->GetName();
			Sort(SortCondition);
		}
	}
}

void UMyCategorySortWidget::Sort(FString& InSortCondition)
{
	if (CurrentSort != InSortCondition)
	{
		DisableAllButOne(InSortCondition);
		CurrentSort = InSortCondition;
		bAscending = true;
	}

	TArray<UObject*> CharacterArray = ListView->GetListItems();

	if (!bAscending)
	{
		CharacterArray.Sort([this, InSortCondition](UObject& A, UObject& B)
		{
			UMyListRowWidget* CharacterA = Cast<UMyListRowWidget>(&A);
			UMyListRowWidget* CharacterB = Cast<UMyListRowWidget>(&B);

			if (!CharacterA || !CharacterB) { return false; }

			auto ValueA = CharacterA->FindStat(InSortCondition);
			auto ValueB = CharacterB->FindStat(InSortCondition);

			if (ValueA.IsNumeric() && ValueB.IsNumeric())
			{
				return FCString::Atoi(*ValueA) >= FCString::Atoi(*ValueB);
			}
			else
			{
				return ValueA >= ValueB;
			}
		});

		bAscending = true;
	}
	else
	{
		CharacterArray.Sort([this, InSortCondition](UObject& A, UObject& B)
			{
				UMyListRowWidget* CharacterA = Cast<UMyListRowWidget>(&A);
				UMyListRowWidget* CharacterB = Cast<UMyListRowWidget>(&B);

				if (!CharacterA || !CharacterB) { return false; }

				auto ValueA = CharacterA->FindStat(InSortCondition);
				auto ValueB = CharacterB->FindStat(InSortCondition);

				if (ValueA.IsNumeric() && ValueB.IsNumeric())
				{
					return FCString::Atoi(*ValueA) <= FCString::Atoi(*ValueB);
				}
				else
				{
					return ValueA <= ValueB;
				}
			});

		bAscending = false;
	}

	ListView->ClearListItems();

	for (UObject* Character : CharacterArray)
	{
		ListView->AddItem(Character);
	}
}

void UMyCategorySortWidget::DisableAllButOne(FString& InSortCondition)
{
	for (int32 i = 0; i < ColumnContainer->GetChildrenCount(); i++)
	{
		UMyButton* SortColumn = Cast<UMyButton>(ColumnContainer->GetChildAt(i));

		if (SortColumn->GetName() != InSortCondition)
		{
			SortColumn->SetDisable();
		}
		else
		{
			SortColumn->SetEnable();
		}
	}
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

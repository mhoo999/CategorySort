// © 2024 Hoon. A touch of creativity in every line of code.


#include "Widget/MyCategorySortWidget.h"
#include "Widget/MyListRowWidget.h"
#include "Widget/MyStatPopupWidget.h"

#include "Widget/Component/MyButton.h"
#include "Widget/Component/MyTagButtonWidget.h"

#include "Components/ListView.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"

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

	static ConstructorHelpers::FClassFinder<UMyTagButtonWidget> TagButtonClassRef(TEXT("/Game/UI/WBP_TagButton.WBP_TagButton_C"));
	if (TagButtonClassRef.Class)
	{
		TagButtonClass = TagButtonClassRef.Class;
	}
}

void UMyCategorySortWidget::NativeConstruct()
{
	CloseArea->OnClicked.AddDynamic(this, &UMyCategorySortWidget::ClosePopup);
	TagClearButton->OnClicked.AddDynamic(this, &UMyCategorySortWidget::ClearFilters);
	StatPopupButton->OnClicked.AddDynamic(this, &UMyCategorySortWidget::OpenFilterStatsPopup);

	StatPopup->SetParentWidget(this);

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

void UMyCategorySortWidget::ClosePopup()
{
	StatPopup->SetVisibility(ESlateVisibility::Hidden);
	CloseArea->SetVisibility(ESlateVisibility::Hidden);
}

void UMyCategorySortWidget::ApplyFilter()
{
	ClosePopup();
}

void UMyCategorySortWidget::ClearFilters()
{
	TagClearButton->SetVisibility(ESlateVisibility::Hidden);
	TagContainer->ClearChildren();

	ApplyFilter();
}

void UMyCategorySortWidget::AddRangeFilter(FString& InCondition, FVector2D InRange)
{
	FString NewButtonName = InCondition + TEXT(": ") + FString::FromInt(InRange.X) + TEXT("-") + FString::FromInt(InRange.Y);

	for (int32 i = 0; i < TagContainer->GetChildrenCount(); i++)
	{
		UMyTagButtonWidget* Tag = Cast<UMyTagButtonWidget>(TagContainer->GetChildAt(i));

		if (Tag->GetFilteringCondition() == InCondition)
		{
			if (Tag->GetDisplayName() == NewButtonName)
			{
				return;
			}
			else
			{
				TagContainer->RemoveChildAt(i);
				break;
			}
		}
	}

	AddTag(NewButtonName, InCondition, InRange);
}

void UMyCategorySortWidget::RemoveFilter(FString& InCondition)
{
	for (int32 i = 0; i < TagContainer->GetChildrenCount(); i++)
	{
		UMyTagButtonWidget* Tag = Cast<UMyTagButtonWidget>(TagContainer->GetChildAt(i));
		if (Tag->GetFilteringCondition() == InCondition)
		{
			TagContainer->RemoveChildAt(i);
		}
	}

	if (TagContainer->GetChildrenCount() <= 0)
	{
		TagClearButton->SetVisibility(ESlateVisibility::Hidden);
	}

	ApplyFilter();
}

void UMyCategorySortWidget::RefreshList()
{
	ListView->ClearListItems();
	GenerateRows();
}

void UMyCategorySortWidget::AddTag(FString& InTagName, FString& InCondition, FVector2D InRange)
{
	UMyTagButtonWidget* NewTag = CreateWidget<UMyTagButtonWidget>(this, TagButtonClass);

	NewTag->SetDisplayName(InTagName);
	NewTag->SetFilteringCondition(InCondition);
	NewTag->SetValue(InRange);
	NewTag->SetParentWidget(this);

	NewTag->GetButton()->SetStringParameter1(InCondition);
	NewTag->GetButton()->OnClickStringParamDelegate.BindUObject(this, &UMyCategorySortWidget::RemoveFilter);

	NewTag->SetPadding(FVector4d(0.f, 10.f, 10.f, 10.f));
	TagContainer->AddChild(NewTag);

	if (TagClearButton->GetVisibility() == ESlateVisibility::Hidden)
	{
		TagClearButton->SetVisibility(ESlateVisibility::Visible);
	}

	ApplyFilter();
}

void UMyCategorySortWidget::OpenFilterStatsPopup()
{
	CloseArea->SetVisibility(ESlateVisibility::Visible);
	StatPopup->SetVisibility(ESlateVisibility::Visible);
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

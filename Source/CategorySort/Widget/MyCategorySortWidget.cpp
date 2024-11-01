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

	// 리스트 초기화
	LoadCharacters(GenerateRows());

	// 정렬용 컬럼 버튼 초기화
	for (int32 i = 0; i < ColumnContainer->GetChildrenCount(); i++)
	{
		UMyButton* SortColumn = Cast<UMyButton>(ColumnContainer->GetChildAt(i));
		SortColumn->SetStringParameter1(SortColumn->GetName());
		SortColumn->OnClickStringParamDelegate.BindUObject(this, &UMyCategorySortWidget::SortButtonClickEvent);
		SortColumn->AddOnClickEvent();

		if (i == 0)
		{
			FString SortCondition = SortColumn->GetName();
			Sort(SortCondition);
		}
	}
}

void UMyCategorySortWidget::SortButtonClickEvent(FString& InSortCondition)
{
	// 정렬 버튼이 중복되어 눌렸을 때, 오름차순 <-> 내림차순으로 변경하여 정렬
	if (CurrentSort != InSortCondition)
	{
		DisableAllButOne(InSortCondition);
		CurrentSort = InSortCondition;
		bAscending = true;
	}

	// 정렬을 실행
	Sort(InSortCondition);
}

void UMyCategorySortWidget::Sort(FString& InSortCondition)
{
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

			// 숫자일 경우 int로 변환하여 정렬 실행
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
	ListView->ClearListItems();

	// 태그 컨테이너에 태그가 있을 경우에만 필터링을 실행
	if (TagContainer->GetChildrenCount() > 0)
	{
		TArray<FMyCharacterData*> AllRows;
		GenerateRows()->GetAllRows(TEXT("CharacterData"), AllRows);

		for (FMyCharacterData* Row : AllRows)
		{
			bool bPass = true;
			
			for (int32 i = 0; i < TagContainer->GetChildrenCount(); i++)
			{
				UMyTagButtonWidget* Tag = Cast<UMyTagButtonWidget>(TagContainer->GetChildAt(i));

				FString Condition = Tag->GetFilteringCondition();
				int32 ConditionValue = Row->GetStatValue(Condition);

				int32 MinValue = Tag->GetValue().X;
				int32 MaxValue = Tag->GetValue().Y;
				// 로우 안에서 (Condition)값을 찾아, 태그의 최소 값과 최대 값의 범위 안인지 확인
				if (ConditionValue < MinValue || ConditionValue > MaxValue)
				{
					bPass = false;
					break;
				}
			}

			// 모든 조건을 충족할 경우 새로운 로우로 데이터를 추가
			if (bPass)
			{
				UMyListRowWidget* NewRow = CreateWidget<UMyListRowWidget>(this, MyListRowWidgetClass);
				NewRow->SetData(Row);
				ListView->AddItem(NewRow);
			}
		}
	}
	else
	{
		LoadCharacters(GenerateRows());
	}

	ClosePopup();
	// 정렬 상태를 유지하기 위해 재정렬
	Sort(CurrentSort);
}

void UMyCategorySortWidget::ClearFilters()
{
	TagClearButton->SetVisibility(ESlateVisibility::Hidden);
	TagContainer->ClearChildren();
	StatPopup->ClearText();

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

	StatPopup->SpecificClearText(InCondition);
	ApplyFilter();
}

void UMyCategorySortWidget::RefreshList()
{
	ListView->ClearListItems();
	LoadCharacters(GenerateRows());
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
	NewTag->GetButton()->AddOnClickEvent();

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

void UMyCategorySortWidget::LoadCharacters(UDataTable* InCharacterData)
{
	if (!InCharacterData) return;

	TArray<FMyCharacterData*> AllRows;
	InCharacterData->GetAllRows(TEXT("CharacterData"), AllRows);

	for (FMyCharacterData* Row : AllRows)
	{
		UMyListRowWidget* NewRow = CreateWidget<UMyListRowWidget>(this, MyListRowWidgetClass);
		NewRow->SetData(Row);
		ListView->AddItem(NewRow);
	}
}

UDataTable* UMyCategorySortWidget::GenerateRows()
{
	UMyDataManageSubsystem* DataManageSubsystem = Cast<UGameInstance>(GetWorld()->GetGameInstance())->GetSubsystem<UMyDataManageSubsystem>();
	UDataTable* CharacterData = DataManageSubsystem->FindDataTable(TEXT("CharacterData"));

	return CharacterData;
}

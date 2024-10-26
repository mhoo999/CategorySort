// © 2024 Hoon. A touch of creativity in every line of code.


#include "CategorySort/Widget/MyCategorySortWidget.h"
#include "CategorySort/Widget/MyListRowWidget.h"

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
	
}

void UMyCategorySortWidget::GenerateRows(int32 RowCount)
{
	/*for (int32 i = 0; i < RowCount; i++)
	{
		UMyListRowWidget* NewRow = CreateWidget<UMyListRowWidget>(MyListRowWidgetClass);
	}*/
}

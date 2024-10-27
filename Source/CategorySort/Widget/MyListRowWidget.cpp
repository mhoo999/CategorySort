// © 2024 Hoon. A touch of creativity in every line of code.


#include "Widget/MyListRowWidget.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

#include "GameData/MyCharacterData.h"

void UMyListRowWidget::NativeConstruct()
{
	NormalColor = BackgroundBorder->GetBrushColor();
}

void UMyListRowWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UMyListRowWidget* ListRowWIdget = Cast<UMyListRowWidget>(ListItemObject);

	if (!ListRowWIdget) return;

	FMyCharacterData* InCharacterData = ListRowWIdget->GetCharacterData();

	if (InCharacterData)
	{
		CharacterData = InCharacterData;
		Level->SetText(FText::FromString(FString::FromInt(CharacterData->Level)));
		Name->SetText(FText::FromString(CharacterData->Name.ToString()));
		Strength->SetText(FText::FromString(FString::FromInt(CharacterData->Strength)));
		Intelligence->SetText(FText::FromString(FString::FromInt(CharacterData->Intelligence)));
		Vitality->SetText(FText::FromString(FString::FromInt(CharacterData->Vitality)));
	}

}

void UMyListRowWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	BackgroundBorder->SetBrushColor(FLinearColor(0.289125f, 0.466949f, 0.480903f, 1.f));
}

void UMyListRowWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	BackgroundBorder->SetBrushColor(NormalColor);
}

void UMyListRowWidget::SetData(FMyCharacterData* InCharacterData)
{
	if (InCharacterData)
	{
		CharacterData = InCharacterData;
		Level->SetText(FText::FromString(FString::FromInt(CharacterData->Level)));
		Name->SetText(FText::FromString(CharacterData->Name.ToString()));
		Strength->SetText(FText::FromString(FString::FromInt(CharacterData->Strength)));
		Intelligence->SetText(FText::FromString(FString::FromInt(CharacterData->Intelligence)));
		Vitality->SetText(FText::FromString(FString::FromInt(CharacterData->Vitality)));
	}
}

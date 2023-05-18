#pragma once
//#include "CoreMinimal.h"
#include "Cerberus/Items/CerberusItem.h"
#include "Templates/SubclassOf.h"
#include "Cerberus/Utilities/CerberusUtils.h"
#include "CerberusNotification.generated.h"


class UCerberusUtils;
class UCerberusItem;

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	NT_Basic UMETA(DisplayName = "Basic Notification"),
	NT_Important UMETA(DisplayName = "Important Notification"),
	NT_Loot UMETA(DisplayName = "Loot Notification"),

	NUM UMETA(Hidden)
};


USTRUCT(BlueprintType)
struct FNotification
{
	GENERATED_BODY()

public:
	FNotification() {};
	FNotification(ENotificationType InType, FText InMessage) : Type(InType), Message(InMessage) {};
	FNotification(ENotificationType InType, FText InMessage, UObject* InPackage) : Type(InType), Message(InMessage), Package(InPackage) {};
	
	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Notification")
	ENotificationType Type;

	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Notification")
	FText Message;
	
	UPROPERTY(BlueprintReadWrite, Category="Cerberus|Notification")
	UObject* Package;

	//Helper functions
	static FNotification CreateBasicNotification(FText InMessage)
	{
		FNotification Notification(ENotificationType::NT_Basic, InMessage);
		return Notification;
	}

	static FNotification CreateLootNotification(UCerberusItem* InItem)
	{
		FText Message = FText::Format(InItem->ItemDisplayName, FText::FromString(" Acquired."));
		FNotification Notification(ENotificationType::NT_Loot, Message, InItem);
		return Notification;
	}
};
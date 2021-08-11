// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All);

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Создание визуализации актора
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	SetColor(GeometryData.Color);
	//логирование
	//PrintTransform();
	//PrintStringsTypes();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryActor::OnTimerFired, GeometryData.TimerRate, true);
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
}

void ABaseGeometryActor::PrintStringsTypes()
{
	FString Name = "John Connor";
	UE_LOG(LogBaseGeometry, Display, TEXT("Name: %s"), *Name);

	FString WeaponNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
	FString IsDeadStr = "Is dead = " + FString(IsDead ? "true" : "false");
	FString Stat = FString::Printf(TEXT("\n ==All stat==\n %s \n %s \n %s"), *WeaponNumStr, *HealthStr, *IsDeadStr);
	UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *Stat);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(1.5f, 1.5f));
}

void ABaseGeometryActor::PrintTransform()
{
	//получение управления над трансформацией актора
	FTransform Transform = GetActorTransform();
	//получаем координаты актора
	FVector Location = Transform.GetLocation();
	//контроль над углами актора
	FRotator Rotator = Transform.Rotator();
	//получаем scale актора
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actro name %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actro Transform %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actro Location %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actro Rotator %s"), *Rotator.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actro Scale %s"), *Scale.ToString());
	UE_LOG(LogBaseGeometry, Error, TEXT("Actro Human Transform %s"), *Transform.ToHumanReadableString());
}

void ABaseGeometryActor::HandleMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
	{
		//движение актора по закону синуса
		// z = z0 + amplitude * sin(freq * t);
		float Time = GetWorld()->GetTimeSeconds();
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
		SetActorLocation(CurrentLocation);
	}
	break;
	case EMovementType::Static: break;
	default:break;
	}
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	const FLinearColor NewColor = FLinearColor::MakeRandomColor();
	SetColor(NewColor);
}


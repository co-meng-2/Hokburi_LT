
#include "CmTDC_CameraPawn.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACmTDC_CameraPawn::ACmTDC_CameraPawn(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;

	CmCamera = CreateDefaultSubobject<UCameraComponent>(L"Camera");
	CmCameraBoom = CreateDefaultSubobject<USpringArmComponent>(L"CameraBoom");
	CmCameraBoom->SetupAttachment(RootComponent);
	CmCamera->SetupAttachment(CmCameraBoom);

#if WITH_EDITOR
	CmTestMesh = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(L"TestMesh");
	CmTestMesh->SetupAttachment(RootComponent);
	CmCameraBoom->SetupAttachment(CmTestMesh);
#endif


}

void ACmTDC_CameraPawn::BeginPlay()
{
	Super::BeginPlay();
	CmCameraBoom->TargetArmLength = MaxLen;
	CachedLen = MaxLen;
}

void ACmTDC_CameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);


	if(bHasZoomInput)
	{
		CachedLen = FMath::Clamp(
		 	CmCameraBoom->TargetArmLength + CachedZoomDir * ZoomSpeed * DeltaTime, MinLen, MaxLen);
		bHasZoomInput = false;
	}

	if(CmCameraBoom->TargetArmLength != CachedLen)
	{
		CmCameraBoom->TargetArmLength = FMath::FInterpTo(CmCameraBoom->TargetArmLength, CachedLen, DeltaTime, ZoomSmoothSpeed);
		if(FMath::Abs(CmCameraBoom->TargetArmLength - CachedLen) <= ZoomThreshold )
		{
			CmCameraBoom->TargetArmLength = CachedLen;
		}
	}
}

void ACmTDC_CameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EIComp{Cast<UEnhancedInputComponent>(PlayerInputComponent)})
	{
		EIComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::ActionMove);
		EIComp->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ThisClass::ActionZoom);
	}
}

void ACmTDC_CameraPawn::ActionMove(const FInputActionValue& InValue)
{
	// Find the front, back, left, right direction relative to the Actor.
	CachedMoveDir = FRotationMatrix(GetActorRotation()).TransformVector(InValue.Get<FVector>());
	
	bHasMoveInput = true;
}

void ACmTDC_CameraPawn::ActionZoom(const FInputActionValue& InValue)
{
	CachedZoomDir = InValue.Get<float>();
	bHasZoomInput = true;
}

void ACmTDC_CameraPawn::Move(float DeltaTime)
{
	bool Loc_bIsMoved{false};

	FVector Loc_MoveLocation{GetActorLocation()};
	
	
	if(bHasMoveInput)
	{
		Loc_MoveLocation += CachedMoveDir * MoveSpeed * DeltaTime;
		
		Loc_bIsMoved = true;
		bHasMoveInput = false;
	}
	else if(bUseEdgeMove)
	{
		if(APlayerController* PC{Cast<APlayerController>(Controller)})
		{
			float MouseX, MouseY;
			int32 ScreenSzX, ScreenSzY;
			PC->GetMousePosition(MouseX, MouseY);
			PC->GetViewportSize(ScreenSzX, ScreenSzY);
			if(MouseY <= EdgeMargin)
			{
				Loc_MoveLocation += GetActorForwardVector() * MoveSpeed * DeltaTime;
			}
			else if(MouseY >=  ScreenSzY - EdgeMargin)
			{
				Loc_MoveLocation -= GetActorForwardVector() * MoveSpeed * DeltaTime;
			}
			
			if(MouseX <= EdgeMargin)
			{
				Loc_MoveLocation -= GetActorRightVector() * MoveSpeed * DeltaTime;
			}
			else if(MouseX >= ScreenSzX - EdgeMargin)
			{
				Loc_MoveLocation += GetActorRightVector() * MoveSpeed * DeltaTime;
			}
			Loc_bIsMoved = true;
		}	
	}

	if(Loc_bIsMoved)
	{
		SetActorLocation(Loc_MoveLocation);
		OnCameraPawnMoved.Broadcast(Loc_MoveLocation);
	}
	
}


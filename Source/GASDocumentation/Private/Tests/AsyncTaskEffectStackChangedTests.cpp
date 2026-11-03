// Copyright 2020 Dan Kestranek.

#if WITH_DEV_AUTOMATION_TESTS

#include "Characters/Abilities/AsyncTaskEffectStackChanged.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGDAsyncTaskEffectStackChangedCleanupTest,
	"GASDocumentation.AsyncTasks.EffectStackChanged.Cleanup",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FGDAsyncTaskEffectStackChangedCleanupTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	if (!TestNotNull(TEXT("Test world"), World))
	{
		return false;
	}

	AActor* Owner = World->SpawnActor<AActor>();
	if (!TestNotNull(TEXT("ASC owner"), Owner))
	{
		World->DestroyWorld(false);
		return false;
	}
	UAbilitySystemComponent* ASC = NewObject<UAbilitySystemComponent>(Owner);
	ASC->RegisterComponent();
	ASC->InitAbilityActorInfo(Owner, Owner);
	const FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));

	UGameplayEffect* Definition = NewObject<UGameplayEffect>();
	Definition->DurationPolicy = EGameplayEffectDurationType::Infinite;
	auto ApplyEffect = [&]()
	{
		FGameplayEffectSpec Spec(Definition, ASC->MakeEffectContext(), 1.0f);
		Spec.AddDynamicAssetTag(Tag);
		return ASC->ApplyGameplayEffectSpecToSelf(Spec);
	};

	// All matching effects must be unbound, not only the most recent one.
	UAsyncTaskEffectStackChanged* Task = UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange(ASC, Tag);
	const FActiveGameplayEffectHandle First = ApplyEffect();
	const FActiveGameplayEffectHandle Second = ApplyEffect();
	TestTrue(TEXT("First effect applied"), First.IsValid());
	TestTrue(TEXT("Second effect applied"), Second.IsValid());
	TestEqual(TEXT("Both effects are tracked"), Task->ActiveEffectHandles.Num(), 2);
	Task->EndTask();
	for (const FActiveGameplayEffectHandle Handle : {First, Second})
	{
		FOnActiveGameplayEffectStackChange* Delegate = ASC->OnGameplayEffectStackChangeDelegate(Handle);
		if (TestNotNull(TEXT("Live effect delegate"), Delegate))
		{
			TestFalse(TEXT("Task is unbound from every effect"), Delegate->IsBoundToObject(Task));
		}
	}
	TestFalse(TEXT("Effect-added delegate unbound"), ASC->OnActiveGameplayEffectAddedDelegateToSelf.IsBoundToObject(Task));
	TestFalse(TEXT("Effect-removed delegate unbound"), ASC->OnAnyGameplayEffectRemovedDelegate().IsBoundToObject(Task));
	Task->EndTask();
	TestEqual(TEXT("Repeated cleanup leaves no tracked effects"), Task->ActiveEffectHandles.Num(), 0);
	ASC->RemoveActiveGameplayEffect(First);
	ASC->RemoveActiveGameplayEffect(Second);

	// Normal removal clears tracking before the widget ends its task.
	Task = UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange(ASC, Tag);
	const FActiveGameplayEffectHandle Removed = ApplyEffect();
	ASC->RemoveActiveGameplayEffect(Removed);
	TestTrue(TEXT("Removed handle still reports valid"), Removed.IsValid());
	TestTrue(TEXT("Removed effect has no stack delegate"), ASC->OnGameplayEffectStackChangeDelegate(Removed) == nullptr);
	TestEqual(TEXT("Removal callback clears tracking"), Task->ActiveEffectHandles.Num(), 0);
	Task->EndTask();

	// Simulate teardown where the removal callback is no longer delivered.
	// This is the stale-handle/null-delegate condition from the PIE crash.
	Task = UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange(ASC, Tag);
	const FActiveGameplayEffectHandle Stale = ApplyEffect();
	ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(Task);
	ASC->RemoveActiveGameplayEffect(Stale);
	TestEqual(TEXT("Teardown left a stale handle"), Task->ActiveEffectHandles.Num(), 1);
	TestTrue(TEXT("Stale handle resolves to null"), ASC->OnGameplayEffectStackChangeDelegate(Stale) == nullptr);
	Task->EndTask();
	TestEqual(TEXT("Stale handle cleaned without crashing"), Task->ActiveEffectHandles.Num(), 0);
	Task->EndTask();

	TestTrue(TEXT("Invalid ASC input is handled"),
		UAsyncTaskEffectStackChanged::ListenForGameplayEffectStackChange(nullptr, Tag) == nullptr);
	World->DestroyWorld(false);
	return true;
}

#endif

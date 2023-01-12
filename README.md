# FPS combat balancing using AI Accuracy
## Introduction
Balancing difficulty in FPS games can be a very exhausting process. Without a proper balancing system, the game can feel very unfair and unpleasant to the player.

**Examples:**

* When you are spotted by multiple AI agents, you die instantly because they shoot at the same time.<br />
* You get killed by agents that are behind you while you are fighting agents in front of you.<br />


![Getting oneshot GIF](https://media.giphy.com/media/Y8XNs2nQvKC90V1Kcn/giphy.gif)



With this project, I aim to showcase one of the ways we can deal with this issue. I will be using **Unreal Engine 4** and **c++**. But before we get to the actual implementation, we need to go over some theory.

![Brace_Yourself](https://user-images.githubusercontent.com/88614889/209974892-66c1d689-6793-454b-93b1-d5de31ee77b1.jpg)


## Options
There are different ways to address the matter in question:

* Adjust the amount of damage each agent deals
* Modify damage values based on the number of agents shooting at the player simultaneously
* Make the AI agents less accurate 

The latter will be implemented in this project.
## Theory
When tweaking the AI accuracy, we must make sure that the behavior is believable. Agents in front of us missing most of their shots are not very convincing.

To have believable behavior we need to solve two problems:
* Which agents should hit and which ones should miss? 
* What is the delay between each time the player should get hit?
### Token System
In order to determine which agent gets to hit their shot, we will use a logical token. By default, all the agents will miss their shots, except the one with the token. The latter will hit their shot and release the token, which will be distributed again.


![AgentWithToken](https://user-images.githubusercontent.com/88614889/209975787-1ca6e759-65e6-4fb3-9e9f-76e79caec66e.png)


In our logic we would have:

* Code that determines which agent is the most relevant to get the token 
* Code that calculates when this agent should get the token (in seconds), thus when should the bullet hit the player (dynamic timer)


To simplify further explanation I will use a single AI. In this case we can ignore the logic that will determine which AI is the most relevant and we can focus on the delay calculation for a single AI. Understanding how to deal with a single agent will enable us to deal with multiple agents without difficulties since both concepts use similar techniques.
## Dealing With Single AI

### Delay between hits
How can we calculate the delay between hits, a.k.a when should an agent hit us? If we use a constant delay between each hit, the player might find out that the shots hit them every x second, which is not believable behavior. But we also don't want randomness since it can create weird situations. We want to make this delay dynamic, which gets us the best of both worlds. So we'll use a formula to calculate this final delay.

`finalDelay = baseDelay * multipliers`

### Rules and Multipliers
Multipliers are floating point values that are defined by us using Rules. Our implementation guarantees that every multiplier can adjust its value depending on the scenario. 

<ins>**Example**:</ins> 

We want our AI to hit more shots when using the Super weapon, miss most of the shots when using the Old weapon and we don't want to affect the timer when utilizing the Normal weapon. We can define a Weapon Rule, which will look like this:


![ExampleRule](https://user-images.githubusercontent.com/88614889/209980519-5cef0b6c-483b-4952-8ed2-3ec8d67a99bc.png)

Let's define a base delay and explore every scenario.

`float baseDelay = .5f`


`finalDelay = baseDelay * weaponMultiplier`

**Scenario 1: AI is using the Old weapon**

```
weaponMultiplier = 2.f

finalDelay = .5f * 2.f

finalDelay = 1.f
```

This means that when our AI is using the Old weapon, he is going to hit us **every 1 second**  


**Scenario 2: AI is using the Super weapon**

```
weaponMultiplier = .5f

finalDelay = .5f * .5f

finalDelay = .25f
```

We are gonna get hit more frequently if the AI is using the Super weapon (every 0.25s)

With the explanation out of the way, I am going to define some actual rules and multipliers for this project.


### Rule 1: Distance
**Goal**

* Make the player feel more pressured when AI is close

**Definition:**

* Any distance closer than 5m will halve the delay
* Any distance greater than 40m will double the delay
* Linear interpolation in between

**Graph**

![DistanceMultiplier](https://user-images.githubusercontent.com/88614889/209985384-fe486011-322e-4521-97ea-189fc260d96f.png)

### Rule 2: Player stance
**Goal**

* Make the player feel safer while crouching

**Definition:**

* When the player is crouching, double the delay
* When the player is standing, do not affect the delay

**Graph**

![StanceMultiplier](https://user-images.githubusercontent.com/88614889/210240189-85397fde-a288-48ff-b593-53f3b38da5e5.png)

### Rule 3: Player velocity
**Goal**

* Punish the player when running toward the AI
* More forgiving behavior when running away from the AI

**Graph**

![PlayerDirChart](https://user-images.githubusercontent.com/88614889/210240634-2ab7922e-f3fe-4a2d-8c3c-f21490bc73f9.png)

## Implementation
### Start Project
As I mentioned before, I am going to use Unreal Engine 4. The First Person template is a good starting point, but we need more features to get started with the project.

![image](https://user-images.githubusercontent.com/88614889/210248819-f2c44074-195d-48d9-91c9-58e4ccc2a409.png )


I added the following functionality to the First Person template:
* Enemy class
  * Basic Behavior Tree with Combat and Idle states
  * AI perception
  * Shooting logic
* Player
  * Crouching
  * Health component
  * Player feeback when taking damage (blood screen overlay)
* HUD
  * Player health
  * Player stance

### Approach
Before we start programming, we need to think about our approach. How do we want to represent our Rules inside Unreal? More specifically, how do we link the values to the multipliers?

We could use a [TMap](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TMap/) to join values with multipliers. This is a promising approach, but there is one issue. It would fully depend on c++. This is not a problem for us, the programmers, but we want a system that is easy to use for everyone. 

### Data Tables
Data tables are a way to group related data in a meaningful way. It provides us with the functionality that the TMap implementation would provide, but there is more. We can write code that would interpret the data table at any given time, enabling the designers to add/remove multipliers and tweak values without interacting with c++.

For more information regarding Data Tables in Unreal, I refer to the official [documentation](https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/DataDriven/). This is what a Data Table inside Unreal looks like:

![image](https://user-images.githubusercontent.com/88614889/210260610-e7f10592-fd81-4675-80e4-c5a2996f81c2.png)


How will we use data tables?

* We'll define Rule structs in code
* The structs can be interpreted as 1 row inside our data table.
* We'll add the necessary data from the engine (no c++)
* We will read the data from the table when needed (c++)

![datatable-datatable-everywhere](https://user-images.githubusercontent.com/88614889/210256970-3c1e81db-ea3e-47f1-8945-96bdbb305368.jpg)

Time to embrace c++.

### Defining Rule Types
<ins>**Introduction**</ins>

I start by defining 2 structs that represent 2 rule types in *RuleTypes.h*. This is an empty c++ class.

So far every rule that I've encountered uses one of the following approaches:
* Multiplier linked to string (Stance, Weapon Type, Weather etc.)
* Multiplier linked to float value (Distance, Health, Player Facing Angle etc.)

The structs will hold data related to their specific types. 

<ins>**Code**</ins>

`RuleTypes.h`

```
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RuleTypes.generated.h"


USTRUCT(BlueprintType)
struct AI_ACCURACY_API FStringRule : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Value;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString Description;

	UPROPERTY(BlueprintReadOnly)
	bool isValid;
};

USTRUCT(BlueprintType)
struct AI_ACCURACY_API FValueRule : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Multiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;

	FORCEINLINE bool operator<(const FValueRule& other) const
	{
		return Multiplier < other.Multiplier;
	}
};
```

<ins>**Explanation**</ins>

FValueRule:

* This type can be used for any rule that has numerical values connected to their multipliers
* Interpolation between multipliers possible
* In our case, Distance Rule can return any multiplier between 0.5f and 40.f, depending on input 

FStringRule:

* This type can be used by any rule that involves multipliers being linked with non-numerical values
* No interpolation between multipliers, can only return the multipliers defined in the data table
* In our case, Stance rule can return either 1 or 2, nothing in between, hence it is a String Rule

Both structs have a similar structure. Two notable differences are the `isValid` boolean and the `operator<`, but we can ignore them for now. `Multiplier` and `Value` will be used to determine our logic, so they are essential in our structs. Description is an optional variable to provide more clarity.

## Creating Data Tables

Creating a data table in unreal is simple.

![HowToCreateDataTables](https://user-images.githubusercontent.com/88614889/210407232-b03e861a-8e53-4325-8838-c0dfd4d665ce.gif)

Inside the `RuleTypes.h` the structs inherit from `FTableRowBase`. There is a reason for this. 

When creating a data table inside Unreal, we must specify a *Row Structure.* Unreal has pre-defined row structures that can be used. But if we want our custom made one, our struct must inherit from `FTableRowBase`.


![RowStructureChoose](https://user-images.githubusercontent.com/88614889/210408751-11f5a44f-39ad-4de8-b26f-7a941c7816f1.png)

Our 3 Rule data tables are the following:

![VelRuleTable](https://user-images.githubusercontent.com/88614889/210410088-f67635c6-b465-47b2-8038-8ddb7cc2bea9.png)


![DistanceRuleTable](https://user-images.githubusercontent.com/88614889/210410098-e1e16803-2909-4b2c-9adc-0569612ec579.png)


![stanceRuleTable](https://user-images.githubusercontent.com/88614889/210410104-67cb4373-3d4e-4ddb-a4d0-9cb34bada635.png)

As we can see, the column names correspond with the variables we created inside the structs. This is essential to understand, since this enables us to expose more variables to the designers by simply adding them inside our structs from c++.


### Creating Rule Manager
<ins>**Introduction**</ins>

We want to have a dedicated class that manages every current and future rule of our game.

This class:

* Derrives from UObject
* Creates / Destroys rules
* Links rules with their data tables
* Reads / Writes to data tables
* Provides information to outside classes

<ins>**Code**</ins>

`RuleManager.h`

```
UCLASS()
class AI_ACCURACY_API URuleManager : public UObject
{
	GENERATED_BODY()

public:
	URuleManager();

	/**Pointer to DistanceTable created in the engine*/
	UPROPERTY()
	UDataTable* V_DistanceRuleTable;

	/**Pointer to StanceTable created in the engine*/
	UPROPERTY()
	UDataTable* S_StanceRuleTable;

	/**Pointer to VelocityTable created in the engine*/
	UPROPERTY()
	UDataTable* V_VelocityRuleTable;
private:
	/**Arrays to cache table content for later use*/
	TArray<FValueRule*> DistanceRuleArr;
	TArray<FValueRule*> DirectionRuleArr;
	TArray<FStringRule*> StanceRuleArr;
};

```

First off, we want to be able to access the data tables created inside the engine from the code. So we create pointers to those data tables. Then, we don't want to read the data every time we need it, so we read all the necessary content in the constructor and store it inside TArrays.

`RuleManager.cpp`
```
URuleManager::URuleManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DistanceTable(TEXT("DataTable'/Game/DataTables/DistanceRuleTable.DistanceRuleTable'"));
	if (DistanceTable.Succeeded())
	{
		V_DistanceRuleTable = DistanceTable.Object;

		//Cache the table in array and pre sort
		const FString context{ TEXT("Distance Rule Array Context") };
		V_DistanceRuleTable->GetAllRows(context, DistanceRuleArr);
		DistanceRuleArr.Sort();
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StanceTable(TEXT("DataTable'/Game/DataTables/StanceRuleTable.StanceRuleTable'"));
	if (StanceTable.Succeeded())
	{
		S_StanceRuleTable = StanceTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> VelTable(TEXT("DataTable'/Game/DataTables/VelocityRuleTable.VelocityRuleTable'"));
	if (VelTable.Succeeded())
	{
		V_VelocityRuleTable = VelTable.Object;

		//Cache the table in array and pre sort
		const FString context{ TEXT("Velocity Rule Array Context") };
		V_VelocityRuleTable->GetAllRows(context, VelocityRuleArr);
		VelocityRuleArr.Sort();
	}
}
```

As you can see, we don't cache the content of our String Rule yet. That is because there is plenty of room for errors when inputting data inside a string rule table.
Since the value type inside a string rule is an `FString`, the user can input any text. Even the slightest typo can cause our game to break. So we want to validate the data inside a string rule before copying them to our local `TArray`. We also want to give the user some feedback in case they made a mistake. We will use a separate function for this.

`RuleManager.h`
```
void ValidateStringTableStance();
```

`RuleManager.cpp`
```
void URuleManager::ValidateStringTableStance()
{
	//Read all the data from the given table
	const FString context{ TEXT("Stance rule context")};
	TArray<FStringRule*> outRowArr;
	S_StanceRuleTable->GetAllRows(context, outRowArr);

	//Validate every row inside the data table
	for (int i = 0; i < outRowArr.Num(); i++)
	{
		outRowArr[i]->Description = "INVALID VALUE";
		outRowArr[i]->isValid = false;

		//Custom enum class that keeps information about player stances
		for (ECharacterStance stance : TEnumRange<ECharacterStance>())
		{
			if (outRowArr[i]->Value == UEnum::GetDisplayValueAsText(stance).ToString())
			{
				outRowArr[i]->isValid = true;
				outRowArr[i]->Description = "VALID VALUE";
				break;
			}
		}
	}

	//Copy the validated data to our local array
	StanceRuleArr = outRowArr;
}

```

It is important to note that the validation will only work if the Row Names inside our data tables correspond with the names inside our Enumeration that keeps track of the player stances.

In this project I have defined the following enum:

```
UENUM()
enum class ECharacterStance : uint8
{
	Crouching,
	Standing,

	//To iterate over enum elements
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ECharacterStance, ECharacterStance::Count);
```

Let's see what happens if we add a stance to our table that doesn't exist.


![image](https://user-images.githubusercontent.com/88614889/210423639-ac72769c-cfd3-4b8f-841d-b8b5a0a89053.png)


After compiling, our code validates the first two stances (which are present inside our enum), but overrides the Flying stance description since it is not valid.

Now let's add Flying to our enum class and see what happens.
```
UENUM()
enum class ECharacterStance : uint8
{
	Crouching,
	Standing,
	Flying,

	//To iterate over enum elements
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ECharacterStance, ECharacterStance::Count);
```

![image](https://user-images.githubusercontent.com/88614889/210421775-5d37d1c1-29ef-4b9b-947b-efeb78702927.png)

As we can see, flying gets recognized. This means that the corresponding multiplier can be used.

### Reading data: String Rule Tables
We know that String Rule Tables are about exact values. The player is either crouching or standing, with no in-between. So we don't have to interpolate between multipliers.

With that being said, defining a function that returns a multiplier from a String Rule Table is straightforward. 

**Code**

`RuleManager.h`
```
float GetMultiplierFromString(const EStringRule& valueRule, const FString& value);
```
```
UENUM()
enum class EStringRule : uint8
{
	//Existing string rules
	StanceRule,
};
```


`RuleManager.cpp`
```
float URuleManager::GetMultiplierFromString(const EStringRule& valueRule, const FString& value)
{
	//Get the row based on the RowName
	const FString context{ TEXT("Find multiplier from string") };
	FStringRule* row = nullptr;

	switch (valueRule)
	{
	case EStringRule::StanceRule:
		row = S_StanceRuleTable->FindRow<FStringRule>(FName(*value), context);
		break;
	}

	//If the row exists
	if (row)
	{
		//If the content of the row is valid
		if (row->isValid)
			return row->Multiplier;
	}

	//If invalid row then we want this multiplayer to have no effect;
	return 1.f;
}
```
**Explanation**

In the previous section, we cached the contents of our Data Tables. It is crucial because the functions that return multipliers get called every frame. So we don't want to read the data from the table every frame. It can cause some performance issues. Instead, I have defined a struct that holds every String Rule type. When we call the `GetMultiplierFromString` function, we need to specify which rule we want to use. This way we can read data from the right `TArray`, which is already cached.  

### Reading data: Value Rule Tables
Value rules are more complex. If we get a value that is between 2 predefined values, we don't know what our exact multiplier is.

**Example**

* In our Velocity Rule, we get a 92° angle
* How do we determine the multiplier?

![ValueRuleExplanation](https://user-images.githubusercontent.com/88614889/211813463-9cbc9f7e-d02b-4cda-ab8a-aec322f7e64c.png)

We will use the *Equation of a line through two points* to interpolate between 2 predefined values. Explanation can be found [here](https://www.bbc.co.uk/bitesize/guides/z9387p3/revision/5#:~:text=The%20straight%20line%20through%20two,coordinates%20of%20the%20two%20points).

The formula is the following:

`y = mx + c `, 

where `y` is the multiplier we want to return, `x` is the value we get (92°), `m`is the *gradient* of the line and `c` is the y-intercept.

To find `m` and `c` we use the following formulas:

`m = (y2 - y1 ) / (x2 - x1)`

`c = y2 - m * x2`

With that knowledge, all we need to know is find the neighboring values of the input value. In our example the left value of `92°` is `90°`, the right value is `135°`.
Since our value arrays are sorted, we can find the neigbhors with a single *for loop*

``` 
	float leftBorderIdx{};
	float rightBorderIdx{};

	//Go over every element of table (cached array)
	for (int i = 0; i < outRowArrPtr->Num(); ++i)
	{
		//Get the left element index 
		if ((*outRowArrPtr)[i]->Value <= value)
		{
			leftBorderIdx = i;
		}

		//get the right element index
		if ((*outRowArrPtr)[outRowArrPtr->Num() - 1 - i]->Value >= value)
		{
			rightBorderIdx = outRowArrPtr->Num() - 1 - i;
		}
	}
```

Then, after finding our left and right values, we can plug them into the formulas and find our exact multiplier.

```
// Formula for in between values y = mx + c
	float m = ((*outRowArrPtr)[rightBorderIdx]->Multiplier - (*outRowArrPtr)[leftBorderIdx]->Multiplier) / ((*outRowArrPtr)[rightBorderIdx]->Value - (*outRowArrPtr)[leftBorderIdx]->Value);
	float c = (*outRowArrPtr)[rightBorderIdx]->Multiplier - m * (*outRowArrPtr)[rightBorderIdx]->Value;

	return m * value + c;
```

The complete function looks like this:

`RuleManager.cpp`

```
float URuleManager::GetMultiplier(const EValueRule& valueRule, float value)
{
	//avoids copying the array
	TArray<FValueRule*>* outRowArrPtr = nullptr;

	switch (valueRule)
	{
	case EValueRule::DistanceRule:
		outRowArrPtr = &DistanceRuleArr;
		break;
	case EValueRule::VelocityRule:
		outRowArrPtr = &VelocityRuleArr;
		break;
	}
		
	if (!outRowArrPtr)
		return 1.f;

	//Smallest and biggest indexes (array sorted)
	int smallest{ 0 };
	int biggest{ outRowArrPtr->Num() - 1 };

	//If our value is less than our smallest value, we return the smallest multiplier
	if (value <= (*outRowArrPtr)[smallest]->Value)
		return (*outRowArrPtr)[smallest]->Multiplier;
	
	
	//If our value is bigger than our biggest value, we return the biggest multiplier
	if (value >= (*outRowArrPtr)[biggest]->Value)
		return (*outRowArrPtr)[biggest]->Multiplier;
	

	float leftBorderIdx{};
	float rightBorderIdx{};

	//Go over every element of table (cached array)
	for (int i = 0; i < outRowArrPtr->Num(); ++i)
	{
		//If we have the exact value in our array, we stop and return the corresponding multiplier
		if (UKismetMathLibrary::EqualEqual_FloatFloat((*outRowArrPtr)[i]->Value,value))
		{
			return (*outRowArrPtr)[i]->Multiplier;
		}


		//Get the left element index 
		if ((*outRowArrPtr)[i]->Value <= value)
		{
			leftBorderIdx = i;
		}

		//get the right element index
		if ((*outRowArrPtr)[outRowArrPtr->Num() - 1 - i]->Value >= value)
		{
			rightBorderIdx = outRowArrPtr->Num() - 1 - i;
		}
	}

	//If we made it so far, the value is between 2 other values
	//https://www.bbc.co.uk/bitesize/guides/z9387p3/revision/5#:~:text=The%20straight%20line%20through%20two,coordinates%20of%20the%20two%20points.
	// Formula for in between values y = mx + c
	float m = ((*outRowArrPtr)[rightBorderIdx]->Multiplier - (*outRowArrPtr)[leftBorderIdx]->Multiplier) / ((*outRowArrPtr)[rightBorderIdx]->Value - (*outRowArrPtr)[leftBorderIdx]->Value);
	float c = (*outRowArrPtr)[rightBorderIdx]->Multiplier - m * (*outRowArrPtr)[rightBorderIdx]->Value;

	return m * value + c;

}
```

Great, now we can also read from a *Value Table*. Time to calculate the final delay for our AI agent.

![its-finally-over-597a15](https://user-images.githubusercontent.com/88614889/211821018-00dc2e3a-2c16-46a3-9530-2233bb6cdbf4.jpg)

## Putting it all together
The hardest part of the project is done. Now we need to calculate the values and plug them into our function to get the correct multipliers.

**Stance Rule Value**

Getting the value for our stance rule is simple. Since we have the data stored in our *Player Character*, we can get it using a reference to our player.
```
	FString stanceValue = UEnum::GetDisplayValueAsText(PlayerCharRef->GetCharacterStance()).ToString();
```

**Distance Rule Value**

Just like the name suggests, we need to find the current distance from our player to our AI. We can use [FVector::Distance](https://docs.unrealengine.com/4.26/en-US/API/Runtime/Core/Math/FVector/Distance/).

```
	//Calculate distance value to plug into GetMultiplier
	float distanceValue = FVector::Distance(ControlledPawnRef->GetActorLocation(), PlayerCharRef->GetActorLocation());

	//Convert distance to meters (table is defined in meters)
	distanceValue = FUnitConversion::Convert(distanceValue, EUnit::Centimeters, EUnit::Meters);
```

**Velocity Rule Value**

For this value, we need the player's velocity vector and the Player-AI vector. Using the [UKismetMathLibrary](https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Kismet/UKismetMathLibrary/) we can calculate the angle difference.

```
	//Get player velocity vec and Player-AI vec
	FVector vec1 = PlayerCharRef->GetVelocity();
	FVector vec2 = ControlledPawnRef->GetActorLocation() - PlayerCharRef->GetActorLocation();
		
	//Normalize for correct calculation
	vec1.Normalize();
	vec2.Normalize();

	float angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(vec1, vec2));

	//This value should have effect if the player is actually moving
	angle = angle == 0.f ? 1.0f : angle;
```

### Final delay calculation

For this calculation we use the functions that we made inside our *RuleManager*.

```
	//Calculate multipliers
	float distanceMultiplier = RuleManager->GetMultiplier(EValueRule::DistanceRule, distanceValue);
	float stanceMultiplier = RuleManager->GetMultiplierFromString(EStringRule::StanceRule, UEnum::GetDisplayValueAsText(PlayerCharRef->GetCharacterStance()).ToString());
	float velocityMultiplier = RuleManager->GetMultiplier(EValueRule::VelocityRule, angle);
		
	//Calculate final delay
	float finalDelay = baseDelay * distanceMultiplier * stanceMultiplier * velocityMultiplier;
```

Then, to determine when our AI should hit the player, we can manipulate a boolean based on our timer. 

```
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentDelay += DeltaTime;
	
	if (currentDelay >= finalDelay)
	{
		shouldHit = true;
		TimerWidget->SetHitMissText(true);
		currentDelay = 0.f;	
	}
}
```

When `shouldHit` is false, we add an offset to the *FRotator* used to spawn the bullet. This way the bullet will miss the player. When `shouldHit` is true, no offset is added and the bullet will fly towards the player.

`const FRotator randomRotation = shouldHit ? FRotator{ 0.f,0.f,0.f } : FRotator{ 0.f,10.f,0.f }; `

**Shoot function**

```
void AEnemy::Shoot()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator randomRotation = shouldHit ? FRotator{ 0.f,0.f,0.f } : FRotator{ 0.f,10.f,0.f };
			const FRotator SpawnRotation = GetControlRotation() + randomRotation;

			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			
			//If we hit the last shot, we reset this boolean
			if (shouldHit)
			{
				shouldHit = false;
			}
		}
}	}
```

## Final Result: Single AI

Our current implementation adds the following to the player experience:

* Safer when crouching
* Punishing when running towards the AI
* Forgiving when running away from the AI
* More challenging combat when closer to the AI
* Indulgent combat difficulty when far from the AI

![DistanceRulGIF](https://github.com/hovo-hakobyan/FPS-AI-Combat-Difficulty/blob/main/GIF/DistanceRule.gif)

![StanceRuleGIF](https://github.com/hovo-hakobyan/FPS-AI-Combat-Difficulty/blob/main/GIF/StanceRule.gif)


## Future

So far our implementation deals perfectly with a single AI. But in games, there are often multiple enemies shooting at the player. 

![FinalGIF](https://github.com/hovo-hakobyan/FPS-AI-Combat-Difficulty/blob/main/GIF/Final.gif)

Implementing a logical token on top of everything we did so far will also solve the problem for multiple AI agents. But that is meant for the future :). Thanks for reading!


## Acknowledgements and Webgraphy

[AI for dynamic difficulty adjustment in games](https://www.researchgate.net/publication/228889029_AI_for_dynamic_difficulty_adjustment_in_games)  by Robin Hunicke

[Using AI Accuracy to Balance Difficulty](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter33_Using_Your_Combat_AI_Accuracy_to_Balance_Difficulty.pdf) by Sergio Ocio Barriales

[UE Programming with c++](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/)

[Blueprint to C++ Series, Mike Stevanovic](https://youtube.com/playlist?list=PLY9cHlxw3OgjKmwaTEekW3sr_oHY-HwGv)

[UE C++ Fundamentals, Jolly Monster Studios](https://youtube.com/playlist?list=PLCeaAi_Ah78SXMH-qUxjeEz35reRWduKh)

[UE C++ Training Series, Normalish Software](https://youtube.com/playlist?list=PLa8-f-Rrm0Nkmd1RELB8XTRGWwermTkxE)

[UE C++ AI Series, MrCxx](https://youtube.com/playlist?list=PLWUvrI0mg8VLrvq3n2iu45lhCo69uluDK)

[Straight line graphs](https://www.bbc.co.uk/bitesize/guides/z9387p3/revision/5#:~:text=The%20straight%20line%20through%20two,coordinates%20of%20the%20two%20points)

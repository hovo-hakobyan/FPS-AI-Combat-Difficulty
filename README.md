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


To simplify further explanation I will use a single AI. In this case we can ignore the logic that will determine which AI is the most relevant and we can focus on the delay calculation for a single AI. I will show how to deal with multiple AI agents in the upcoming sections.
## Dealing With Single AI

### Delay between hits
How can we calculate the delay between hits, a.k.a when should an agent get the token? If we use a constant delay between each hit, the player might find out that the shots hit them every x second, which is not believable behavior. We want to make this delay dynamic, so we'll use a formula to calculate the final delay.

`finalDelay = baseDelay * multipliers`

### What are Rules and Multipliers
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

### Rule 3: Player direction
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

When defining a struct that is going to be used inside a Data Table, it needs to inherit from `FTableRowBase`. 

FValueRule:

* This type can be used for any rule that has numerical values connected to their multipliers
* Interpolation between multipliers possible
* In our case, Distance Rule can return any multiplier between 0.5f and 40.f, depending on input 

FStringRule:

* This type can be used by any rule that involves multipliers being linked with non-numerical values
* No interpolation between multipliers, can only return the multipliers defined in the data table
* In our case, Stance rule can return either 1 or 2, nothing in between, hence it is a String Rule

Both structs have a similar structure. Two notable differences are the `isValid` boolean and the `operator<`, but we can ignore them for now. `Multiplier` and `Value` will be used to determine our logic, so they are essential in our structs. Description is an optional variable to provide more clarity.

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
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RuleManager.generated.h"

class UDataTable;

UCLASS()
class AI_ACCURACY_API URuleManager : public UObject
{
	GENERATED_BODY()

public:
	URuleManager();

	UPROPERTY()
	UDataTable* V_DistanceRuleTable;
	UPROPERTY()
	UDataTable* S_StanceRuleTable;
	UPROPERTY()
	UDataTable* V_DirectionRuleTable;	

	UDataTable* GetDistanceRuleTable() { return V_DistanceRuleTable; }
	UDataTable* GetStanceRuleTable() { return S_StanceRuleTable; }
	UDataTable* GetDirectionRuleTable() { return V_DirectionRuleTable; }
};

```

`RuleManager.cpp`
```
#include "RuleManager.h"
#include "RuleTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI_AccuracyCharacter.h"

URuleManager::URuleManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DistanceTable(TEXT("DataTable'/Game/DataTables/DistanceRuleTable.DistanceRuleTable'"));
	if (DistanceTable.Succeeded())
	{
		V_DistanceRuleTable = DistanceTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> StanceTable(TEXT("DataTable'/Game/DataTables/StanceRuleTable.StanceRuleTable'"));
	if (StanceTable.Succeeded())
	{
		S_StanceRuleTable = StanceTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DirTable(TEXT("DataTable'/Game/DataTables/PlayerDirTable.PlayerDirTable'"));
	if (DirTable.Succeeded())
	{
		V_DirectionRuleTable = DirTable.Object;
	}
}
```

<ins>**Explanation**</ins>

So far the functionality of our RuleManager is fairly simple. We create 3 `UDataTable` objects using the `FObjectFinder` function that the framework provides and link them to the Data Tables that we created in the editor.

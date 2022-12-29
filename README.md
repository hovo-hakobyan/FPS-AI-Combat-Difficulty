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
### Token system
In order to determine which agent gets to hit their shot, we will use a logical token. By default, all the agents will miss their shots, except the one with the token. The latter will hit their shot and release the token, which will be distributed again.


![AgentWithToken](https://user-images.githubusercontent.com/88614889/209975787-1ca6e759-65e6-4fb3-9e9f-76e79caec66e.png)


In our logic we would have:

* Code that determines which agent is the most relevant to get the token 
* Code that calculates when this agent should get the token (in seconds), thus when should the bullet hit the player (dynamic timer)


To simplify further explanation I will use a single AI. In this case we can ignore the logic that will determine which AI is the most relevant and we can focus on the delay calculation for a single AI. I will show how to deal with multiple AI agents in the upcoming sections.
## Dealing with single AI

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

With the explanation out of the way, I am going to define some rules and multipliers for this project.


### Rule 1: Distance
**Goal**

* Make the player feel more pressured when AI is close

**Definition:**

* Any distance closer than 5m will halve the delay
* Any distance greater than 40m will double the delay
* Linear interpolation in between

**Graph**

![DistanceMultiplier](https://user-images.githubusercontent.com/88614889/209985384-fe486011-322e-4521-97ea-189fc260d96f.png)

### Implementation

# FPS combat balancing using AI Accuracy
## Introduction
Balancing difficulty in FPS games can be a very exhausting process. Without a proper balancing system, the game can feel very unfair and unpleasant to the player.

**Examples:**

* When you are spotted by multiple AI agents, you die instantly because they shoot at the same time.<br />
* You get killed by agents that are behind you while you are fighting agents in front of you.<br />

With this project, I aim to showcase one of the ways we can deal with this issue. I will be using **Unreal Engine 4** and **c++**.
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

In our logic we would have:

* Code that determines which agent is the most relevant to get the token 
* Code that calculates when this agent should get the token (in seconds), thus when should the bullet hit the player (dynamic timer)


To simplify further explanation I will use a single AI. In this case we can ignore the logic that will determine which AI is the most relevant and we can focus on the delay calculation for a single AI. I will show how to deal with multiple AI agents in the upcoming sections.
## Dealing with single AI

### Delay between hits
How can we calculate the delay between hits, a.k.a when should an agent get the token? If we use a constant delay between each hit, the player might find out that the shots hit them every x second, which is not believable behavior. We want to make this delay dynamic, so we'll use a formula to calculate the final delay.

`finalDelay = baseDelay * multipliers`



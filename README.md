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

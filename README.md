# Human VS Zombie Opitmization

## Description
Optimization on the problem of fighting between human vs zombies using framework on C++ language. Optimization on this project were utilized using Improved Harmony Search from PaGMO library. This project was create within 2 days (first day for environment setting and another day for coding) to reskill my C++ programming skill.

## Problem Description
This is turn-based gameplay which were played between 2 groups, human and zombie. On initialization, human will have HP=100 including with AK-47 gun for 30 ammos and unlimited ammo magazines, and also unlimited flash grenade. While zombie only contains HP=999.

For each turn, human will play first then zombie. Available actions for human each turn is lists below
- "Shoot That ZOMBIE" using 1 ammo of AK-47, take 29 damages to zombies (zombie_HP-=29, ammo-=1).
- "Reload AK-47", ammo were reset to 30 and unable to play for 2 turns (ammo=30, human_stun=2).
- "Flash Grenade" to zombie, zombie unable to action for 3 turns (zombie_stun=3).
- "Regenerated HP", human will get 50 extra HP and unable to play for 1 turn (human_HP+=50, human_stun=1).

And available action for zombie is listed below.
- Attack, take 4 damages to human (human_HP-=4).

However, if human or zombie were stun on that turn, just decrease counter by one (human_stun-=1 or zombie_stun-=1).

In this optimization, least count of turns that can killed the zombie were optimized within constraints that zombie should be clear including human_hp which more than 0.

## How to run it
Simply run the following script

`g++ -O2 -DNDEBUG -std=c++17 main.cpp -pthread -lpagmo -lboost_serialization -ltbb`

## Where is my output
Output were simply saved as "output.txt".

## Contributor
Kawin Chinpong (Github: kawin7538)

## PS
I knew that this problem might be solved using some dynamic programming or just greedy method.
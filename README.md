# Puppycam for SM64

Puppycam is a brand new camera mode for SM64, designed from the ground up, to improve and fix all of the existing flaws with the horribly dated camera, that comes with the original game.

# Usage

You will want to use [this repo as a base](https://github.com/sm64-port/sm64-port) and then apply the patch in WSL. Simply copy puppycam.patch to the enhancements folder, then in the root directory of your repo, run "tools/apply_patch.sh enhancements/puppycam.patch" To revert, type "tools/revert_patch.sh enhancements/puppycam.patch" instead. This repo still supports everything necessary to develop for N64, so there's absolutely nothing lost abandoning the old one, going forward.

# Features:

- It features movement on both the X and Y axis, as well as accurate raycasting collision that prevents the camera from ever getting stuck, while retaining its absolute formost in staying within the level boundaries.
- It also features a whole range of different behavioural flags that dictate how the camera itself will operate, based on what you decide to set.
- It has a dynamic options menu, that is incredibly easy to add, or take away options for.
- Support for a player 2 joystick, for emulator users, to allow for analogue camera control.

# Screenshots:

![Screenshot 1](https://i.imgur.com/NJHhyyd.png)
![Screenshot 2](https://i.imgur.com/TmELc4d.png)

# Credits:

- Me, I made the thing, lmao
- [CuckyDev](https://github.com/cuckydev) for making the raycasting script that powers Puppycam's collision system.
- [The Clever Folks™](https://github.com/n64decomp/sm64) who made the decompilation project happen to begin with

SM64 Decomp Discord: https://discord.gg/DuYH3Fh

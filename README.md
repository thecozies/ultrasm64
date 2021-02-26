# Puppy Camera 2 for SM64

The long awaited sequel to the critically acclaimed (allegedly) Puppy Camera is here. Rewritten from the ground up, this camera aims to improve on the flaws underneath in the code the original had. The original Puppycam was written when I was still learning a lot of things in C, and I missed out on some basic things. Puppycam2 is (in my opinion) a much better codebase than the original, being completely unrecognisable from what it once was.

From the outside perspective, it's basically the same though.

Puppycam is a brand new camera mode for SM64, designed from the ground up, to improve and fix all of the existing flaws with the horribly dated camera, that comes with the original game.

I used [this repo as a base](https://github.com/CrashOveride95/ultrasm64) for this modification, however, with the changes made, this should integrate seamlessly into any other repo. 

# Features:

- It features movement on both the X and Y axis, as well as accurate raycasting collision that prevents the camera from ever getting stuck, while retaining its absolute formost in staying within the level boundaries.
- It also features a whole range of different behavioural flags that dictate how the camera itself will operate, based on what you decide to set.
- It has a dynamic options menu, that is incredibly easy to add, or take away options for.
- Support for a player 2 joystick, for emulator users, to allow for analogue camera control.

# To Do and future updates:
- Add an option for changing the input preset to match with Return to Yoshi's Island because I can't believe I didn't think of that sooner.
- Cutscenes. A lot of the system's already done, as it was written for Puppycam 1, but it does 
- Water camera improvements. The initial implimentation is functional, but not perfect yet.

# Screenshots:

![Screenshot 1](https://i.gyazo.com/a92aaf930ffe6fe839b87e7cdc02a508.jpg)
(More screenshots coming soon)

# Credits:

- Me, I made the thing, lmao
- [CuckyDev](https://github.com/cuckydev) for making the raycasting script that powers Puppycam's collision system.
- [The Clever Folks™](https://github.com/n64decomp/sm64) who made the decompilation project happen to begin with
- Those that made fixing, improving and interating on this project way easier. (and also sometimes harder >:( )

SM64 Decomp Discord: https://discord.gg/DuYH3Fh

Feel free to make pull requests

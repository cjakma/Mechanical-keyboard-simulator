[**Mechanical-keyboard-simulator is now reviewed and available on Softpedia!**](http://www.softpedia.com/get/Others/Miscellaneous/Mechanical-Keyboard-Simulator.shtml) 

# Mechanical-keyboard-simulator
If you're anything like me, you are fond of the sound of your own keyboard, especially while doing some serious coding. The thing is, it is also in my habit to put on headphones and play loud tunes whenever I program, even more so if I'm doing some tedious refactoring. Then, quite obviously, I cannot at all hear my own keystrokes anymore. Why not have the best of both worlds?

That is why I wrote a configurable Windows daemon that plays a spatialized sound every time you press and release a keyboard button.
The code uses [**Hypersomnia**](https://github.com/TeamHypersomnia/Hypersomnia) codebase (```augs/``` and ```3rdparty/```) to play sounds and perform various file operations.
Thus, the reason I've chosen AGPL-3.0 for this project is because [**Hypersomnia**](https://github.com/TeamHypersomnia/Hypersomnia) itself uses AGPL-3.0.

Advantages over the commonly found simulators:
- Both keydown and keyup events, and optionally mouse clicks, are handled.
- Unlimited amount of sound sources. Other simulators abruptly mute the currently playing sound if it does not finish playing before the next keystroke.
- Full spatialization of the sounds based on the actual positions of keys on the keyboard, or custom values.
- Ability to use HRTF.
- Ability to choose the output device.
- Ability to add custom sounds.

# How to build
Just download the repository, open .sln file in Visual Studio 2017 and hit build.

# Configuration
Open config.cfg for various configuration options.
Explanation of values:

- ```volume``` - a value by which to multiply gain of each played sound.
- ```enable_hrtf``` - whether or not to enable Head-Related Transfer Function (in other words, an even improved spatialization)
- ```mix_all_sounds_to_mono``` - whether or not all requested sounds should be mixed to mono, and therefore spatialized. Note that if a requested sound is stereo, it will not be spatialized unless this flag is set to 1.
- ```listener_position``` - the position in 3D space of the listener of the sounds.
- ```listener_orientation``` - two three-dimensional vectors that define the listener's orientation. First three values are the "at" vector, the latter three values are the "up" vector.
- ```scale_key_positions``` - a value by which to scale all positions in 3D space of the keys. By default, the app positions escape at ```(0;0;0)``` coordinates and makes standard-sized buttons like Q,W,E,R,T,Y... 45 units wide and high.
- ```output_device``` - output audio device. Leave "" to select the default output device.
- ```sleep_every_iteration_for_microseconds``` - how much to sleep per every main loop iteration. Higher values will eat less CPU, but some keystrokes might be missed.
- ```default_pairs``` - the default sound pairs for the unspecified keys.
- ```mute_when_these_processes_are_on``` - list of process names whose existence will mute the simulator. Useful when you want to play a game without the clicking sounds, for example. You can leave this field empty.

The above lines **must remain in that order and no other line might be found inbetween them**.
The next line must be equal to ```keys:```

Each next line contains custom values for the keys.
Format:

```name="Key Name" position=(x;y;z) pairs: "down_sound1" "up_sound1" "down_sound2" "up_sound2" ... "down_soundn" "up_soundn"```

You might leave just a single space after ```pairs:``` to specify no sound for that key.
If a line begins with %, it is a comment.

To see available key names, go to 
https://github.com/geneotech/Mechanical-keyboard-simulator/blob/master/augs/window_framework/event.cpp#L109

Notice that mouse clicking sounds don't get properly fetched in some areas, for example in Task Manager.
If however it happens that the simulator misses some other obvious mouse clicks, try increasing the priority of the process.
To disable mouse clicking sounds altogether, uncomment these three following lines:

```
%name="Left Mouse Button" position=(1.0;0;0) pairs: 
%name="Right Mouse Button" position=(1.2;0;0) pairs: 
%name="Middle Mouse Button" position=(1.1;0;0) pairs: 
```

# Sounds
The keystroke sounds are recordings of my own keyboard.

The mouse click sounds were taken from http://www.freesound.org/people/junkfood2121/sounds/208204/.

Feel free to create a pull request with better sounds that you find, perhaps with a separate config.cfg file for them.

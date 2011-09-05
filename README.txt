This branch aims to give the possibility to convert the blender scene before releasing the game and than allow the blenderplayer to load the already converted scene.

This should bring a few advantages:
1) the loading should be faster(depending on the heaviness of the scene)
2) the scene will be more "protected" (it will be more difficult to extract the data packed within the converted scene, and blender won't simply load it)
3) it may be smaller, since it doesn't contains informations that are not used by bge.

It will still be OS independent.

The saving project relies on gamekit's FBT(file binary table), a system that is really similar to blender dna, but allow it for c++.
Right now you can find all the converting stuff in source/gameengine/Converter/FileSave, the KX_FileInterface is the actual class that mind to save and load the file, and do all the other stuff required by this system(resolve pointers, change endianess, etc..), while KX_ConvertClassStruct is the class that actually converts the bge objects and "populate" the relative lists in KX_FileInterface.

This work is not difficult, but it is quite long and it is highly parallelizable, that means:
whoever wants to join this work is highly encouraged to, for each object you just need to create the relative struct in KX_Bgedna.h and the converting class in KX_ConvertClassStruct. There are already many converting class, so you'll understand easily how to do this.

Last little note:
i have never dug too much into the converting code, so it may be that i'm saving stuff that is not really setted up after conversion, so it wouldn't be necessary to save it. But to be on the safe side, i'm saving everything.
After completing this part, i'll look in what is leaft empty/null, and i'll remove it.
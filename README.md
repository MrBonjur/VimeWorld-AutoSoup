# VimeWorld AutoSoup in C++
![example2](https://user-images.githubusercontent.com/55990897/160291871-efe4663b-1afb-4726-9c78-8c7715a61147.gif)

* F8 - Unload DLL

* isDebugMode = true; - Creating a console, for printf

* Inject only in the inventory!
## AutoSoup for KitPvP https://vimeworld.ru
Bypass for method PostMessage (pressing keys in a minimized window)





## An example of how to replace pattern and mask:

pattern: ```00 1D 0F FF 00 ?? 4D```

pattern in code: ```\x00\x1D\x0F\xFF\x00\x00\x4D```
mask in code: ```xxxxx?x```

trigger_hp - At what hp level AutoSoup will be activated




To find pattern, you need to scan in CheatEngine 4 byte, value = 20

After that, reduce your life (for example, about a cactus), and click "Value decreased"


Next, in the table below, change the type from 4 byte to array of bytes, this will be our pattern.

P.S. With every update of the launcher, the pattern will change.


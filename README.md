# Widescan

## Functions

```cpp
TrackingStartSet(uint32_t index); //queues an outgoing 0xF5 packet to tell the server to start tracking the XiActor with that index
TrackingStopSet(); //queues an outgoing 0xF6 packet to tell the server you wish to stop tracking
GetWidescanInfo(); //Returns a table containing the currently tracked targets Index, Level, XZY Position, and the current Widescan track Status.
```

## Including in an addon

```lua
--place the dll in the same directory as your addon, then

local addonPath = windower.addon_path:gsub('\\', '/')
local dllPath = addonPath .. 'Widescan.dll'
local WidescanInterface = assert(package.loadlib(dllPath, 'luaopen_Widescan'))()

--and use
WidescanInterface.TrackingStartSet(127)
WidescanInterface.TrackingStopSet()
local widescanInfo = WidescanInterface.GetWidescanInfo()

```

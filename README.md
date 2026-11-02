# moonsweepers

Internal cheat DLL for **Moon Sweepers** (Unity 6000.6.0f1, IL2CPP).

Hooks DirectX 11 to draw an ImGui overlay, resolves game classes through the
IL2CPP exports, and drives features by calling the game's own methods on the
local walker.

## Requirements

* Windows 10/11 x64
* Visual Studio 2022 with C++ Desktop workload (MSVC v143)
* Moon Sweepers launched in the **DirectX 11** profile. Steam shows a dialog
  before the game starts: pick the DX11 option. The overlay backend is DX11,
  so DX12 will not render it.
* An injector that can load a native DLL into the game process
  (Process Hacker 2 "Inject DLL" works, or any manual-map / LoadLibrary tool)

## Build

Open `moonsweepers.sln` in Visual Studio and build **Release x64**, or from a
Developer Command Prompt:

```
msbuild moonsweepers\moonsweepers.vcxproj /p:Configuration=Release /p:Platform=x64
```

Output lands at:

```
x64\Release\moonsweepers.dll
```

## Install

1. Launch Moon Sweepers from Steam and pick DirectX 11 in the launcher dialog.
2. Reach the main menu or a session.
3. Inject `moonsweepers.dll` into `MoonSweepers.exe`.
4. The overlay comes up on the first presented frame.

## Keybinds

| Key    | Action                                                   |
| ------ | -------------------------------------------------------- |
| Insert | Toggle the menu                                          |
| End    | Request a clean unload (FreeLibrary + ExitThread)        |
| T      | Teleport to the nearest unraised landmark (rebindable)   |

## Features

### Fly

Calls `MoonWalker.SetFlying(on, noclip)`. The second argument is the real
"phase through geometry" bit. A slider controls `FlySpeed`.

### Speed

Writes `MoonWalker.DevSpeedMultiplier` and `MoonWalker.EquipmentSpeedMultiplier`
to scale both base movement and tool-use speed.

### Night vision

Calls `MoonWalker.SetNightVision(bool)` so the game runs its own apply logic
(shader + network sync + ownership check).

### Gravity boots

Calls `MoonWalker.SetGravityBoots(bool)`.

### Brush tuner

Writes directly to the local `MoonBrush` instance found via
`MoonCrewSession.Instance -> localVisual -> brush`:

* `cleaningWidth`, `cleaningDepth`, `cleaningEdgeWidth`
* `reach`, `removalRate`, `strokeInterval`
* `clearInOnePass`

Two presets are included:

* **Vanilla** - back to roughly stock values.
* **Wide + smooth** - a bigger sweep that keeps frame times stable.

Rule of thumb if you tweak by hand: keep `removalRate` under 1000, leave
`clearInOnePass` off, and keep `strokeInterval` at or above 0.05 s. Width can
go wide without hurting frame time; it is the removal rate that costs frames.

### Dust locator and dust fill

An AOB scanner finds the sentinel pattern that precedes the local dust
inventory double. Once the slot is found, the fill feature can keep topping it
up to a configured target so you can empty at the depot without walking.

### Flag / landmark ESP

For each `MoonLandmark` returned by `MoonLandmarks.get_All()`:

* reads `latitude`, `longitude`, `kind`, `index`, `name`
* converts to a world position (direction scaled by current camera altitude)
* projects to screen through `UnityEngine.Camera.WorldToScreenPoint` on the
  walker's `viewCamera`
* calls `MoonWorld.IsLandmarkRaised(index)` to classify as raised or unraised

The menu exposes:

* **Only unraised** filter for both the on-screen draw and the picker list
* Live counters: `Flags: X/Y  Sites: X/Y  (total N)`
* **Teleport to nearest unraised** (via `MoonWalker.Teleport`)
* **Raise all unraised landmarks** (via `MoonWorld.CrewRaiseLandmark` so the
  P2P sync and stats path runs properly)
* **Pick landmark** combo plus **Teleport to selected**
* A rebindable key for the "teleport to nearest unraised" action

## Project layout

```
moonsweepers/
  main.cpp                     DllMain entrypoint, spawns the init thread
  components/
    core/
      core.cpp/.hpp            Init, shutdown, unload request
      hook/                    kiero + MinHook, D3D11 Present hook at slot 8
      overlay/                 ImGui context, Win32 wndproc swap, render loop
      menu/                    Top-level feature menu
      features/
        fly, speed, night_vision, gravity_boots
        brush_tuner
        dust_locator, dust_fill
        flag_esp
    engine/
      engine.cpp/.hpp          Caches local walker each frame, attaches render
                               thread to the IL2CPP domain
      il2cpp.cpp/.hpp          Thin binding over the il2cpp_* exports
      classes/
        camera                 UnityEngine.Camera + Transform helpers
        moon_brush
        moon_crew_session      Instance + localVisual
        moon_dev_tools
        moon_dust_hand
        moon_landmark          Snapshot of all landmarks with raised state
        moon_player_visual     Brush accessor via the field offset
        moon_run_over          get_LocalWalker
        moon_walker            Teleport, SetFlying, SetNightVision, etc.
        moon_world             GentleGround, SurfaceAt, IsLandmarkRaised,
                               CrewRaiseLandmark, LocalPlayer
      enum/landmark_kind
      structures/matrix, vector
  utilities/
    utilities.cpp/.hpp         PEB-based module lookup, wait_module, get_export
    log/log.cpp/.hpp           Logger: std::format into OutputDebugStringA
  vendor/
    include/
      imgui/                   ImGui 1.91.5 with the dx11 and win32 backends
      MinHook/                 Trampoline hooks for Present
      kiero/                   D3D11/D3D12 vtable discovery
```

## How it finds things

* `GameAssembly.dll` and `UnityPlayer.dll` are located with a PEB walk, no
  `EnumProcessModules`.
* IL2CPP functions are resolved by name through `GetProcAddress`.
* The local walker is obtained via `MoonRunOver.get_LocalWalker` each frame
  and cached inside `engine::local_walker()`.
* The render thread attaches to the IL2CPP domain on its first tick so that
  `runtime_invoke` works outside the game's main thread.

## Diagnostics

The build has no console. Every `lg::info`, `warn`, and `error` call emits via
`OutputDebugStringA` and shows up in **DebugView** (Sysinternals) with
`Ctrl+W` to capture Win32 output. Filter include `moon` to see only this DLL.

## Credits

Vendored third-party code, unmodified except where noted:

* [ImGui](https://github.com/ocornut/imgui) 1.91.5
* [MinHook](https://github.com/TsudaKageyu/minhook)
* [kiero](https://github.com/Rebzzel/kiero) with `KIERO_INCLUDE_D3D11`,
  `KIERO_INCLUDE_D3D12`, and `KIERO_USE_MINHOOK` all set to 1

Game class metadata was obtained from the standard Il2CppInspector / Cpp2IL
dumping pipeline against the shipped `GameAssembly.dll` and
`global-metadata.dat`.

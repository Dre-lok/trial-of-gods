# Trial of Gods

A solo-built, souls-like dungeon crawler demo made in Unreal Engine 5. Third-person combat, AI enemies, health and stamina management, and a complete loop from desert spawn to a completion screen with your time.

> **Status:** vertical-slice demo, not a finished game. Built by one developer in a single semester for CS 495.

<!-- TODO: add a gameplay GIF or 3-4 screenshots here, e.g. ![Desert opener](docs/screenshots/desert.png) -->

## The demo

1. **Desert opener.** You spawn in an empty desert with dunes and a ruined pyramid in the distance. The pyramid is the only landmark, so the player walks toward it with no waypoint or HUD arrow.
2. **Camera transition.** Reaching the entrance briefly takes control away with a cinematic camera move to mark the shift in tone.
3. **Egyptian-themed dungeon.** Branching paths, real dead ends, and enemies that chase and fight back. Health and stamina have to be managed across the whole space, not only inside fights.
4. **Parkour room.** A no-enemy room that changes the pace between maze combat and the final door.
5. **Completion screen.** The final door ends the run and shows your time.

## Gameplay systems

The core systems are written in C++ as self-contained components that attach in the constructor and communicate through delegates, so the same logic runs on the player and on enemies.

| Component | What it does |
|---|---|
| `HealthComponent` | Applies damage, broadcasts death delegates, triggers death animations |
| `AtkComponent` | Two-hit light combo, weapon sphere trace, stamina cost per swing |
| `DodgeComponent` | Directional dodge using `LaunchCharacter`, with a backward-roll fallback when there is no directional input |
| `StatusComponent` | Owns the `ECombatState` enum, a `CanAct()` gate, stamina drain, and a regen delay timer |

Blueprints sit on top as the glue layer: binding delegates in `BeginPlay`, playing animation montages, updating UI widgets, and routing input through the Enhanced Input System.

### Combat

- `TryLightAttack` returns a bool that Blueprint uses to branch into the `Light1` or `Light2` montage section, driven by a `CurrentCombo` index.
- The combo advances on an animation notify at the end of each swing, not on a timer, so the animation controls the window between hits.
- While `bTraceActive` is set, a sphere trace runs every tick from the weapon socket. A `HitActorsThisSwing` set prevents hitting the same target twice in one swing.

### Enemy AI

- Enemies use a Behavior Tree with a custom `BTTask` for attacks, an AI Controller, and a NavMesh for pathfinding. Behaviors include chasing, attacking with timing synced to the animation (around frame 13 of the swing), and circling.
- Enemies use the same hit trace as the player, so their attacks have a real hit window that the player can dodge through.
- **Gotcha:** the player is on team 255 (the neutral team), and the default AI perception ignores neutrals. `DetectNeutrals` has to be set to `true` on the perception config or enemies cannot see the player.

## Design notes

- **Landmark navigation:** a well-placed landmark guides the player without breaking immersion.
- **Branching layout with dead ends:** wrong turns cost something, which makes finding the right path feel earned.
- **Pacing:** the parkour room resets the player after close-quarters combat.

## Known limitations

- **Hitstop was cut.** The brief time-scale freeze on a successful hit caused a camera flash on enemy hits that I could not resolve in time.
- **Layered upper/lower body animation blending** was planned and started, then deprioritized.
- **Movement is functional, not polished:** no strafing, coyote time, landing squash, or footstep IK.
- A dungeon music track was sourced but is not in the current build.
- Several systems are intentionally rough. The goal was a vertical slice that shows every part of the game working together.

## Tech stack

- Unreal Engine 5 (**TODO: add exact version**)
- C++ and Blueprint hybrid
- Enhanced Input System, Behavior Trees, AI Perception, NavMesh
- Mixamo animations retargeted to the UE5 mannequin skeleton

## Running the project

**TODO: confirm these steps against the repo.**

1. Install Unreal Engine 5 (**TODO: version**) and, on Windows, Visual Studio with the "Game development with C++" workload.
2. Clone this repository.
3. Right-click the `.uproject` file and choose **Generate Visual Studio project files**.
4. Open the project in Unreal Engine and build when prompted.

## Controls

**TODO: fill in your actual bindings.**

| Action | Input |
|---|---|
| Move | |
| Camera | |
| Light attack | |
| Dodge | |

## Credits

- Characters and animations: [Mixamo](https://www.mixamo.com)
- Dungeon environment (sandstone walls, pillars, torches, carved statues): asset packs from the Fab marketplace (**TODO: list each pack, its author, and its license**)

Third-party assets remain the property of their respective owners and are subject to their own licenses.

## What I learned

- **Where not to spend time.** Using professional asset packs and Mixamo let the time go into systems instead of asset production.
- **Retargeting is a real cost.** Every animation and asset pack arrived with its own skeleton, naming, and scale assumptions, and each needed manual fixes.
- **Set up Git LFS from day one.** Unreal produces a lot of binary content, and adding large-file handling late made pushing the full project impractical.
- **Know when "good enough" is the right call.** Every major system reached a working state and was left there so that combat, AI, and level design could all get built.

## Author

DeAndre Vega · [LinkedIn](https://www.linkedin.com/in/deandre-vega-367968207/) · [GitHub](https://github.com/Dre-lok)

# Person 1 Execution Plan

Planning authority: `TEAM_CURRENT_STATE_AUDIT.md` (2026-08-19). This is an execution-control document, not implementation. The required whole-project verdict remains `PLAN_SUFFICIENT_BUT_INTEGRATION_CONTRACTS_ARE_MISSING`.

## 1. Component checklist and status summary

Update the checkbox dimensions only after attaching evidence in the task card or master board. A source implementation does not imply integration or gameplay completion.

| P1 production component / file | Disposition | Task(s) | Current status | SOURCE_DONE | INTEGRATION_DONE | RUNTIME_TESTED | VISUALLY_VERIFIED | GAMEPLAY_VERIFIED |
|---|---|---|---|---|---|---|---|---|
| `src/main.cpp` | TASK | `P1-APP-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Core/MyApp.hpp` | TASK | `P1-APP-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| planned `src/Core/MyApp.cpp` (currently missing) | TASK | `P1-APP-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Core/stdafx.hpp` | NO_CHANGE | none | DONE for present empty compatibility TU; do not use as a dumping ground | [x] | [x] | [x] syntax | n/a | n/a |
| `src/Core/stdafx.cpp` | NO_CHANGE | none | DONE for present empty compatibility TU | [x] | [x] | [x] syntax | n/a | n/a |
| `include/Core/GameEventMediator.hpp` | TASK | `P1-EVENT-001` | READY | [ ] | [ ] | [ ] | n/a | [ ] |
| `src/Core/GameEventMediator.cpp` | TASK | `P1-EVENT-001` | READY | [ ] | [ ] | [ ] | n/a | [ ] |
| `include/States/Base/State.hpp` | TASK | `P1-STATE-001` | READY | [ ] | [ ] | [ ] | n/a | [ ] |
| `src/States/Base/State.cpp` | TASK | `P1-STATE-001` | READY | [ ] | [ ] | [ ] | n/a | [ ] |
| `include/States/Base/GameState.hpp` | TASK | `P1-GAME-001`, `P1-CAMERA-001`, `P1-DEATH-001`, `P1-WIN-001`, `P1-PAUSE-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `src/States/Base/GameState.cpp` | TASK | `P1-GAME-001`, `P1-CAMERA-001`, `P1-DEATH-001`, `P1-WIN-001`, `P1-PAUSE-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/MainMenuState.hpp`, `src/States/Menus/MainMenuState.cpp` | TASK | `P1-MENU-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/EnterNameState.hpp`, `src/States/Menus/EnterNameState.cpp` | TASK | `P1-MENU-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/SettingState.hpp`, `src/States/Menus/SettingState.cpp` | TASK | `P1-MENU-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/MenuCharacterSelectionState.hpp`, `src/States/Menus/MenuCharacterSelectionState.cpp` | TASK | `P1-SELECT-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/MenuWorldState.hpp`, `src/States/Menus/MenuWorldState.cpp` | TASK | `P1-SELECT-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/MenuLevelState.hpp`, `src/States/Menus/MenuLevelState.cpp` | TASK | `P1-SELECT-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/DeathMenuState.hpp`, `src/States/Menus/DeathMenuState.cpp` | TASK | `P1-DEATH-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/WinMenuState.hpp`, `src/States/Menus/WinMenuState.cpp` | TASK | `P1-WIN-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/PauseMenuState.hpp`, `src/States/Menus/PauseMenuState.cpp` | TASK | `P1-PAUSE-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/LeaderboardState.hpp`, `src/States/Menus/LeaderboardState.cpp` | EXCLUDED from P1 implementation; P4-owned | P4 board task | DEFERRED for P1 | n/a | n/a | n/a | n/a | n/a |

P1 task totals: 10 remaining tasks; 4 READY; 6 BLOCKED; 8 P0; 2 P1; 0 P2; 0 P3. There is no P1 verified baseline implementation task. The two `stdafx` files are explicitly `NO_CHANGE`, not a completed application feature.

## 2. Exact current baseline

- `src/main.cpp` is an interactive normal-Brick demonstration. It opens an 800x600 window, owns `Brick` and spawned objects through a global `std::vector<GameObject*>`, simulates a bottom collision on Space, and deletes those raw pointers at exit. It does not construct `MyApp`, a `State`, a player, or a level.
- `include/Core/MyApp.hpp` declares `MyApp()`, `run()`, and private `processEvents()`, `update()`, and `render()` around an `sf::RenderWindow`. There is no `src/Core/MyApp.cpp`, no state stack, and no delta-time parameter on `update()`.
- `State`, `GameState`, `GameEventMediator`, and every P1-owned menu are empty class shells; their `.cpp` files only include the matching header.
- No production P1 code calls `LevelManager::load`, `setPlayers`, `setLevelCompletedCallback`, `setPlayerDeathCallback`, `update`, or `render`. There is no camera code.
- The P2 level runtime already owns map/runtime objects with `std::unique_ptr` and deliberately borrows `PlayerManager*` views. P1 must not replace that ownership model or transfer map-object implementation ownership.
- `ConfiguredLevel` currently hides load failure and requests `assets/levels/...` while the named files are under `assets/textures`. P1 must wait for the P2 load/readiness contract; P1 must not compensate with a second path heuristic.
- `UserData` has in-memory getters and mutation methods, but P4 owns it and persistence. P1 may consume the agreed API only.
- All current production translation units passed direct C++20/SFML 3.1 syntax checking at audit time. The standard CMake build is separately blocked by the workspace-space `windres` issue and pins SFML 3.0.0; that is not a P1 source failure.
- No map PNG was opened or re-audited for this plan. Exactly nine level identities are in scope: `W1_LV1` through `W3_LV3`; map-data editing remains user-owned.
- Group5 is reference-only. Its raw-pointer/state architecture and LLM/Ollama/chat feature must not enter Group4.

## 3. Remaining tasks in dependency order

| Order | Task | Status | Priority | Start/finish gate |
|---:|---|---|---|---|
| 1 | `P1-STATE-001` State base and stack lifecycle | READY | P0 | No cross-owner start gate; finish with runnable transition checks |
| 2 | `P1-APP-001` Production application entry / MyApp | READY | P0 | Integrates after `P1-STATE-001` source surface is stable |
| 3 | `P1-EVENT-001` GameEventMediator event bridge | READY | P0 | Contract names are stable; provider hookups land later |
| 4 | `P1-SELECT-001` Character/world/level selection and session ownership | BLOCKED | P0 | `DEC-PLAYER-OWNER`; `P3-FACTORY-001`; P4 GUI; current in-memory progression read contract |
| 5 | `P1-GAME-001` GameState active runtime orchestration | BLOCKED | P0 | selection, P2 load/readiness, active-level and player-ownership contracts |
| 6 | `P1-CAMERA-001` Clamped camera | BLOCKED | P0 | P2 world-extent/camera contract and running GameState |
| 7 | `P1-DEATH-001` Death/lives/restart/Game Over | BLOCKED | P0 | `DEC-MULTIPLAYER-DEATH`, `DEC-RESPAWN`, P2 death, P3 reset, and P4 GUI |
| 8 | `P1-WIN-001` Completion/unlock/next/final flow | BLOCKED | P0 | P2 completion, current in-memory progression contract, and P4 GUI; durable persistence is downstream P1 work |
| 9 | `P1-MENU-001` Main/name/settings menu navigation | READY | P1 | Navigation source can proceed in parallel; final visual completion consumes P4 GUI/Audio |
| 10 | `P1-PAUSE-001` Pause/resume/restart/exit | BLOCKED | P1 | stable state stack/GameState; restart semantics aligned with `DEC-RESPAWN` |

Do not change a BLOCKED task to READY merely because a recommended default exists. Record the decision or provider evidence in `01_MASTER_TASK_BOARD.md`, then update both documents in the same planning commit.

## 4. Files allowed to edit

P1 may edit only the following production surfaces while executing these cards:

- `SUPERMARIO/CS202_Group4_SuperMario/src/main.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Core/MyApp.hpp`
- planned `SUPERMARIO/CS202_Group4_SuperMario/src/Core/MyApp.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Core/GameEventMediator.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Core/GameEventMediator.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/States/Base/State.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/States/Base/State.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/States/Base/GameState.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/States/Base/GameState.cpp`
- P1-owned state pairs under `include/States/Menus` and `src/States/Menus`: `MainMenuState`, `MenuCharacterSelectionState`, `PauseMenuState`, `EnterNameState`, `SettingState`, `MenuWorldState`, `MenuLevelState`, `DeathMenuState`, and `WinMenuState`
- planned P1 checks under `SUPERMARIO/CS202_Group4_SuperMario/tests/Person1StateFlowContracts.cpp` and `tests/Person1ApplicationSmoke.cpp`

`include/Core/stdafx.hpp` and `src/Core/stdafx.cpp` are P1-owned but classified `NO_CHANGE`; remove that classification through the central board before editing them. Avoid new shared headers unless the exact same state/context surface cannot live in the already-owned base headers.

Any needed edit outside this list is a cross-owner request under `04_USAGE_AND_PROGRESS_WORKFLOW.md`: name the consuming task, exact file/symbol, requested contract delta, reason, owner, and test impact; wait for the owning person to land it.

## 5. Files read-only/reference

- P2: `include/src/Levels/Managers/LevelManager.*`, `MapManager.*`, `PhysicsEngine.*`, all stage wrappers, environment objects, and `MovingBlock.*`.
- P3: `include/src/Entities/**`, `include/src/Core/GameObject.*`, and `include/src/Objects/Items/**`.
- P4: `include/src/Core/UserData.*`, `include/src/UI/**`, `include/src/Audio/**`, `include/src/Components/AnimationComponent.*`, all blocks except MovingBlock, and `include/src/States/Menus/LeaderboardState.*`.
- Shared/build: `include/Core/Constants.hpp`, `CMakeLists.txt`, assets, save data, CSV files, and packaging scripts/layout.
- Evidence only: `TEAM_CURRENT_STATE_AUDIT.md`, the other files in `TEAM_EXECUTION_CONTROL`, `PLAN.md`, and Group5 source when a task explicitly calls for reference behavior.

Never edit or inspect map image contents as part of a P1 task. Never implement the excluded Group5 chatbot/LLM feature.

## 6. Inputs expected from other persons

| Input | Provider | Consuming P1 task(s) | Acceptance evidence |
|---|---|---|---|
| One loaded active-level object, explicit load status/error, safe reload/clear behavior | P2 | `P1-GAME-001`, `P1-DEATH-001`, `P1-WIN-001` | `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P2-LEVEL-LOAD`; failed and successful load checks |
| World extent in pixels and camera clamp inputs | P2 | `P1-CAMERA-001` | `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA`; narrow/wide-world checks |
| Once-only affected-player death notification with safe frame timing | P2 | `P1-DEATH-001`, `P1-EVENT-001` | `CON-P1-P2-DEATH`; repeated-frame suppression test |
| Once-only completion notification after correct WinFlag anchor handling | P2 | `P1-WIN-001`, `P1-EVENT-001` | `CON-P1-P2-COMPLETION`; completion idempotence test |
| Mario/Luigi construction and stable reset API | P3 | `P1-SELECT-001`, `P1-GAME-001`, `P1-DEATH-001` | `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P1-P3-PLAYER-RESET`; 1P lifetime and reset checks |
| Score/coin/lives storage semantics and HUD projection | P4 | `P1-EVENT-001`, `P1-DEATH-001`, `P1-WIN-001` | `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-HUD-DATA` |
| Audio event names and non-owning invocation surface | P4 | `P1-EVENT-001`, menu/game states | `CON-P1-P4-AUDIO-EVENTS`; missing-asset behavior does not crash flow |
| Save/load schema, P4 event-to-save timing, progression/unlock rules | P4 | later `P1-MENU-001`/`P1-SELECT-001` restoration plus `P1-EVENT-001` result handling | `CON-P1-P4-PERSISTENCE`, `CON-P1-P4-PROGRESSION`; `DEC-PERSISTENCE` recorded; P0 death/win still emit semantic events while it is open |
| GUI controls, focus, text entry, and value action surface | P4 | `P1-MENU-001`, `P1-SELECT-001`, `P1-DEATH-001`, `P1-WIN-001`, later pause | `CON-P1-P4-GUI`; keyboard-only path and one-action-per-input verified |

## 7. Outputs and contracts provided

| P1 output | Primary consumers | Timing/lifetime guarantee |
|---|---|---|
| Application window, main loop, state stack, and frame-boundary transition queue | all runtime owners | States are uniquely owned; no state is destroyed while its event/update callback is executing |
| Selected session descriptor: player count/characters and world/level identity | P2, P3, P4 | Values remain stable for an active stage; exactly world 1-3 and level 1-3 |
| Active player owning collection after `DEC-PLAYER-OWNER` | P2 borrower, P3 implementations | P2 receives non-owning views only; the owner outlives load/update/render and clears P2 views before destruction |
| Active-level lifetime and load-error routing | P2 provider, P4 UI | Exactly one active level per GameState; load failure never enters gameplay update |
| Deferred death/completion/pause/restart transitions | P2/P3/P4 providers | Callback receipt may enqueue, never destroy the current level/player during P2 traversal |
| Event bridge for score, coin, lives, death, completion, and audio cues | P2/P3 producers; P4 consumers | Defined direction and once-only semantics; no global owning raw pointer |
| Camera view applied during world rendering and restored for HUD | P2 world render; P4 HUD | Clamp derives from reported pixel extent; HUD is screen-space |
| Deterministic nine-level progression request | P2 stage factory; P4 persistence | `W1_LV1` through `W3_LV3` only; final completion is distinct from next-level |

These outputs implement or consume the canonical contracts named in the task cards. P1 does not unilaterally change a contract card in `02_INTEGRATION_CONTRACTS.md`.

## 8. Per-task implementation notes

### P1-STATE-001 — State base and stack lifecycle

- **Task ID:** `P1-STATE-001`
- **Title:** State base and stack lifecycle
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Provide the smallest safe lifecycle and transition mechanism used by application, menus, and GameState.
- **Exact current behavior:** `State` is an empty class and `State.cpp` only includes its header; no base lifecycle, shared context, state ownership, or transition behavior exists.
- **Missing behavior checklist:** [ ] virtual destructor; [ ] event/input, `update(float)`, and render interface; [ ] state-end/quit signal; [ ] minimal shared non-owning context; [ ] `std::unique_ptr<State>` ownership; [ ] push/pop/replace/quit requests; [ ] requests applied only after the active callback returns; [ ] empty-stack application exit; [ ] no duplicate transition from one event.
- **Exact target files:** `include/States/Base/State.hpp`; `src/States/Base/State.cpp`; planned `tests/Person1StateFlowContracts.cpp`.
- **Allowed edit files:** the exact target files only.
- **Read-only dependency files:** `include/Core/MyApp.hpp`; all menu/GameState headers; P4 GUI/UserData headers only after their contracts exist.
- **Do not implement:** MyApp loop, any concrete menu, GameState, P2 level code, P3 player code, P4 GUI/UserData, persistence, map handling, or a general-purpose event framework.
- **Dependencies:** none for source start; application integration is `P1-APP-001`.
- **Contracts provided:** frame-boundary transition/lifetime portion of `CON-P1-P2-ACTIVE-LEVEL` and `CON-P1-P3-PLAYER-OWNERSHIP`.
- **Contracts consumed:** none for the base lifecycle.
- **Group5 reference if useful:** only state lifecycle ordering may be compared; Group5 does not define Group4 ownership.
- **Do-not-copy warning:** do not copy Group5 raw-pointer state ownership, global managers, or chat/LLM wiring.
- **Implementation notes:** keep the stack owner in one place and use queued commands or an equivalent minimal mechanism. The active `State` must not delete itself. Put shared context in an existing P1 base header unless a separate type is proven necessary.
- **SFML 3.1 notes:** accept/inspect SFML 3 events using their variant API; do not use SFML 2 event fields or polling signatures.
- **Compile checks:** direct C++20/SFML 3.1 syntax check for `src/States/Base/State.cpp` and the small contract test; warning-clean for new code.
- **Runtime checks:** push A, push B, pop B, replace A, quit, and transition-request-during-update; destruct each state exactly once.
- **Visual checks:** not applicable to the base class.
- **Gameplay checks:** menu-to-state transition requests do not double-apply; empty stack exits cleanly.
- **Definition of done:** all missing-behavior boxes pass; no owning raw pointer; transition checks are runnable; consumer headers compile; all five completion dimensions are recorded accurately.
- **Suggested commit message:** `person1: add safe state lifecycle and queued transitions`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ] (n/a allowed with evidence); GAMEPLAY_VERIFIED [ ].

### P1-APP-001 — Production application entry / MyApp

- **Task ID:** `P1-APP-001`
- **Title:** Production application entry / MyApp
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Replace the Brick test entry with the production C++20/SFML 3.1 application loop and state owner.
- **Exact current behavior:** `main.cpp` is a manual Brick test with a global raw-pointer vector; `MyApp.hpp` is declaration-only; `src/Core/MyApp.cpp` is absent.
- **Missing behavior checklist:** [ ] `main` constructs/runs `MyApp`; [ ] window creation/close path; [ ] SFML 3.1 event polling; [ ] clock and bounded delta time; [ ] unique ownership of state stack; [ ] initial state creation; [ ] input/update/render order; [ ] frame-boundary transitions; [ ] graceful empty-stack/close exit; [ ] no Brick-test globals/manual deletes.
- **Exact target files:** `src/main.cpp`; `include/Core/MyApp.hpp`; new `src/Core/MyApp.cpp`; planned `tests/Person1ApplicationSmoke.cpp`.
- **Allowed edit files:** exact target files plus P1 State header only through a separately tracked `P1-STATE-001` change.
- **Read-only dependency files:** all concrete state headers; P4 Audio/UserData/GUI headers; `CMakeLists.txt` (P4 packaging owner).
- **Do not implement:** blocks, textures, levels, players, menus beyond selecting the initial state, AudioSystem, build/package corrections, or asset-root fallback logic.
- **Dependencies:** source surface from `P1-STATE-001`; P4 services may be integrated later without blocking the basic loop.
- **Contracts provided:** application lifetime underpinning `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P3-PLAYER-OWNERSHIP`, and P1/P4 service lifetimes.
- **Contracts consumed:** P1 State lifecycle only; later P4 audio/persistence services.
- **Group5 reference if useful:** event/update/render order may be observed only as reference.
- **Do-not-copy warning:** do not copy raw-pointer ownership, hard-coded Group5 paths, or chatbot service startup.
- **Implementation notes:** keep `main` tiny. Prefer one application-owned window and `std::unique_ptr` states. Initialize only services actually required by the first state; surface startup errors instead of entering a dead loop.
- **SFML 3.1 notes:** use `sf::VideoMode({w, h})`, `std::optional<sf::Event>`, event type queries, and C++20; avoid all SFML 2 APIs.
- **Compile checks:** direct syntax check `src/main.cpp`, `src/Core/MyApp.cpp`, `src/States/Base/State.cpp`; later build/link `SuperMario` against SFML 3.1 after the separate packaging/build task resolves its environment issue.
- **Runtime checks:** open and close window; escape/window-close exits once; empty stack exits; 120-frame smoke run has no leak/double destruction under an available sanitizer/debugger.
- **Visual checks:** one window, one clear/display per frame, correct initial state visible once P4 GUI lands.
- **Gameplay checks:** production executable enters the state flow, never the Brick demo.
- **Definition of done:** production entry uses MyApp; Brick-test ownership is gone; State lifetime test passes; successful clean launch/close evidence is linked; completion dimensions are independent.
- **Suggested commit message:** `person1: replace brick demo with production app loop`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-EVENT-001 — GameEventMediator event bridge

- **Task ID:** `P1-EVENT-001`
- **Title:** GameEventMediator event bridge
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Carry gameplay outcomes to state, score/lives/progression, HUD, and Audio consumers without transferring ownership or causing reentrant state destruction.
- **Exact current behavior:** `GameEventMediator` is an empty class and its `.cpp` only includes the header; P2 callbacks exist for death/completion but have no P1 consumer.
- **Missing behavior checklist:** [ ] typed death event with affected player identity/reference valid only during dispatch; [ ] level-completed event; [ ] score delta; [ ] coin delta; [ ] lives change; [ ] audio cue; [ ] subscribe/unsubscribe lifetime; [ ] deterministic dispatch order; [ ] duplicate suppression at the producer/event boundary; [ ] deferred state transition; [ ] safe no-listener behavior.
- **Exact target files:** `include/Core/GameEventMediator.hpp`; `src/Core/GameEventMediator.cpp`; planned `tests/Person1StateFlowContracts.cpp`.
- **Allowed edit files:** exact target files only.
- **Read-only dependency files:** `LevelManager.hpp`, `PlayerManager.hpp`, `UserData.hpp`, `AudioSystem.hpp`, `LevelGUI.hpp`, GameState/menu files.
- **Do not implement:** P2 collision/death/completion detection; P3 outcome calculation; P4 score storage, HUD, audio playback, save/load; a service locator; global owning pointers.
- **Dependencies:** canonical contract cards can be specified now; concrete provider/consumer integration depends on P2/P3/P4 tasks.
- **Contracts provided:** P1 routing/timing in `CON-P1-P2-DEATH`, `CON-P1-P2-COMPLETION`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-HUD-DATA`, `CON-P1-P4-AUDIO-EVENTS`, `CON-P1-P4-PROGRESSION`.
- **Contracts consumed:** P2 affected-player/completion callbacks; P3 item/enemy outcomes; P4 consumer APIs.
- **Group5 reference if useful:** none is needed; Group4 already has callback boundaries.
- **Do-not-copy warning:** never import Group5 LLM/Ollama/chat event types or raw global mediator ownership.
- **Implementation notes:** use a small explicit event surface. If callbacks are used, return/disconnect tokens or bind subscriber lifetime to GameState. Queue transition-causing outcomes until the frame finishes.
- **SFML 3.1 notes:** mediator is SFML-independent unless a P4-approved audio identifier type requires otherwise.
- **Compile checks:** syntax check both mediator files and contract test against all consumer forward declarations; prevent circular includes.
- **Runtime checks:** zero/one/multiple listeners; unsubscribe; listener removed before dispatch; exactly-once death/completion; score/coin events preserve values; queued transition after dispatch.
- **Visual checks:** HUD refresh and audio cue are checked by P4 integration; record linked evidence, not source-only completion.
- **Gameplay checks:** one collected coin increments once; one death decrements lives once; one flag completion transitions once.
- **Definition of done:** event directions and timing match canonical contracts; lifetime checks pass; no reentrant state deletion; all linked consumer integrations and dimensions are recorded separately.
- **Suggested commit message:** `person1: add typed deferred gameplay event bridge`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ] (n/a for mediator alone); GAMEPLAY_VERIFIED [ ].

### P1-MENU-001 — Main/name/settings menu navigation

- **Task ID:** `P1-MENU-001`
- **Title:** Main/name/settings menu navigation
- **Owner:** Person 1
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Implement the state/navigation behavior for first entry, player name, and settings while leaving reusable controls/rendering to P4.
- **Exact current behavior:** `MainMenuState`, `EnterNameState`, and `SettingState` are empty shells; no menu can be entered or exited.
- **Missing behavior checklist:** [ ] main menu commands Start/Settings/Leaderboard/Exit; [ ] EnterName input policy and confirmation/cancel; [ ] settings volume request and back navigation; [ ] keyboard focus/navigation; [ ] queued transitions; [ ] empty/invalid name handling; [ ] route Leaderboard to P4-owned state without editing it; [ ] preserve settings/session context across states.
- **Exact target files:** P1-owned header/source pairs for `MainMenuState`, `EnterNameState`, `SettingState`; planned P1 state-flow test.
- **Allowed edit files:** exact target files; State interface only via `P1-STATE-001`.
- **Read-only dependency files:** P4 `GUI.*`, `AudioSystem.*`, `UserData.*`, and `LeaderboardState.*`.
- **Do not implement:** GUI widgets/styles, audio playback internals, UserData persistence, Leaderboard, character/world/level selection, or GameState.
- **Dependencies:** `P1-STATE-001` for integration; `CON-P1-P4-GUI`; P4 audio/persistence for later visual/full integration. The source navigation model can start now.
- **Contracts provided:** P1 action vocabulary/navigation side of `CON-P1-P4-GUI`, plus menu commands and state-routing inputs to `CON-P1-P4-AUDIO-EVENTS` and `CON-P1-P4-PERSISTENCE`.
- **Contracts consumed:** P4 GUI value actions, audio volume API, and persistence/profile API.
- **Group5 reference if useful:** reference-only menu sequence may be compared; exact layout/architecture is not authoritative.
- **Do-not-copy warning:** no Group5 raw widget pointers, hard-coded asset paths, or chatbot menu entries.
- **Implementation notes:** keep domain navigation independent of rendering widgets. Leaderboard is only a transition target. Do not create a second settings data store.
- **SFML 3.1 notes:** text entry must use SFML 3 event variants and Unicode values; handle Backspace/Enter as key events without SFML 2 unions.
- **Compile checks:** syntax check the six menu translation units with State and P4 public headers.
- **Runtime checks:** navigation forward/back; cancel; empty name rejected or replaced only per agreed P4 profile contract; settings survives return; Exit closes once.
- **Visual checks:** focused option visible; name text readable; volume value visible; keyboard-only operation and contrast verified with P4 GUI.
- **Gameplay checks:** Start reaches selection, not a hard-coded level; Leaderboard route enters P4 state; return paths preserve valid session data.
- **Definition of done:** navigation checks pass, P4 UI is consumed rather than duplicated, accessibility path is verified, and all five dimensions have evidence or remain unchecked.
- **Suggested commit message:** `person1: implement main name and settings navigation`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-SELECT-001 — Character/world/level selection plus active-player session ownership

- **Task ID:** `P1-SELECT-001`
- **Title:** Character/world/level selection plus active-player session ownership
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`; exactly nine identities are `REQUIRED_BY_TEAM`; ownership is `IMPLIED_BY_CURRENT_GROUP4_DESIGN`
- **Current status:** BLOCKED
- **Purpose:** Produce one stable 1P character choice and one valid world/level, then create the production player owner selected by the recorded ownership decision.
- **Exact current behavior:** `MenuCharacterSelectionState`, `MenuWorldState`, and `MenuLevelState` are empty. No production owner exists for selected Mario/Luigi objects. P2 only borrows `PlayerManager*`.
- **Missing behavior checklist:** [ ] 1P choice; [ ] Mario/Luigi choice; [ ] world 1-3 list; [ ] level 1-3 list; [ ] locked-level behavior; [ ] exact nine-level identity validation; [ ] back/cancel; [ ] construct one player through P3-approved surface; [ ] unique owner established by `DEC-PLAYER-OWNER`; [ ] stable borrowed view for P2; [ ] preserve session/progression values.
- **Exact target files:** header/source pairs for `MenuCharacterSelectionState`, `MenuWorldState`, `MenuLevelState`; P1 State/MyApp context only under their own tasks; planned P1 contract test.
- **Allowed edit files:** exact P1 targets only.
- **Read-only dependency files:** P3 `EntityFactory.*`, Mario/Luigi/PlayerManager headers; P2 stage/LevelManager headers; P4 `UserData.*` and GUI.
- **Do not implement:** player classes/factory, LevelManager, stage wrappers, map validation/editing, progression storage, GUI widgets, or a fallback raw-pointer owner.
- **Dependencies:** `P1-STATE-001`; `P3-FACTORY-001`; `P4-GUI-001`; `CON-P1-P4-GUI`; P4's unblocked in-memory progression provider checklist. `P1-MENU-001` and durable persistence are later integrations, not first-slice selection blockers.
- **Contracts provided:** P1 side of `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P4-PROGRESSION`, and the selection action vocabulary in `CON-P1-P4-GUI`.
- **Contracts consumed:** P3 player construction/type mapping; P4 unlock/profile data and GUI value actions.
- **Group5 reference if useful:** character/menu behavior is reference-only; Group4's final matrix and unique ownership control.
- **Do-not-copy warning:** do not copy Group5 player raw-pointer vectors, level ownership, map paths, or chatbot selection UI.
- **Implementation notes:** P1 GameState/session owns `std::unique_ptr<PlayerManager>` while P2 borrows a view. Represent level identity as bounded values, not file paths.
- **SFML 3.1 notes:** consume P4 UI events via its SFML 3-compatible interface; no legacy event fields.
- **Compile checks:** syntax check all six selection files against P3/P4 public headers; compile a construction/lifetime contract check after P3 surface lands.
- **Runtime checks:** both supported characters in 1P; owner survives menu-to-GameState transition; P2 view is invalidated before owner destruction; locked and unlocked selection; bounds reject world/level 0 or 4.
- **Visual checks:** both player slots and selected character distinguishable; three worlds and three levels presented; locked choices readable and keyboard reachable.
- **Gameplay checks:** each of the nine identities routes to the corresponding stage request; no map contents are inspected by this task.
- **Definition of done:** decision is recorded, ownership has no raw owner, nine identities route correctly, progression filtering and all lifetime checks pass, and dimensions are evidenced independently.
- **Suggested commit message:** `person1: add session and nine-level selection flow`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-GAME-001 — GameState active runtime orchestration

- **Task ID:** `P1-GAME-001`
- **Title:** GameState active runtime orchestration
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`; lifetime/load safety is `IMPLIED_BY_CURRENT_GROUP4_DESIGN`
- **Current status:** BLOCKED
- **Purpose:** Own a playable session, select exactly one active stage, bind players/callbacks, and run safe event-update-render transitions.
- **Exact current behavior:** `GameState` is empty. There is no active player owner, active `LevelManager`, load call/result, callback wiring, pause/death/win routing, or game render path.
- **Missing behavior checklist:** [ ] receive validated session selection; [ ] own exactly one active level; [ ] hold selected players according to `DEC-PLAYER-OWNER`; [ ] pass non-owning player views; [ ] consume explicit load result/error; [ ] reject update on failed load; [ ] bind mediator/death/completion callbacks; [ ] poll player input once; [ ] deterministic update order; [ ] render level, players if not P2-rendered by contract, then HUD; [ ] defer state replacement until update returns; [ ] clear callbacks/views before destruction/reload; [ ] pause/death/win requests.
- **Exact target files:** `include/States/Base/GameState.hpp`; `src/States/Base/GameState.cpp`; planned P1 contract/smoke tests.
- **Allowed edit files:** exact target files; other P1 surfaces only in their own cards.
- **Read-only dependency files:** P2 `LevelManager.*` and stage headers; P3 player headers; P4 GUI/HUD/UserData/Audio headers.
- **Do not implement:** stage loading internals/path fallback, map parsing, physics, player internals, block/entity rendering internals, HUD, audio, persistence, or cross-owner resets.
- **Dependencies:** `P1-APP-001`, `P1-STATE-001`, `P1-SELECT-001`, `P1-EVENT-001`; P2 active-level/load contracts; resolved P3 ownership and asset-root decisions via their implementation tasks.
- **Contracts provided:** runtime-owner side of `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P3-PLAYER-OWNERSHIP`; state-consumer side of death/completion/HUD/audio contracts.
- **Contracts consumed:** `CON-P1-P2-LEVEL-LOAD`, P2 update/render/callback interfaces, P3 player surface, P4 HUD/audio/UserData surface.
- **Group5 reference if useful:** update/render sequencing may be compared; Group4 ownership and callbacks remain authoritative.
- **Do-not-copy warning:** no raw active-level/player ownership, fixed map dimensions, working-directory hacks, or LLM/chat wiring.
- **Implementation notes:** perform callbacks as signals only and queue state mutations. Load first, check success, then enter gameplay. Keep one canonical session selection; do not infer world/level from asset filenames.
- **SFML 3.1 notes:** pass `sf::RenderTarget`/views using current SFML 3 APIs; use seconds as `float` only at existing runtime boundaries.
- **Compile checks:** syntax check GameState with every P1 state and P2/P3/P4 public header; direct link smoke after providers land.
- **Runtime checks:** successful and failed load; exactly one level owner; repeated enter/exit/restart; callback during update; P2 borrowed views cleared before player teardown; no update after load failure.
- **Visual checks:** level and players visible; HUD drawn in screen view; failed-load screen/message visible without empty gameplay.
- **Gameplay checks:** one representative level supports input -> physics -> render -> one Block interaction -> one enemy interaction -> one item -> death/restart -> completion/transition.
- **Definition of done:** full vertical slice runs from production state flow, failure paths are safe, lifetime/update order checks pass, and integration/runtime/visual/gameplay dimensions each cite evidence.
- **Suggested commit message:** `person1: orchestrate active gameplay state safely`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-CAMERA-001 — Clamped camera

- **Task ID:** `P1-CAMERA-001`
- **Title:** Clamped camera
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `IMPLIED_BY_CURRENT_GROUP4_DESIGN`
- **Current status:** BLOCKED
- **Purpose:** Follow active players while clamping the world view to P2-reported dynamic pixel extent and keeping HUD screen-relative.
- **Exact current behavior:** no camera/view code exists in P1. Map width is dynamic; hard-coded 192-tile bounds would be incorrect.
- **Missing behavior checklist:** [ ] consume world pixel bounds; [ ] choose active-player focus under recorded multiplayer policy; [ ] horizontal clamp; [ ] vertical clamp or explicit fixed vertical framing; [ ] world narrower than viewport; [ ] spawn/respawn snap; [ ] stable smoothing if used; [ ] render world with camera; [ ] restore default/HUD view; [ ] resize handling.
- **Exact target files:** `include/States/Base/GameState.hpp`; `src/States/Base/GameState.cpp`; planned P1 smoke/contract test.
- **Allowed edit files:** exact target files only.
- **Read-only dependency files:** P2 `LevelManager.hpp`, `MapManager.hpp`; P3 `PlayerManager.hpp`; P4 `LevelGUI.hpp`.
- **Do not implement:** world extent calculation, player movement, map bounds, HUD layout, or map-specific camera coordinates.
- **Dependencies:** `P1-GAME-001`; `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA`; multiplayer active-player result from `DEC-MULTIPLAYER-DEATH` where it affects camera focus.
- **Contracts provided:** P1 view-application side of `CON-P1-P2-CAMERA`; screen-space view guarantee for `CON-P1-P4-HUD-DATA`.
- **Contracts consumed:** `CON-P1-P2-WORLD-EXTENT`; active/living player views from P3/P1 ownership contract.
- **Group5 reference if useful:** only feel/tuning may be observed; fixed Group5 map-width formulas are not reusable.
- **Do-not-copy warning:** do not copy hard-coded map widths, tile counts, raw player pointers without the owner contract, or Group5 architecture.
- **Implementation notes:** clamp from extent supplied in pixels. For narrow worlds, center rather than producing a negative clamp interval. If smoothing is added, snap after respawn and cap time-step effects; start with the minimum correct camera.
- **SFML 3.1 notes:** construct `sf::View` with SFML 3 rectangle/vector APIs and restore the intended view explicitly before HUD drawing.
- **Compile checks:** syntax check GameState and camera-specific check against SFML 3.1.
- **Runtime checks:** narrow/equal/wide world; far left/right players; resize; 1P/2P focus; respawn snap; no view beyond bounds.
- **Visual checks:** no void revealed outside world bounds; no HUD drift/scale; movement does not visibly jitter at normal frame rates.
- **Gameplay checks:** player remains trackable from spawn to flag in one representative wide level and one narrow-bound fixture.
- **Definition of done:** no hard-coded map width, all edge cases pass, HUD view is restored, and linked visual/gameplay evidence is attached.
- **Suggested commit message:** `person1: clamp gameplay camera to dynamic world extent`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-DEATH-001 — Death/lives/restart/Game Over

- **Task ID:** `P1-DEATH-001`
- **Title:** Death/lives/restart/Game Over
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`; pit lifecycle is `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; policy is `NEEDS_HUMAN_DECISION`
- **Current status:** BLOCKED
- **Purpose:** Convert an affected-player death notification into exactly one lives change and the approved respawn, restart, or Game Over flow.
- **Exact current behavior:** `DeathMenuState` and GameState are empty. P2 exposes a `PlayerCallback` death boundary, but no P1 consumer or lives/reset transition exists.
- **Missing behavior checklist:** [ ] receive affected player identity; [ ] suppress duplicate handling; [ ] apply shared/separate lives rule; [ ] choose surviving-player behavior; [ ] decrement in-memory lives once; [ ] select approved reset/reload/menu transition; [ ] call P3 reset only through contract; [ ] preserve/reset score/power per decision; [ ] Game Over at exact condition; [ ] DeathMenu restart/exit; [ ] clear callbacks before teardown; [ ] emit a stable death/session-change outcome that downstream HUD/persistence may consume later.
- **Exact target files:** GameState header/source; DeathMenuState header/source; mediator files only through `P1-EVENT-001`; planned P1 tests.
- **Allowed edit files:** exact P1 targets only.
- **Read-only dependency files:** P2 LevelManager/hazard/physics; P3 PlayerManager; P4 UserData/HUD/persistence/GUI.
- **Do not implement:** pit/enemy/hazard detection, Player reset internals, UserData mutation internals/serialization, HUD, or an implicit policy chosen in code.
- **Dependencies:** `P1-GAME-001`, `P1-EVENT-001`, `P4-GUI-001`; approved 1P death/respawn rules; `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-GUI`.
- **Contracts provided:** deferred state-flow/lives consumer portions of `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET`, and stable semantic outcomes for later P4 HUD/persistence consumption.
- **Contracts consumed:** affected-player event from P2; reset from P3; current in-memory lives API and `CON-P1-P4-GUI` value actions from P4. Durable persistence is not a P0 blocker.
- **Group5 reference if useful:** pit threshold and death-menu behavior are technique references only; Group4 decisions control multiplayer and ownership.
- **Do-not-copy warning:** do not copy Group5 health-zero/raw-player replacement, separate architecture assumptions, fixed map height formulas, or chatbot hooks.
- **Implementation notes:** callbacks enqueue one transition token keyed by active stage/death sequence. State teardown occurs after LevelManager update returns. Recommended defaults in decision cards are not authorization.
- **SFML 3.1 notes:** DeathMenu input uses event variants; timers use `sf::Clock`/seconds consistently if a delay is approved.
- **Compile checks:** syntax check GameState, DeathMenu, mediator, and death contract test with P2/P3/P4 headers.
- **Runtime checks:** pit/enemy/lava death; duplicate callbacks across frames; last-life boundary; 1P/2P cases selected by decision; restart repeatedly; exit; failed reload; callback cannot outlive owner.
- **Visual checks:** death/Game Over state is visible once; in-memory lives value changes once; input focus works. P4-HUD records its separate HUD proof later.
- **Gameplay checks:** approved respawn preserves/resets exactly the decided fields and resumes in a valid position; Game Over returns to the approved menu.
- **Definition of done:** `DEC-MULTIPLAYER-DEATH` and `DEC-RESPAWN` are approved, once-only/lifetime checks pass, the in-memory lives and visible DeathMenu flow agree, a stable downstream session event is emitted, and all dimensions have linked evidence. HUD/durable persistence close in their P4 tasks.
- **Suggested commit message:** `person1: wire death lives restart and game-over flow`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-WIN-001 — Completion/unlock/next/final flow

- **Task ID:** `P1-WIN-001`
- **Title:** Completion/unlock/next/final flow
- **Owner:** Person 1
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN` and nine-level count `REQUIRED_BY_TEAM`
- **Current status:** BLOCKED
- **Purpose:** Turn one level-completion event into in-memory unlock/menu behavior, the correct next stage, or final-game completion, while publishing a stable outcome for later persistence.
- **Exact current behavior:** `WinMenuState` and GameState are empty. P2 has a completion callback boundary, but no consumer. No progression is connected to UserData or persistence.
- **Missing behavior checklist:** [ ] exactly-once completion handling; [ ] freeze/finish current gameplay safely; [ ] show WinMenu; [ ] update the in-memory unlocked successor; [ ] emit a stable completion/progression outcome for later persistence; [ ] retry/select/next controls; [ ] `Wn_L1 -> Wn_L2 -> Wn_L3`; [ ] `W1_L3 -> W2_L1`; [ ] `W2_L3 -> W3_L1`; [ ] `W3_L3 -> final completion`, never W4; [ ] load failure route; [ ] preserve selected session as agreed.
- **Exact target files:** GameState header/source; WinMenuState header/source; selection menus only under `P1-SELECT-001`; planned P1 tests.
- **Allowed edit files:** exact P1 targets only.
- **Read-only dependency files:** P2 WinFlag/LevelManager/stages; P4 UserData/persistence/GUI; P3 players.
- **Do not implement:** WinFlag anchor/animation, stage wrappers, map editing, unlock storage, serialization, final artwork, or leaderboard.
- **Dependencies:** `P1-GAME-001`, `P1-EVENT-001`, `P4-GUI-001`; `CON-P1-P4-GUI`, `CON-P1-P2-COMPLETION`, and the implemented/tested in-memory portion of `CON-P1-P4-PROGRESSION`. `DEC-WINFLAG-POLISH` and `DEC-PERSISTENCE` do not block the core state transition.
- **Contracts provided:** state-transition side of completion and progression contracts; valid next-level identity.
- **Contracts consumed:** P2 once-only completion; P4 in-memory unlock and `CON-P1-P4-GUI` value-action APIs. `CON-P1-P4-PERSISTENCE` consumes the emitted outcome later.
- **Group5 reference if useful:** completion timing/visual polish may be observed, but Group4 base-anchor correction and nine-level rules control.
- **Do-not-copy warning:** do not copy Group5 flag coordinates, map paths, raw pointers, or any chatbot completion behavior.
- **Implementation notes:** implement progression as bounded world/level arithmetic with a distinct terminal result. Queue the transition after the update. Core completion must not wait on optional WinFlag polish.
- **SFML 3.1 notes:** WinMenu controls/events use SFML 3 variants; presentation timing must not block event polling.
- **Compile checks:** syntax check GameState/WinMenu and a pure nine-transition table check.
- **Runtime checks:** all nine source identities; eight successor cases plus W3_L3 terminal; duplicate completion; downstream outcome emitted once; next-level load failure; repeated menu input.
- **Visual checks:** Win/final states are distinguishable; unlocked next level is visible after returning to selection.
- **Gameplay checks:** complete each `LV-W1-LV1` through `LV-W3-LV3` validation only when its separate validation entry reaches GAMEPLAY_VERIFIED; this task supplies engine flow, not map edits.
- **Definition of done:** transition table is exhaustive, no W4/out-of-range level occurs, in-memory unlock and visible transition are once-only, a stable completion event is available to later persistence, and all dimensions cite evidence.
- **Suggested commit message:** `person1: implement nine-level completion progression flow`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P1-PAUSE-001 — Pause/resume/restart/exit

- **Task ID:** `P1-PAUSE-001`
- **Title:** Pause/resume/restart/exit
- **Owner:** Person 1
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** BLOCKED
- **Purpose:** Pause gameplay without corrupting active ownership, then resume, restart, or exit through queued state transitions.
- **Exact current behavior:** `PauseMenuState` is empty and GameState has no pause input or update suppression.
- **Missing behavior checklist:** [ ] pause input edge; [ ] show PauseMenu once; [ ] stop gameplay updates/timers through the state contract; [ ] continue window event processing/render; [ ] resume same owners/state; [ ] restart through approved reload/reset path; [ ] exit to approved menu; [ ] repeated key/debounce; [ ] callback/view cleanup on restart/exit; [ ] no save unless approved.
- **Exact target files:** PauseMenuState header/source; GameState header/source; planned P1 tests.
- **Allowed edit files:** exact P1 targets only.
- **Read-only dependency files:** P2 LevelManager; P3 PlayerManager; P4 GUI/Audio/UserData/persistence.
- **Do not implement:** P2 runtime pause internals, entity-specific timer edits, GUI widgets, audio internals, persistence policy, or a separate restart implementation from death flow.
- **Dependencies:** `P1-STATE-001`, `P1-GAME-001`; restart semantics aligned with the resolved respawn and persistence decisions where save timing is affected.
- **Contracts provided:** application pause/update gate; restart/teardown timing for active-level/player ownership contracts.
- **Contracts consumed:** P4 menu/audio UI and the approved P2/P3 reload/reset behavior.
- **Group5 reference if useful:** menu choices may be reference-only; no architecture is copied.
- **Do-not-copy warning:** do not copy raw state pointers, ad-hoc global paused flags, fixed paths, or chatbot UI.
- **Implementation notes:** make pause a state/overlay policy with one source of truth. No gameplay `update(dt)` while paused; render may continue. Restart reuses the canonical GameState construction path.
- **SFML 3.1 notes:** use key-pressed event variants; do not poll a held key each frame for stack mutations.
- **Compile checks:** syntax check PauseMenu/GameState and state-flow test.
- **Runtime checks:** pause/resume repeatedly; timers/positions unchanged while paused; close window while paused; restart and exit; no duplicate overlays; ownership destroyed once.
- **Visual checks:** paused state obvious; menu focus visible; world backdrop does not update; resume restores correct camera/HUD.
- **Gameplay checks:** pause at movement, collision, death callback boundary, and near completion without duplicate gameplay outcomes.
- **Definition of done:** update gate and transition safety checks pass, restart shares canonical lifecycle, P4 UI/audio integrate, and all dimensions are recorded separately.
- **Suggested commit message:** `person1: add safe pause resume restart and exit flow`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

## 9. Tests required per task

No test is considered evidence unless its command/result is linked from the master board. Prefer one small runnable `tests/Person1StateFlowContracts.cpp` over a new framework; keep window-dependent smoke checks separate.

| Task | Smallest required automated/runnable check | Required manual/runtime evidence | Dimensions it may close |
|---|---|---|---|
| `P1-STATE-001` | Stack push/pop/replace/quit, deferred mutation, destructor count assertions | none beyond runnable result | SOURCE_DONE, RUNTIME_TESTED; INTEGRATION_DONE only after a concrete state uses it |
| `P1-APP-001` | construction/empty-stack exit where headless-safe | production window launch/close and initial state | SOURCE_DONE, INTEGRATION_DONE, RUNTIME_TESTED, VISUALLY_VERIFIED, GAMEPLAY_VERIFIED |
| `P1-EVENT-001` | subscribe/unsubscribe, value preservation, ordering, once-only/deferred transition assertions | coin/death/completion event trace in vertical slice | SOURCE_DONE, RUNTIME_TESTED; remaining dimensions after integration |
| `P1-MENU-001` | command-to-transition table and name/settings validation | keyboard-only Main/Name/Settings/Leaderboard routing | all five after P4 GUI/audio integration |
| `P1-SELECT-001` | bounded world/level table, 1P/2P lifetime, P2 view-clear ordering | visual selection of all characters and nine stage identities | all five after P3/P4 integration |
| `P1-GAME-001` | fake/provider-assisted success/failure load, callback deferral, destruction order | production representative-level vertical slice | all five |
| `P1-CAMERA-001` | pure clamp cases: narrower/equal/wider than viewport and both edges | representative wide-level follow, resize, HUD stability | all five |
| `P1-DEATH-001` | policy table after decisions, once-only lives change, last-life, teardown order | pit/enemy/lava deaths and approved 1P/2P restart/Game Over | all five |
| `P1-WIN-001` | nine-entry successor/terminal table, duplicate completion, failure routes | complete representative level then all nine validation entries | all five |
| `P1-PAUSE-001` | update counter frozen while paused; stack/restart destruction assertions | pause/resume/restart/exit at risky gameplay boundaries | all five |

Compile baseline for each touched translation unit: `g++ -std=c++20 -fsyntax-only` with Group4 `include` and installed SFML 3.1 include paths. Do not mark the task compile-complete using the existing CMake 3.0 pin. Full link/package evidence belongs to the coordinated release path because of the known `windres` workspace-space failure.

## 10. Integration checks

Run these in order and attach result, commit, platform, and failure notes. A failure updates the owning task; it does not authorize editing another owner's file.

1. **Lifecycle seam:** MyApp uniquely owns a State stack; transitions requested during event/update occur at frame boundary and destruct once.
2. **Selection/ownership seam:** recorded `DEC-PLAYER-OWNER` is implemented; selected Mario/Luigi owners outlive every P2 borrowed view and are cleared after P2 detaches.
3. **Load seam:** GameState selects one of exactly nine identities, P2 reports success/failure, and failed load routes visibly without update/render of an empty level.
4. **First integrated vertical slice:** production `main` -> MyApp -> menu/selection -> one loaded level -> one selected player owner -> P2 update/render -> P1 clamped camera -> visible world/player -> one representative normal Block interaction (preserve the verified baseline) -> one enemy interaction -> one item interaction -> HUD/event update -> pit or death -> approved restart -> WinFlag completion -> next state.
5. **Callback safety:** death/completion during P2 update only enqueue P1 changes; current level, players, subscribers, and views are not destroyed until traversal completes.
6. **Pause seam:** input processing continues, gameplay update stops, HUD/camera render correctly, and resume preserves owners.
7. **Progression seam:** all eight successors and W3_LV3 terminal behavior agree with P4 unlock/persistence; no state invents an asset path.
8. **Nine-level validation seam:** consume separate `LV-W1-LV1` through `LV-W3-LV3` entries. P1 records flow results but never edits or audits map pixels.
9. **Failure seam:** missing/corrupt level, missing optional audio, persistence failure, window close during a callback/menu, and empty state stack all fail visibly and safely.
10. **Regression seam:** P2's 27 runtime checks and P4 normal-Brick/BrickFragment lifecycle baseline still pass; P1 never takes ownership of those implementations.

Merge-risk order: stabilize State/MyApp interfaces first; mediator second; selection context third; then integrate GameState once P2/P3 contracts land. `GameState.*` is the P1 merge hotspot. One P1 integrator owns that pair during vertical-slice merges. Changes requested in `LevelManager.*`, `PlayerManager.*`, `UserData.*`, GUI/Audio, Leaderboard, blocks, `CMakeLists.txt`, assets, or maps go through the cross-owner workflow.

## 11. Final owner-specific definition of done

Person 1 is complete only when all of the following are true:

- All ten P1 task cards are DONE on the master board; none remains READY, IN_PROGRESS, BLOCKED, TESTING, DEFERRED, or EXCLUDED as a substitute for required work.
- Every P1 production file in Section 1 remains accounted for. The only `NO_CHANGE` files are the two `stdafx` files unless a board-approved task changes that classification. Leaderboard implementation remains P4-owned.
- Production `main` launches `MyApp`, and one safe unique-owner state stack drives event, update, render, and deferred transitions under C++20/SFML 3.1.
- Exactly one active level and the approved active-player owner exist for a GameState; P2 borrows players and owns its runtime/map entities with no raw owning regression.
- Load failure, world extent, camera, death, completion, player reset, score/coins/lives, HUD, audio, persistence, and progression all use the canonical contracts. All relevant OPEN decisions are CLOSED by humans before their dependent behavior is accepted.
- Main/name/settings, 1P/2P character selection, world/level selection, pause, death/Game Over, win/next/final, and Leaderboard routing function through the assigned owner boundaries.
- The first integrated vertical slice passes end to end. All nine separate validation entries `LV-W1-LV1` through `LV-W3-LV3` pass their P1 state/progression portions without duplicating engine code or assigning map edits to P1.
- Every task independently records SOURCE_DONE, INTEGRATION_DONE, RUNTIME_TESTED, VISUALLY_VERIFIED, and GAMEPLAY_VERIFIED with evidence. Source completion alone is never reported as full completion.
- Required automated/runnable checks and manual visual/gameplay checks pass; P2's 27-check ownership/lifecycle suite and P4's verified normal-Brick behavior remain intact.
- No P1 commit contains Group5 chatbot/LLM code, Group5 raw-pointer ownership, hard-coded 192-tile assumptions, alternate asset-root heuristics, map-image edits, P2/P3/P4 implementation work, or silent decision defaults.
- Final P1 merge is warning-clean under direct C++20/SFML 3.1 checks and participates in the coordinated full build/package verification once the separate build environment task is ready.

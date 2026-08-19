# Group4 Super Mario — Implementation Overview

Planning baseline: `TEAM_CURRENT_STATE_AUDIT.md`, 2026-08-19  
Group4 source baseline: branch `person2/map-integration`, commit `f242c4f0e64a484a44727568d9d7f6d698798232`  
Plan verdict: **PLAN_SUFFICIENT_BUT_INTEGRATION_CONTRACTS_ARE_MISSING**  
Execution target: **C++20 + SFML 3.1.0**

## 1. Purpose and authority

This directory is the team's persistent execution-control system. It converts the verified audit into owner-specific implementation cards, stable interface contracts, decision gates, test gates, and a merge-safe critical path. It is planning only; it does not authorize map edits, cross-owner source edits, production changes, test changes, commits, branch changes, or pushes.

When documents disagree, use this order:

1. `03_HUMAN_DECISIONS.md` for approved product choices; until approval, every decision remains `OPEN`.
2. `02_INTEGRATION_CONTRACTS.md` for cross-owner interfaces and lifetimes.
3. `01_MASTER_TASK_BOARD.md` for canonical task ID, owner, priority, status, lane, and dependency state.
4. `P1_EXECUTION_PLAN.md` through `P4_EXECUTION_PLAN.md` for implementation and verification detail.
5. `TEAM_CURRENT_STATE_AUDIT.md` for the evidence baseline.

No post-audit production-source change was present when these plans were generated. Group4 remained at the audited commit, with the same user-owned modified `assets/textures/LevelSketch_W1_LV1.png` and existing untracked build directories. Therefore no task carries `STATUS_CHANGED_SINCE_AUDIT`.

## 2. Fixed scope facts

- The game has exactly nine required stages: `W1_LV1`, `W1_LV2`, `W1_LV3`, `W2_LV1`, `W2_LV2`, `W2_LV3`, `W3_LV1`, `W3_LV2`, and `W3_LV3`.
- Group5 is behavioral and technique reference only. It is not Group4's architecture authority.
- Group5's chatbot/LLM/Ollama/chat overlay is `EXCLUDED` and is never a task, dependency, acceptance criterion, or completion gate.
- Deterministic enemy movement, contacts, state machines, and boss behavior are normal gameplay and remain in scope.
- Group4 retains unique-ownership runtime architecture: one `std::unique_ptr` owner for map/runtime objects and non-owning typed views. Never regress to Group5 raw-pointer ownership.
- Runtime ownership, implementation ownership, integration responsibility, and test responsibility are separate axes.
- The map architecture is dynamic width, exactly 30 image rows, two logical layers of 15 rows each, and `TILE_SIZE=32`.
- Known map-data defects are user-owned `KNOWN_MAP_EDIT_ITEM`s. These plans contain engine readiness and per-level validation, not map-pixel editing tasks.

## 3. Canonical record counts

| Record class | Count | Meaning |
|---|---:|---|
| Remaining implementation tasks | 45 | P1: 10, P2: 13, P3: 11, P4: 11 |
| Verified-baseline records | 7 | P2: 4, P4: 3; frozen unless new contrary evidence appears |
| Total task records | 52 | Remaining tasks plus verified baselines |
| `READY` remaining tasks | 21 | May start within allowed files and stated contracts |
| `BLOCKED` remaining tasks | 24 | Must not start the blocked portion until the named gate clears |
| `DONE` baseline records | 7 | Preserve and regression-test; do not reopen by default |
| P0 remaining tasks | 25 | Every one blocks the first complete vertical slice |
| P1 remaining tasks | 18 | Required for full gameplay completion |
| P2 remaining tasks | 2 | Late/release work |
| P3 remaining tasks | 0 | Maintenance only; none created at planning baseline |
| Integration contracts | 25 | Canonical provider/consumer boundaries |
| Open human decisions | 8 | All remain `OPEN` |
| Level validation entries | 9 | One per required stage; not duplicate engine tasks |

The status vocabulary is closed: `DONE`, `READY`, `IN_PROGRESS`, `BLOCKED`, `TESTING`, `DEFERRED`, `EXCLUDED`.

Completion is not a single checkbox. Every task tracks these dimensions independently:

- `SOURCE_DONE`
- `INTEGRATION_DONE`
- `RUNTIME_TESTED`
- `VISUALLY_VERIFIED`
- `GAMEPLAY_VERIFIED`

A source-complete task may move to `TESTING`; it does not become `DONE` until every applicable Definition-of-Done gate is satisfied or explicitly marked `NOT_APPLICABLE` with reviewer agreement.

## 4. Ownership summary

| Owner | Implementation ownership | Runtime/integration boundary |
|---|---|---|
| Person 1 | `MyApp`, production entry, state base/stack, `GameState`, menus except Leaderboard implementation, selection/session ownership, mediator, camera, death/win transitions | Owns the active level; selected-player/session ownership remains pending `DEC-PLAYER-OWNER` (recommended default: P1); consumes P2 callbacks and P4 session/UI services |
| Person 2 | movement, physics, map/level managers, all nine stage wrappers, Pipe/Lava/Trampoline/MovingBlock, Cannon/Bullet/Rocket/WinFlag | Owns map/runtime objects with `unique_ptr`; borrows players from the approved session owner (P1 is only the recommended default while `DEC-PLAYER-OWNER` is open); orchestrates world contacts and lifecycle |
| Person 3 | `GameObject`, living/entity/player/enemy/item implementations, `EntityFactory`, player reset and gameplay outcomes | Provides entity behavior and state APIs; does not own the production session, world orchestrator, blocks, HUD, or persistence |
| Person 4 | animation component, blocks except MovingBlock, BrickFragment, Audio, GUI/HUD, UserData/persistence, Leaderboard, release packaging | Provides block behavior/visual invariants and session presentation/storage; P2 adopts spawned runtime objects |

BrickFragment implementation remains Person 4 work. Person 2 only adopts, updates, tests, and removes instances in the runtime owner system.

## 5. Verified baseline — preserve, do not reopen

| Baseline ID | Verified fact | Regression expectation |
|---|---|---|
| `BASE-P2-OWNERSHIP-001` | Map/runtime objects have one `unique_ptr` owner plus non-owning typed views; generic physics participation is distinct from ownership | Existing ownership/participation checks continue to pass |
| `BASE-P2-LIFECYCLE-001` | Inactive owners are cleaned up and inactive Blocks are excluded in the same frame; normal Brick spawns are adopted safely | No inactive collision or duplicate/dangling owner regression |
| `BASE-P2-ROCKET-001` | Rocket target resolution does not retain a dangling player pointer | Live and disappeared-target checks continue to pass |
| `BASE-P2-RUNTIME-001` | Existing Person 2 runtime executable reports 27 passing checks | Preserve or migrate all 27 assertions into the final runnable test path |
| `BASE-P4-BRICK-001` | Normal Brick bottom-hit behavior breaks once and produces four real fragments | Preserve one-shot activation and exact four-fragment result |
| `BASE-P4-FRAGMENT-001` | BrickFragment position, update, handoff, expiry, and P2 removal path work | Preserve P4 implementation ownership and P2 runtime adoption |
| `BASE-P4-BLOCK-INTEGRATION-001` | Shared collision constants, public lifecycle query, popped Coin spawn, and one-tile-above Mushroom spawn are corrected | Do not reintroduce duplicate constants, private lifecycle access, or stale spawn offsets |

These are scoped baselines, not claims that the surrounding subsystem is complete. New tasks may extend a subsystem without rewriting its verified foundation.

## 6. Priority model

| Priority | Definition |
|---|---|
| P0 | Prevents the first complete vertical slice: launch → select → load → render → move/collide → representative interactions → camera/bounds → both death and completion branches in separate executions → transitions |
| P1 | Required for whole-game completion after the first slice |
| P2 | Late integration, release, packaging, or presentation completion |
| P3 | Maintenance/polish that is non-blocking; none is scheduled initially |

Priority does not override ownership, contract gates, or open decisions. A P0 task can remain `BLOCKED` while independent P1 work proceeds.

## 7. First complete vertical slice

The first vertical slice must use production paths and the contracted Group4 ownership model; selected-player ownership follows the still-open `DEC-PLAYER-OWNER`:

```text
production main / MyApp
  → State stack and selection
  → selected PlayerManager owner(s) under the approved decision (recommended default: P1 session)
  → P1-owned active ConfiguredLevel / LevelManager
  → P2 receives borrowed PlayerManager* views
  → stage load reports success or a visible safe failure
  → map objects become P2 unique_ptr owners plus typed views
  → frame update and block physics
  → representative enemy, item, Block, and hazard behavior
  → P1 camera clamps to P2 world extent
  → P2 emits exactly-once death and completion callbacks in separate executions
  → P1 defers state replacement until LevelManager::update returns
  → death/win transition is visible and stable
```

The 25 P0 task cards are the canonical first-slice blockers. The normal-Brick path is already represented by frozen baseline records and must be reused rather than rebuilt.

The slice is not complete with a direct test `main`, hard-coded stage path, stack-owned borrowed players, empty rendered entities, manual callback invocation, or a transition that destroys the active level during its own update.

## 8. Whole-game Definition of Done

The whole game is complete only when all of the following are true:

- Production startup reaches the state-driven flow; no Brick test remains as production `main`.
- Character selection creates one or two valid players under the approved ownership/lifetime contract.
- Exactly nine stages load through shared engine code with explicit load success and coherent runtime assets.
- Dynamic world extent, camera clamp, left/right bounds, map-derived pit death, and owned-object off-world cleanup work.
- Player/block, player/enemy, player/item, hazard, projectile, and WinFlag interactions have deterministic ordering and exactly-once outcomes.
- Representative and required entities are visible, animated as applicable, collision-aligned, and gameplay verified.
- Normal Brick remains correct; question blocks, payload/special Brick behavior, variants, and approved Cloud semantics work without ownership regressions.
- Death, lives, reset/reload, Game Over, completion, unlock, next-level, and final-completion flows follow approved decisions.
- HUD, audio, UserData, persistence, and leaderboard consume stable event/data contracts and fail safely.
- All nine `LV-*` validations pass; map content issues remain separately resolved by the user and are never hidden as engine code.
- Production and test targets compile as C++20 against SFML 3.1.0; legacy SFML 2 test syntax is removed from required checks.
- A clean documented build/package launches with executable-adjacent or otherwise approved assets from the approved working directory.
- All applicable completion dimensions are recorded as satisfied and the critical integration scenarios are runtime, visual, and gameplay verified.

## 9. Explicit non-goals and exclusions

- No Group5 chatbot, LLM service, Ollama integration, chat component, or chat UI.
- No port of Group5 raw-pointer ownership, mediator-held object ownership, hard-coded 64-pixel assumptions, or state architecture.
- No map-image inspection, pixel edits, palette edits, or re-audit in this planning work.
- No new level-specific engine forks. The nine wrappers select data; shared managers own engine behavior.
- No scope expansion into generic Mario features not assigned by PLAN or implied by current Group4 architecture.
- No silent resolution of any `DEC-*` card.
- No cross-owner production edit without the workflow in `04_USAGE_AND_PROGRESS_WORKFLOW.md`.

## 10. Execution order at a glance

1. Human owners review and decide only the eight `DEC-*` cards needed for their next blocked work.
2. Owners implement `READY` independent foundations in separate files/commits.
3. Contract providers land the smallest stable surfaces in `02_INTEGRATION_CONTRACTS.md`.
4. Build the production first vertical slice around one representative stage without changing its map pixels.
5. Run component checks, contract integration checks, then the nine separate `LV-*` validations.
6. Add full menus, remaining entity/block behaviors, HUD/audio/persistence, and release packaging.
7. Mark tasks `DONE` only after applicable source, integration, runtime, visual, and gameplay gates are recorded.

Use `05_CRITICAL_PATH.md` for exact merge order and conflict-risk sequencing.

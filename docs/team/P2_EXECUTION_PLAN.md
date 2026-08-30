# Person 2 Execution Plan — Physics, Level Runtime, and Environment

Planning baseline: `TEAM_CURRENT_STATE_AUDIT.md` dated 2026-08-19. This document plans work only. It does not authorize production, test, map, asset, build, or Git changes. The controlling verdict is `PLAN_SUFFICIENT_BUT_INTEGRATION_CONTRACTS_ARE_MISSING`.

## 1. Component checklist and status summary

Status values are limited to `DONE`, `READY`, `IN_PROGRESS`, `BLOCKED`, `TESTING`, `DEFERRED`, and `EXCLUDED`. Completion dimensions are independent: `SOURCE_DONE`, `INTEGRATION_DONE`, `RUNTIME_TESTED`, `VISUALLY_VERIFIED`, and `GAMEPLAY_VERIFIED` must each be recorded rather than inferred from the workflow status.

| Component and every owned production file | Accounting | Task / baseline | Status | Source | Integration | Runtime | Visual | Gameplay |
|---|---|---|---|---|---|---|---|---|
| `include/Components/MovementComponent.hpp`; `src/Components/MovementComponent.cpp` | `VERIFIED_DONE` for audited base contract | `BASE-P2-RUNTIME-001` | `DONE` | YES | YES (current consumers) | YES (indirect) | N/A | NO |
| `include/Physics/PhysicsEngine.hpp`; `src/Physics/PhysicsEngine.cpp` | `TASK` plus frozen baseline | `BASE-P2-LIFECYCLE-001`, `P2-BOUNDS-001`, `P2-CONTACT-ENEMY-001`, `P2-CONTACT-ITEM-001`, `P2-PROJECTILE-001` | `BLOCKED` overall | PARTIAL | PARTIAL | PARTIAL | N/A | NO |
| `include/Levels/Managers/MapManager.hpp`; `src/Levels/Managers/MapManager.cpp` | `TASK` | `P2-LOAD-001`, `P2-EXTENT-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001` | `BLOCKED` overall | PARTIAL | NO | PARTIAL | N/A | NO |
| `include/Levels/Managers/LevelManager.hpp`; `src/Levels/Managers/LevelManager.cpp` | `TASK` plus frozen baselines | all remaining P2 runtime tasks; `BASE-P2-OWNERSHIP-001`, `BASE-P2-LIFECYCLE-001`, `BASE-P2-ROCKET-001` | `BLOCKED` overall | PARTIAL | PARTIAL | PARTIAL | NO | NO |
| `include/Levels/Stages/W1_LV1.hpp`; `src/Levels/Stages/W1_LV1.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W1-LV1` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W1_LV2.hpp`; `src/Levels/Stages/W1_LV2.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W1-LV2` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W1_LV3.hpp`; `src/Levels/Stages/W1_LV3.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W1-LV3` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W2_LV1.hpp`; `src/Levels/Stages/W2_LV1.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W2-LV1` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W2_LV2.hpp`; `src/Levels/Stages/W2_LV2.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W2-LV2` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W2_LV3.hpp`; `src/Levels/Stages/W2_LV3.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W2-LV3` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W3_LV1.hpp`; `src/Levels/Stages/W3_LV1.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W3-LV1` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W3_LV2.hpp`; `src/Levels/Stages/W3_LV2.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W3-LV2` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Levels/Stages/W3_LV3.hpp`; `src/Levels/Stages/W3_LV3.cpp` | `TASK` | `P2-NINE-LEVEL-001`, `LV-W3-LV3` | `BLOCKED` | YES (wrapper) | NO | NO | NO | NO |
| `include/Objects/Environment/Pipe.hpp`; `src/Objects/Environment/Pipe.cpp` | `TASK` (verify; preserve if green) | `P2-ENV-001` | `DONE` | YES | YES | YES | NO | NO |
| `include/Objects/Environment/Lava.hpp`; `src/Objects/Environment/Lava.cpp` | `TASK` | `P2-LAVA-001` | `DONE` | YES | PARTIAL | YES | NO | NO |
| `include/Objects/Environment/Trampoline.hpp`; `src/Objects/Environment/Trampoline.cpp` | `TASK` (verify; preserve if green) | `P2-ENV-001` | `DONE` | YES | YES | YES | NO | NO |
| `include/Objects/Blocks/MovingBlock.hpp`; `src/Objects/Blocks/MovingBlock.cpp` | `TASK` (verify; preserve if green) | `P2-ENV-001`, `P2-VARIANT-WIRE-001` | `DONE` | YES | YES | YES | NO | NO |
| `include/Objects/Environment/Cannon.hpp`; `src/Objects/Environment/Cannon.cpp` | `TASK` plus frozen lifecycle | `P2-PROJECTILE-001`, `BASE-P2-OWNERSHIP-001` | `DONE` | YES | PARTIAL | YES | NO | NO |
| `include/Objects/Environment/Bullet.hpp`; `src/Objects/Environment/Bullet.cpp` | `TASK` plus frozen lifecycle | `P2-PROJECTILE-001`, `BASE-P2-OWNERSHIP-001` | `DONE` | YES | PARTIAL | YES | NO | NO |
| `include/Objects/Environment/Rocket.hpp`; `src/Objects/Environment/Rocket.cpp` | `TASK` plus frozen lifetime | `P2-PROJECTILE-001`, `BASE-P2-ROCKET-001` | `DONE` | YES | PARTIAL | YES | NO | NO |
| `include/Objects/Environment/WinFlag.hpp`; `src/Objects/Environment/WinFlag.cpp` | `TASK` | `P2-WINFLAG-001` | `DONE` | YES | PARTIAL | YES | NO | NO |

Summary: 7 P2 implementation tasks are `DONE`, 6 remaining tasks are `BLOCKED`, 4 frozen P2 baseline cards remain `DONE`, and 9 separate level-validation entries remain tracked. No owned production file is unaccounted for. There are no P2-owned production files classified `NO_CHANGE`, `DEFERRED`, or `EXCLUDED`; preservation-only components are explicitly tied to a task or verified baseline.

## 2. Exact current baseline

- Group4 targets C++20 and SFML 3.1.0. The audit found all 68 production translation units syntax-compatible with installed SFML 3.1.0; CMake still pins SFML 3.0.0 and the full build is separately blocked by MinGW `windres` handling spaces in the workspace path.
- The adopted Group5-compatible sketches are dynamic-width and 45 rows at `CELL_SIZE=64`: P2 reads the first two 15-row runtime bands (objects plus actors) and deliberately ignores the final legacy band. World gameplay height derives from one logical band, never from encoded image height or window height.
- `MapManager` validates the layout, records dynamic width, decodes spawns and variants, and reports load diagnostics. It exposes tile width but no explicit pixel-world extent.
- All nine wrappers W1_LV1 through W3_LV3 exist. `ConfiguredLevel` currently requests `assets/levels/LevelSketch_W*_LV*.png`, while the named files are under `assets/textures`, and its constructor discards `LevelManager::load`'s Boolean result. No map image is to be opened or edited by this plan.
- `LevelManager` is the unique owner of map/runtime objects through `std::unique_ptr`; typed raw-pointer collections are non-owning views. P1-selected players are borrowed and must have a stable external owner. Do not convert either design to Group5-style raw ownership.
- The audited ownership/physics-participation distinction, immediate callback adoption, pending-spawn flush, inactive-owner cleanup, and same-frame inactive-Block exclusion are current working baselines.
- The 27-check P2 runtime executable passed. It covers current floor/wall/ceiling/headbutt behavior, ownership/view rebuilds, Bullet/BrickFragment lifecycle, normal Brick integration, and live/lost Rocket target lifetime. It does not establish camera, bounds, pit, player/enemy, player/item, nine-level, or visual/gameplay completion.
- Current update order is owned-object update, borrowed-player block physics, participating-enemy block physics, selected Lava/Bullet/Rocket/WinFlag overlap checks, then spawn flush and inactive cleanup. There is no player/enemy or player/item pass.
- Lava may shrink a powered player while still firing the death callback unconditionally. WinFlag interprets its marker as a pole top, but the team's Group5-derived white marker convention is a base/anchor tile. Anchor direction is a required correction; delayed flag polish remains `DEC-WINFLAG-POLISH` and stays open.
- Map variants are parsed but discarded at construction. CloudPlatform/CloudBlock is deferred from the current release; `MovingBlock` remains independent.
- Group5 is reference-only. Its map-derived pit threshold and base-anchor reasoning may be adapted; its raw-pointer mediator/ownership design must not be copied. Its LLM/Ollama/chat UI is `EXCLUDED_FROM_GROUP4`.

## 3. Remaining tasks in dependency order

| Order | Task | Priority | Status | Parallel class | Hard dependency / unblock condition |
|---:|---|---|---|---|---|
| 1 | `P2-EXTENT-001` | P0 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; P1 camera/gameplay consumption is downstream |
| 2 | `P2-LAVA-001` | P0 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; P1 death transition is downstream |
| 3 | `P2-WINFLAG-001` | P0 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; P1 win transition is downstream |
| 4 | `P2-ENV-001` | P1 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; visual/gameplay acceptance remains downstream |
| 5 | `P2-PROJECTILE-001` | P1 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; P3 callers and gameplay acceptance are downstream |
| 6 | `P2-LOAD-001` | P0 | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-ASSET-ROOT`; then `CON-P1-P2-LEVEL-LOAD` |
| 7 | `P2-BOUNDS-001` | P0 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; production visual/gameplay acceptance is downstream |
| 8 | `P2-PIT-001` | P0 | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime complete; P1/P3 fatal-respawn flow is downstream |
| 9 | `P2-CLEANUP-001` | P1 | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-EXTENT-001`, P3 lifecycle predicates, `CON-P2-P3-ENEMY-REMOVAL` |
| 10 | `P2-CONTACT-ENEMY-001` | P0 | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P3 contact/result APIs, P1 event sink, `CON-P2-P3-PLAYER-ENEMY` |
| 11 | `P2-CONTACT-ITEM-001` | P0 | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P3 collection/result APIs, P1 score sink, `CON-P2-P3-PLAYER-ITEM` |
| 12 | `P2-VARIANT-WIRE-001` | P1 | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P4 variant constructors, P3 payloads, `DEC-CLOUD-SEMANTICS`, `DEC-BLOCK-ACTOR-ELIGIBILITY` |
| 13 | `P2-NINE-LEVEL-001` | P1 | `BLOCKED` | `LATE_INTEGRATION` | Tasks 1–12 as applicable, P1 playable flow, P3/P4 visible gameplay; map-data issues remain user-owned |

P2 merge-risk order: serialize edits to `LevelManager.cpp/.hpp` in the sequence extent/load → bounds/pit/cleanup → contacts → variants/projectiles → nine-level integration. Do not run overlapping branches against that hub without an accepted cross-owner edit request and a named integrator.

## 4. Files allowed for P2 implementation

These are ownership boundaries for future implementation; this planning pass changes none of them.

- Core P2 production files: `include/Components/MovementComponent.hpp`, `src/Components/MovementComponent.cpp`, `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `include/Levels/Managers/MapManager.hpp`, `src/Levels/Managers/MapManager.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`.
- Stage files: `include/Levels/Stages/W1_LV1.hpp`, `W1_LV2.hpp`, `W1_LV3.hpp`, `W2_LV1.hpp`, `W2_LV2.hpp`, `W2_LV3.hpp`, `W3_LV1.hpp`, `W3_LV2.hpp`, `W3_LV3.hpp`, and matching nine files under `src/Levels/Stages/`.
- Environment files: matching header/source pairs for `Pipe`, `Lava`, `Trampoline`, `Cannon`, `Bullet`, `Rocket`, and `WinFlag` under `include/Objects/Environment/` and `src/Objects/Environment/`.
- P2 Block files: `include/Objects/Blocks/MovingBlock.hpp`, `src/Objects/Blocks/MovingBlock.cpp`.
- Focused P2 test file: `tests/Person2RuntimeContracts.cpp`. Preserve existing assertions; append the smallest checks that establish new contracts.

Allowed does not mean every task should touch every file. Each task card narrows its exact target set. The minimum root-cause diff wins after tracing all callers.

## 5. Files read-only to Person 2

- P1: `src/main.cpp`, `include/Core/MyApp.hpp`, any future `src/Core/MyApp.cpp`, `include/Core/GameEventMediator.hpp`, `src/Core/GameEventMediator.cpp`, every file under `include/States/` and `src/States/`.
- P3: `include/Core/GameObject.hpp`, `src/Core/GameObject.cpp`, every production header/source under `Entities/`, and every production header/source under `Objects/Items/`.
- P4: `include/Components/AnimationComponent.hpp`, `src/Components/AnimationComponent.cpp`; `include/Core/UserData.hpp`, `src/Core/UserData.cpp`; all Block files except `MovingBlock`; all Audio/UI files; `LeaderboardState` files.
- Shared/release: `include/Core/Constants.hpp` is read-only unless a cross-owner edit request is accepted; `CMakeLists.txt`, runtime asset-copy rules, save files, and release layout belong to P4 packaging.
- All `assets/**`, including all nine level PNGs, are read-only. Known map-data findings remain user-owned `KNOWN_MAP_EDIT_ITEM`s. Do not inspect image contents as part of any source task.
- Group5 is read-only reference material. Never copy its raw-pointer object ownership, mediator-held owners, 64-pixel assumptions, chatbot/LLM/Ollama code, or whole files.

If P2 needs a read-only file changed, create a cross-owner edit request containing requester, target owner, exact file/symbol, reason, proposed API or patch, contracts/tasks affected, compatibility risk, tests, and desired merge order. The target owner implements or explicitly delegates; P2 does not edit first and ask later.

## 6. Inputs P2 consumes

| Input | Provider / owner | Lifetime and timing | Consuming tasks |
|---|---|---|---|
| Stable non-owning `PlayerManager*` view and selected world/level | P1 owns the player under resolved `DEC-PLAYER-OWNER`; P3 implements player | Player outlives active P2 level and is injected before load/update | `P2-LOAD-001`, bounds, pit, contacts, hazards, completion |
| `DEC-ASSET-ROOT` | Human | `RESOLVED`; consume the approved executable-adjacent asset policy | `P2-LOAD-001`, `P2-NINE-LEVEL-001` |
| `DEC-MULTIPLAYER-DEATH`, `DEC-RESPAWN` | Human | `RESOLVED`; consume the approved 1P death/respawn policy | `P2-PIT-001` |
| `DEC-BLOCK-ACTOR-ELIGIBILITY` | Human | `RESOLVED`; consumed before construction/contact behavior is frozen | `P2-VARIANT-WIRE-001`; enemy-driven block work if later required |
| `DEC-WINFLAG-POLISH` | Human | `RESOLVED`; completion waits for bounded flag animation | `P2-WINFLAG-001` |
| P3 factory, contact, damage, stomp/shell, item-collection, lifecycle, reset, and attack-request APIs | P3 | Borrowed player; P2-owned enemies/items/projectiles; construction during load, contacts after movement, cleanup after traversal | load, contact, pit, cleanup, projectile tasks |
| P4 Block lifecycle/activation, variant constructors, spawn callbacks, and current 64x64 visual-size invariant | P4 | P2 owns Block objects and immediately adopts callback spawns | physics, variants, environment, nine-level tasks |
| P1 callback/event consumers | P1 | Callbacks must not outlive GameState; state replacement is deferred until update returns | death, completion, contact outcomes |
| Dynamic map metadata | P2 `MapManager` | Valid only after successful load and while manager exists | extent, bounds, pit, cleanup, camera contract |

## 7. Outputs and integration contracts P2 provides

| Contract | P2 role | Output, direction, timing, failure behavior | Status / dependent tasks |
|---|---|---|---|
| `CON-P1-P2-ACTIVE-LEVEL` | Provider of level object/update/render; borrower of players | P1 owns the active level; selected players remain with the owner approved by `DEC-PLAYER-OWNER` (P1 recommended), while P2 stores only non-owning views. Inject before load; no callback/state replacement during traversal. | PARTIAL; load, pit, contacts, nine-level |
| `CON-P1-P2-LEVEL-LOAD` | Provider | Explicit load/readiness plus diagnostic after construction/load. P1 may update/render only when ready. Failure leaves a safe inert level and reports error; it must not look playable. | MISSING; `P2-LOAD-001` |
| `CON-P1-P2-WORLD-EXTENT` | Provider | Immutable pixel extent derived after successful parse from dynamic width × `MapFormat::TILE_SIZE` and one logical 15-row world band. Invalid before successful load; expose validity rather than a plausible zero rectangle. | P2 provider source/runtime complete; P1 camera consumption pending |
| `CON-P1-P2-CAMERA` | Extent provider only | P1 queries P2 extent after load and clamps/follows per frame after movement. P2 does not own `sf::View`. | MISSING; extent; P1 camera task |
| `CON-P1-P2-DEATH` | Trigger provider | Borrowed player identity emitted once for a fatal hazard/contact or position Y beyond `worldExtent.bottom + TILE_SIZE`; P1 defers transition. P2 never decrements lives or chooses respawn. | PARTIAL; lava, pit, enemy/projectile contacts |
| `CON-P1-P2-COMPLETION` | Trigger provider | Emit one completion signal after valid WinFlag activation; P1 defers state replacement. Failed/unready levels cannot complete. | PARTIAL; WinFlag and P1 flow |
| `CON-P2-P3-ENTITY-CONSTRUCTION` | Spawn-data provider/runtime owner | P2 passes validated type and position to P3 factory, accepts exactly one non-null `unique_ptr`, then registers views. Invalid/null construction diagnoses and fails load rather than silently omitting an object. | PARTIAL; load, factory, nine-level |
| `CON-P2-P3-PLAYER-ENEMY` | Orchestrator/consumer | P2 classifies contact after movement, invokes typed P3 result exactly once per applicable contact, applies bounce/velocity result, and forwards outcome without owning P3 behavior policy. | MISSING; contact-enemy |
| `CON-P2-P3-PLAYER-ITEM` | Orchestrator/consumer | P2 detects overlap after movement/before cleanup and supplies safe-growth clearance for the current 64×128 powered bounds; P3 returns outcome; denied growth leaves player/item unchanged and inactive items cannot collect twice. | MISSING; contact-item |
| `CON-P2-P3-ENEMY-REMOVAL` | Runtime owner | P2 removes owned inactive/dead/off-world enemies only after traversal and rebuilds views; P3 supplies lifecycle predicate. No dangling view may survive rebuild. | PARTIAL; cleanup |
| `CON-P2-P3-PROJECTILE-SPAWN` | Attack-request consumer/runtime owner | P2 accepts a P3 value/move-only request after update, constructs/adopts one unique owner, registers after traversal, and rejects invalid requests without a raw/partial owner. | MISSING; projectile, boss, fire |
| `CON-P2-P4-BLOCK-COLLISION` | Consumer/orchestrator | P2 supplies side and, once agreed, actor context; P4 owns reaction rules. Inactive Block is skipped immediately. | PARTIAL; contact/variant tasks; decision open |
| `CON-P2-P4-BLOCK-LIFECYCLE` | Consumer/runtime owner | P4 marks existence; P2 skips same-frame and erases owner after traversal. | `DONE` frozen baseline |
| `CON-P2-P4-SPAWN-HANDOFF` | Runtime owner/consumer | P4 callback supplies a fresh object exactly once; P2 immediately wraps it in one `unique_ptr`, queues during traversal, then registers views. Null/duplicate/unsupported spawn fails safely. | `DONE` structurally; payload behavior still incomplete |
| `CON-P2-P4-BLOCK-VARIANTS` | Metadata provider/constructor consumer | Parsed variant reaches the agreed P4 constructor/factory without reinterpretation. Unknown variants produce a diagnostic/fallback agreed by owners, never silent defaulting. | MISSING; variant-wire |
| `CON-P2-P4-BLOCK-VISUAL-SIZE` | Construction consumer | P2 calls the P4-owned invariant that sets a 64×64 hitbox and scales 16×16 art exactly once before first render. | MISSING; variant/nine-level; P4 provider task |

## 8. Per-task implementation notes

The following are persistent task cards. Requirement labels are `REQUIRED_BY_PLAN`, `REQUIRED_BY_TEAM`, or `IMPLIED_BY_CURRENT_GROUP4_DESIGN`. Human decisions named here use the status recorded in `03_HUMAN_DECISIONS.md`.

### Verified implementation batch — 2026-08-28

- Implementation commit: `419d03d` (`person2: integrate verified map runtime contracts`).
- Fresh C++20/SFML 3.1 build: `SuperMario` and `Person2RuntimeContracts` both linked successfully.
- Current CTest result: `1/1` passed. Direct runtime result after bounds/AABB/environment work: `64 PASS / 0 FAIL`; the original 27 ownership/lifecycle checks remain green.
- Latest execution checkpoint (`build`, 2026-08-29): a fresh rebuild of `Person2RuntimeContracts`, its CTest run, and a fresh `SuperMario` link all completed successfully.
- Non-blocking external warning: P4-owned `BrickFragment::reactToCollision(int)` intentionally performs no collision work and therefore does not use `collidedSide`. This is an `-Wunused-parameter` warning only; the BrickFragment lifecycle contract remains green and it is not a P2 blocker.
- Source/runtime evidence is recorded below for extent, pit, Lava, environment contracts, projectile collision/culling, and WinFlag completion.
- These seven cards are `DONE` for P2 source/runtime scope. P1/P3/P4 consumption and applicable visual/gameplay acceptance remain recorded independently and do not reopen the completed P2 providers.
- All nine Group4 PNGs parse through the SFML 3.1 loader with zero unknown colors in the two runtime bands. This is load smoke evidence, not production render/gameplay acceptance.

### `P2-EXTENT-001` — Expose explicit dynamic world extent

- **Owner / priority / requirement / status:** Person 2; P0; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE`.
- **Purpose:** Make one authoritative pixel-world boundary available to physics, pit cleanup, and P1 camera code.
- **Exact current behavior:** `MapManager::getWorldBounds()` derives an `sf::FloatRect` from parsed dynamic width and one 15-row gameplay band; `LevelManager::getWorldBounds()` exposes it only after a successful load. The runtime suite covers loaded/unloaded behavior and real map width.
- **Missing checklist:** [x] value extent API; [x] dynamic pixel width; [x] one-band logical height; [x] readiness-aware exposure; [x] explicit units; [x] runtime checks; [ ] P1 camera consumes the value in the production flow; [ ] gameplay edge verification.
- **Exact target files:** `include/Levels/Managers/MapManager.hpp`, `src/Levels/Managers/MapManager.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact target files only. **Read-only dependencies:** `include/Core/Constants.hpp`; P1 GameState; map PNGs. **Do not implement:** camera, player reset, asset-root policy, or map-content fixes.
- **Dependencies:** existing parser validation. **Provides:** the tested P2 provider half of `CON-P1-P2-WORLD-EXTENT` and `CON-P1-P2-CAMERA`. **Consumes:** successful-map-load state and `MapFormat::TILE_SIZE`.
- **Group5 reference / do-not-copy:** The approved Group5-compatible sketch coordinates are reused; Group5 ownership and manager architecture are not.
- **Notes / SFML:** Prefer a simple value type already used by the code (`sf::FloatRect` only if its SFML 3.1 API fits cleanly); avoid a new abstraction/dependency. Preserve dynamic width.
- **Checks:** C++20/SFML 3.1 full build passed; fresh unified runtime suite passed. Visual N/A; P1 must still verify camera clamping against the provided value.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** authoritative valid extent is tested and consumed without changing ownership or map encoding. **Suggested commit:** `p2: expose map-derived world extent`.

### `P2-LAVA-001` — Correct Lava fatal-death semantics

- **Owner / priority / requirement / status:** Person 2; P0; `REQUIRED_BY_PLAN`; `DONE`.
- **Purpose:** Ensure Lava reports death only when the player actually reaches the agreed fatal state, once.
- **Exact current behavior:** `Lava::applyTo` returns the actual fatal result. LevelManager emits death only for a dead player, breaks later hazard/contact processing, and preserves Big/Fire downgrade behavior.
- **Missing checklist:** [x] fatal result; [x] callback only on death; [x] duplicate suppression; [x] later-contact exclusion; [x] powered downgrade; [x] focused small/Big/Fire checks; [ ] production death transition; [ ] visual/gameplay verification.
- **Exact target files:** `include/Objects/Environment/Lava.hpp`, `src/Objects/Environment/Lava.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact target files only. **Read-only dependencies:** P3 `PlayerManager` header/source and P1 callback consumer. **Do not implement:** lives, death menu, respawn, multiplayer policy, or P3 damage rules.
- **Dependencies:** current damage/death predicate and `CON-P1-P2-DEATH`. **Provides:** correct Lava source for `CON-P1-P2-DEATH`. **Consumes:** borrowed player and P3 fatal-state result.
- **Group5 reference / do-not-copy:** Group5 forces health to zero for pits, not evidence for Lava policy. Do not copy direct state/menu mutation.
- **Notes / SFML:** no SFML rendering API changes expected; retain SFML 3.1 syntax. Root cause belongs at hazard/result boundary, not in P1.
- **Checks:** compile/syntax-check affected units with C++20/SFML 3.1; runtime small-player fatal event exactly once, powered-player nonfatal downgrade no death event, inactive/dead player excluded later that frame; visual verify existing Lava animation still renders; gameplay verify eventual death transition once.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** callback truth matches player fatality with no duplicate delivery. **Suggested commit:** `p2: emit lava death only for fatal damage`.

### `P2-WINFLAG-001` — Honor base-anchor markers and completion trigger

- **Owner / priority / requirement / status:** Person 2; P0; `REQUIRED_BY_PLAN` plus `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE`.
- **Purpose:** Interpret the team's white marker as the flag base/activation anchor and preserve one-shot completion.
- **Exact current behavior:** the parser coordinate is the base/activation anchor; WinFlag geometry extends upward, activation starts a bounded one-second slide, and its callback queues one deferred LevelManager completion only after the slide finishes.
- **Missing checklist:** [x] base-anchor geometry; [x] upward collision/visual geometry; [x] bounded animation; [x] exactly-once deferred callback; [x] unready-level guard; [x] runtime math/timing checks; [ ] P1 production win transition; [ ] visual/gameplay verification.
- **Exact target files:** `include/Objects/Environment/WinFlag.hpp`, `src/Objects/Environment/WinFlag.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact target files only. **Read-only dependencies:** `src/Levels/Managers/MapManager.cpp` unless source evidence shows parser must pass an explicit anchor tag; P1 GameState; map PNGs. **Do not implement:** WinMenu, unlock/save flow, map marker moves, or P4 art.
- **Dependencies:** `CON-P1-P2-COMPLETION`; resolved `DEC-WINFLAG-POLISH` selects bounded animation-gated completion. **Provides:** the tested P2 completion provider. **Consumes:** parser world coordinate.
- **Group5 reference / do-not-copy:** Reuse base-anchor and bounded-slide behavior adapted to `MapFormat::TILE_SIZE`; do not copy raw pointers, global mediator coupling, or state ownership.
- **Notes / SFML:** use SFML 3.1 shapes/rect APIs already present; geometry follows `MapFormat::TILE_SIZE`, never a map-specific magic coordinate.
- **Checks:** compile/syntax-check affected units with C++20/SFML 3.1; runtime marker-to-top/activation rectangle math and callback exactly once; visual verify pole extends upward from an approved marker in a disposable source-controlled fixture, not by editing a map; gameplay verify completion reaches P1 later.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** base marker activates the correctly placed flag and emits one safe completion event. **Suggested commit:** `p2: anchor win flags at map base markers`.

### `P2-ENV-001` — Verify Pipe, Trampoline, and MovingBlock integration

- **Owner / priority / requirement / status:** Person 2; P1; `REQUIRED_BY_PLAN`; `DONE`.
- **Purpose:** Turn source-only/historical evidence for three implemented environment components into current focused contract evidence.
- **Exact current behavior:** Pipe dimensions and Trampoline per-player launch are registered through the existing P2/P4 physics path. MovingBlock consumes full frame distance through linear endpoints and every square-path corner without a pause, and exposes the net frame delta for rider carry. CloudPlatform remains excluded by decision.
- **Missing checklist:** [x] Pipe dimensions/collision; [x] Trampoline launch/no shared debounce; [x] horizontal/vertical travel limits; [x] carry delta; [x] lifecycle/view registration; [x] tuning ceiling documented; [ ] production visual alignment; [ ] stand/launch/ride gameplay pass.
- **Exact target files:** `include/Objects/Environment/Pipe.hpp`, `src/Objects/Environment/Pipe.cpp`, `include/Objects/Environment/Trampoline.hpp`, `src/Objects/Environment/Trampoline.cpp`, `include/Objects/Blocks/MovingBlock.hpp`, `src/Objects/Blocks/MovingBlock.cpp`, `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact target files only; make no production change if tests pass. **Read-only dependencies:** P3 movement/player APIs; P4 Block base; map PNGs. **Do not implement:** P4 `CloudBlock`, Cloud marker mapping, textures, camera, or map tuning.
- **Dependencies:** frozen block lifecycle and movement baseline. **Provides:** verified environment collision/movement behavior. **Consumes:** `CON-P2-P4-BLOCK-COLLISION` and `CON-P2-P4-BLOCK-LIFECYCLE`.
- **Group5 reference / do-not-copy:** reference unnecessary. Do not port Group5 geometry or ownership.
- **Notes / SFML:** headless logic checks first; visual inspection later with SFML 3.1. Preserve existing code if checks establish it.
- **Checks:** compile all exact targets; runtime assertions above; visual confirm primitive/collider alignment at the configured cell size; gameplay verify player can stand on Pipe, launch once, and ride both platform directions.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** focused automated and later visual/gameplay evidence passes without scope creep. **Suggested commit:** `test(p2): cover environment movement contracts`.

### `P2-PROJECTILE-001` — Complete Cannon, Bullet, and Rocket production paths

- **Owner / priority / requirement / status:** Person 2; P1; `REQUIRED_BY_PLAN`; `DONE`.
- **Purpose:** Preserve sound projectile ownership while making projectiles constructible, collidable, and bounded in the real runtime.
- **Exact current behavior:** LevelManager exposes owned homing and straight Rocket spawn paths. Cannon spawns through deferred unique ownership without a birth-frame update. Bullet/Rocket use configurable lifetimes, swept-AABB Block collision over the full frame path, map-derived extent culling, once-only player damage, and deferred inactive cleanup.
- **Missing checklist:** [x] production Rocket entry points; [x] immediate unique ownership; [x] Block collision and extent culling; [x] once-only player damage/death; [x] hit/expiry/target-loss/off-world removal; [x] focused runtime checks; [ ] P3 boss/fire callers consume the spawn surface; [ ] production visual/gameplay verification.
- **Exact target files:** `include/Objects/Environment/Cannon.hpp`, `src/Objects/Environment/Cannon.cpp`, `include/Objects/Environment/Bullet.hpp`, `src/Objects/Environment/Bullet.cpp`, `include/Objects/Environment/Rocket.hpp`, `src/Objects/Environment/Rocket.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets only. **Read-only dependencies:** P3 boss/player/fire files, P4 block files, P1 mediator/state. **Do not implement:** Bowser/Petey attacks, FireBuff shooting, projectile sprites/audio, or P3 internals.
- **Dependencies:** `P2-EXTENT-001` for final culling; frozen `BASE-P2-OWNERSHIP-001` and `BASE-P2-ROCKET-001`; death/block contracts. **Provides:** P2 runtime half of `CON-P2-P3-PROJECTILE-SPAWN`. **Consumes:** P3 value/move-only attack request, borrowed target identity only through a safe resolver, and P3 damage result; projectile culling is internal P2 cleanup, not `CON-P2-P3-ENEMY-REMOVAL`.
- **Group5 reference / do-not-copy:** no architecture copy; Group4's unique ownership and target resolver are the required baseline.
- **Notes / SFML:** no new dependency; preserve SFML 3.1 vectors/rect APIs. Prefer one generic spawn hook over separate caller-specific hacks.
- **Checks:** compile; runtime Cannon/Bullet existing checks remain green, Rocket production spawn, target loss, block/world hit, off-world and once-only damage; visual verify trajectory/collider alignment; gameplay verify Cannon and one Rocket source without dangling targets.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** each projectile has a reachable production source and deterministic safe removal while all frozen lifetime checks remain green. **Suggested commit:** `p2: complete projectile spawn and culling paths`.

### `P2-LOAD-001` — Make stage path and load readiness explicit

- **Owner / priority / requirement / status:** Person 2; P0; `REQUIRED_BY_PLAN` and `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `BLOCKED`.
- **Purpose:** Prevent a failed wrapper from masquerading as a valid empty level.
- **Exact current behavior:** all wrappers select the correct `assets/textures/LevelSketch_W*_LV*.png` name through `ConfiguredLevel`; `LevelManager::isLoaded()` and parser diagnostics exist, and all nine real PNGs parse. Path resolution still guesses the current working directory and therefore does not satisfy the executable-adjacent asset-root decision.
- **Missing checklist:** [x] resolve `DEC-ASSET-ROOT`; [x] generic nine-name construction; [x] explicit ready state; [ ] consume one shared executable-root resolver; [ ] expose the diagnostic cleanly to P1; [ ] make failed-level update/render explicitly inert; [ ] complete valid/missing/invalid/reload integration checks.
- **Exact target files:** `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, all 18 exact stage wrapper header/source files listed in §4 (verification; edit only if the generic fix cannot cover them), `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** `CMakeLists.txt`, asset-copy/package rules, P1 GameState, all PNGs. **Do not implement:** P1 load-error UI, P4 packaging, asset moves/copies, or map-data corrections.
- **Dependencies:** resolved `DEC-ASSET-ROOT`, accepted `CON-P1-P2-LEVEL-LOAD`, `CON-P2-P3-ENTITY-CONSTRUCTION`. **Provides:** readiness/diagnostic to P1 and nine wrappers. **Consumes:** a stable player view from the P1 owner, P3 factory result, and the approved runtime asset root.
- **Group5 reference / do-not-copy:** Group5 location is evidence, not an asset-layout mandate. Do not copy its loader/owner architecture.
- **Notes / SFML:** use `std::filesystem` if path joining is needed; no dependency. Do not use process working-directory guesses after the decision is fixed.
- **Checks:** compile/syntax-check all manager/wrapper units with C++20/SFML 3.1; runtime missing path = false/inert/diagnostic, valid fixture = ready, failed reload leaves specified safe state; visual/gameplay later P1 shows failure and valid stage only.
- **Completion dimensions:** `SOURCE_DONE=NO`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=NO`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** every wrapper reports truthful readiness from one path policy and P1 can branch safely. **Suggested commit:** `p2: expose stage load readiness`.

### `P2-BOUNDS-001` — Enforce player/world horizontal bounds

- **Owner / priority / requirement / status:** Person 2; P0; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE`.
- **Purpose:** Keep borrowed players inside the loaded dynamic-width world without substituting window-size constants.
- **Exact current behavior:** LevelManager consumes valid dynamic extent after player Block physics, clamps each active player to the legal left/right hitbox range, cancels only outward X velocity, and preserves Y for pit fall.
- **Missing checklist:** [x] consume valid extent; [x] resolve left and right overflow with hitbox size; [x] cancel outward velocity consistently; [x] handle worlds narrower than viewport safely; [x] avoid constraining vertical pit fall; [x] test multiple widths and high-speed overshoot; [ ] visual no-jitter check; [ ] production gameplay edge pass.
- **Exact target files:** `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** P3 `PlayerManager`/movement, P1 camera, map assets. **Do not implement:** `sf::View`, menu transitions, teleport/respawn, or hard-coded 800/1920 bounds.
- **Dependencies:** the tested `P2-EXTENT-001` provider and `CON-P1-P2-WORLD-EXTENT` are available. **Provides:** bounded player world position. **Consumes:** hitbox and movement velocity.
- **Group5 reference / do-not-copy:** adapt clamping against map width only; do not copy Group5 window/cell constants.
- **Notes / SFML:** follow SFML 3.1 rectangle/vector APIs already used. Resolve once at the shared physics/runtime boundary rather than per player class.
- **Checks:** compile; runtime left/right, exact-edge, overshoot, two dynamic widths, narrow world; visual verify no jitter; gameplay verify camera/player reach both legal edges.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** every active borrowed player remains within the valid horizontal world and existing block physics stays green. **Suggested commit:** `p2: enforce map-derived player bounds`.

### `P2-PIT-001` — Detect map-derived pit falls and emit death once

- **Owner / priority / requirement / status:** Person 2; P0; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE`.
- **Purpose:** End unbounded falling using loaded-world data while leaving lives/respawn policy to P1/P4/P3.
- **Exact current behavior:** after player movement/physics, LevelManager compares the hitbox against the loaded logical-world bottom plus one map cell, marks a fatal fall once, emits the borrowed-player callback once, and skips later same-frame contacts.
- **Missing checklist:** [x] valid world bottom; [x] one-cell margin; [x] post-physics timing; [x] repeated-frame suppression through dead state; [x] later-contact exclusion; [x] no lives/state ownership; [ ] P1/P3 fatal reload/reset clears the attempt in production; [ ] gameplay verification.
- **Exact target files:** `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** P1 GameState/Mediator, P3 PlayerManager/reset, P4 UserData, map assets. **Do not implement:** shared/separate lives, respawn construction, DeathMenu, save changes, or direct health reset.
- **Dependencies:** `P2-EXTENT-001`; approved 1P death/respawn policy; `CON-P1-P2-DEATH`, `CON-P1-P2-ACTIVE-LEVEL`. **Provides:** single-player fatal event. **Consumes:** borrowed player and valid extent.
- **Group5 reference / do-not-copy:** adapt map-derived threshold plus one tile. Do not copy direct `setHealth(0)`, raw owners, reset flow, 64-pixel formula, or menu mutation.
- **Notes / SFML:** pure runtime logic. Multiplayer behavior is out of the current release scope.
- **Checks:** compile; runtime threshold just above/equal/below, one callback across repeated frames, two identities per chosen policy, no later same-frame contact; visual verify fall leaves view as expected; gameplay verify P1 deferred death flow and agreed respawn.
- **Completion dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** map-derived fall emits exactly the contractually correct affected-player event and never owns transition policy. **Suggested commit:** `p2: detect map-derived pit deaths`.

### `P2-CLEANUP-001` — Remove off-world owned runtime objects safely

- **Owner / priority / requirement / status:** Person 2; P1; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `BLOCKED`.
- **Purpose:** Prevent enemies, items, and projectiles that leave the dynamic world from living forever in owner/view collections.
- **Exact current behavior:** inactive specialized objects are removed after traversal and views rebuild. Bullet/Rocket now cull through map-derived extent and the existing inactive cleanup; enemy/item off-world policy still lacks the P3 lifecycle predicates needed for safe removal.
- **Missing checklist:** [x] projectile extent margin; [x] projectile lifecycle marking; [x] deferred removal/view rebuild; [ ] enemy/item category margins; [ ] P3 inactive-removal API; [ ] legal-airborne retention; [ ] full category boundary/runtime and gameplay checks.
- **Exact target files:** `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`; projectile files from `P2-PROJECTILE-001` only for their P2 lifecycle predicates.
- **Allowed files:** exact targets. **Read-only dependencies:** all P3 enemy/item files, P4 BrickFragment/Block files. **Do not implement:** P3/P4 lifecycle APIs, camera culling, or delete raw view pointers directly.
- **Dependencies:** `P2-EXTENT-001`, `CON-P2-P3-ENEMY-REMOVAL`; accepted P3 item/enemy predicate. **Provides:** safe owner removal/view rebuild. **Consumes:** extent and provider lifecycle state.
- **Group5 reference / do-not-copy:** Group5 has no equivalent cleanup and is not a model. Preserve Group4 `unique_ptr` superiority.
- **Notes / SFML:** no visual-frustum culling; world-lifecycle cleanup is distinct. If a missing P3 predicate is required, file a cross-owner request.
- **Checks:** compile; runtime each category crosses each relevant boundary, erased after traversal, no dangling view, legal airborne case retained; visual N/A; gameplay long fall/spawn session shows stable object counts.
- **Completion dimensions:** `SOURCE_DONE=NO`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=NO`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** owned off-world actors are removed deterministically without violating update iteration or ownership. **Suggested commit:** `p2: clean up off-world runtime objects`.

### `P2-CONTACT-ENEMY-001` — Orchestrate player/enemy contacts

- **Owner / priority / requirement / status:** Person 2; P0; `REQUIRED_BY_PLAN`; `BLOCKED`.
- **Purpose:** Connect P3 enemy/player behavior to P2's movement/contact order without moving behavior ownership.
- **Exact current behavior:** P2 applies generic block physics to selected enemies and borrowed players and now has a tested previous/current AABB side classifier, but no player/enemy loop consumes it. P3 exposes fragmented damage, stomp, and shell methods; no integrated result contract exists.
- **Missing checklist:** [ ] agree typed contact result; [ ] broad-phase active pairs; [ ] classify stomp vs harmful side/bottom; [ ] invoke P3 once/contact; [ ] apply bounce/shell motion; [ ] respect invulnerability/dead state; [ ] suppress inactive entities later same frame; [ ] forward score/death outcome without owning it.
- **Exact target files:** `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** every P3 player/enemy file, P1 mediator, P4 UserData/HUD. **Do not implement:** enemy state logic, scoring, animation, lives, boss attacks, or edit P3 to fit an ad hoc cast.
- **Dependencies:** accepted `CON-P2-P3-PLAYER-ENEMY`; P3 source tasks; P1/P4 outcome sinks. `DEC-BLOCK-ACTOR-ELIGIBILITY` remains OPEN for any shell/enemy-to-block extension, which is outside the minimum contact task.
- **Provides:** contact classification/order. **Consumes:** borrowed players, P2-owned enemies, P3 behavior result.
- **Group5 reference / do-not-copy:** reuse interaction ordering as a checklist only; do not copy GameEventMediator raw-pointer orchestration.
- **Notes / SFML:** AABB/contact math must use SFML 3.1 rect API and previous/current motion, not render bounds. Avoid O(n²) abstraction unless current entity counts justify a documented `ponytail:` ceiling.
- **Checks:** compile; runtime 1P Goomba stomp/side, Heriss harmful stomp, Koopa shell transitions, invulnerability and inactive removal/order; visual verify bounce/contact alignment; gameplay verify score/death handoff once.
- **Completion dimensions:** `SOURCE_DONE=NO`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=NO`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** representative required enemy contacts produce correct P3 outcomes with deterministic P2 ordering and no duplicate events. **Suggested commit:** `p2: orchestrate player enemy contacts`.

### `P2-CONTACT-ITEM-001` — Orchestrate player/item collection

- **Owner / priority / requirement / status:** Person 2; P0; `REQUIRED_BY_PLAN`; `BLOCKED`.
- **Purpose:** Make P3 Coin/Mushroom/power-up objects collectible exactly once in the owned runtime.
- **Exact current behavior:** LevelManager owns and removes inactive power-ups; it never calls `onCollect`. The feet-anchored `canGrow` clearance primitive has a focused low-ceiling test, but Coin/Mushroom provider behavior is partial and no score/coin event is integrated.
- **Missing checklist:** [ ] active 1P player/item overlap after movement; [ ] P3 typed collection outcome; [ ] one-shot deactivation; [ ] Mushroom/block world physics where contract requires; [ ] queue-safe removal; [ ] outcome forwarded to P1/P4 via owner contract.
- **Exact target files:** `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** P3 item/player/buff files, P1 mediator, P4 UserData/HUD/block payloads. **Do not implement:** score amount, coin-to-life policy, buff internals, payload animation, HUD, or persistence.
- **Dependencies:** accepted `CON-P2-P3-PLAYER-ITEM`; P3 collection result; outcome sink. **Provides:** contact timing and owner cleanup. **Consumes:** P3 item behavior and player identity.
- **Group5 reference / do-not-copy:** Group5 overlap order may be consulted; do not copy its raw vectors or mediator coupling.
- **Notes / SFML:** use hitboxes, not sprite bounds. The current release has one active player, so no simultaneous-2P collection rule is required.
- **Checks:** compile; runtime 1P Coin/Mushroom/power-up exactly once, inactive item skipped, and spawned-during-frame pickup deferred consistently; visual verify item contact alignment; gameplay verify score/state/HUD effects after cross-owner integration.
- **Completion dimensions:** `SOURCE_DONE=NO`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=NO`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** every required collectible routes one P3 outcome through safe P2 ownership and disappears once. **Suggested commit:** `p2: integrate player item collection`.

### `P2-VARIANT-WIRE-001` — Wire Block, payload, and Cloud construction variants

- **Owner / priority / requirement / status:** Person 2; P1; `REQUIRED_BY_PLAN`; `BLOCKED`.
- **Purpose:** Preserve parsed variant metadata through construction without implementing P4 behavior in P2.
- **Exact current behavior:** `MapManager` emits Brick/MushroomBlock variants, but `LevelManager::constructSpawn` ignores them. CloudPlatform always creates P2 MovingBlock; P4 CloudBlock is empty and separately required.
- **Missing checklist:** [x] Cloud excluded for this release; [ ] accept P4 constructor/factory surface; [ ] pass Brick/MushroomBlock variants unchanged; [ ] connect P3 payload type where specified; [ ] retain immediate spawn adoption; [ ] diagnose unsupported variants; [ ] call P4 64×64 invariant.
- **Exact target files:** `include/Levels/Managers/MapManager.hpp`, `src/Levels/Managers/MapManager.cpp` only if metadata shape must change; `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`; `include/Objects/Blocks/MovingBlock.hpp`, `src/Objects/Blocks/MovingBlock.cpp` only for the chosen distinct MovingBlock mapping; `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** P4 Brick/CoinBlock/MushroomBlock/CloudBlock/Block files; P3 payload/item files; all maps. **Do not implement:** payload behavior, CloudBlock, actor eligibility rules, map recoloring, or P4 visual scaling.
- **Dependencies:** resolved `DEC-BLOCK-ACTOR-ELIGIBILITY`; deferred CloudPlatform is excluded; `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF`, `CON-P2-P4-BLOCK-VISUAL-SIZE`; P4/P3 providers.
- **Provides:** faithful construction metadata and owner adoption. **Consumes:** P4 construction API and P3 payload type.
- **Group5 reference / do-not-copy:** Group5 demonstrates preserving variants, not the required class design. Do not copy its constructors/raw owners.
- **Notes / SFML:** no SFML-specific change expected beyond maintaining 3.1 compatibility. Unknown colors/variants must not silently become the wrong gameplay object.
- **Checks:** compile; runtime each supported variant produces the expected dynamic type/config and callback ownership; visual verify 64×64 variant art; gameplay verify representative payload behavior; Cloud is excluded and MovingBlock remains independent; no PNG edit in this task.
- **Completion dimensions:** `SOURCE_DONE=NO`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=NO`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** parsed variants reach their owners' behavior intact, with explicit unsupported handling and unchanged `unique_ptr` ownership. **Suggested commit:** `p2: wire parsed block variants into runtime`.

### `P2-NINE-LEVEL-001` — Make shared engine code ready for exactly nine wrappers

- **Owner / priority / requirement / status:** Person 2; P1; `REQUIRED_BY_TEAM` and `REQUIRED_BY_PLAN`; `BLOCKED`.
- **Purpose:** Establish that one shared engine path supports W1_LV1 through W3_LV3 without nine copies of logic.
- **Exact current behavior:** all 18 wrapper files compile; each wrapper maps to its exact filename; all nine PNGs pass SFML 3.1 parse smoke with zero unknown colors in the runtime bands; extent and pit checks pass. Bounds, contacts, variants, production rendering and state transitions still prevent gameplay acceptance.
- **Missing checklist:** [ ] shared executable-root load path; [x] exact wrapper identities; [x] nine parse smokes; [x] extent/pit runtime; [ ] bounds; [ ] enemy/item contacts; [ ] terrain/payload variants; [ ] P1 production ownership/transitions; [ ] nine visual/gameplay records; [ ] `W3_LV3` boss-defeated completion exactly once.
- **Exact target files:** `include/Levels/Managers/MapManager.hpp`, `src/Levels/Managers/MapManager.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, all 18 stage files listed in §4, `tests/Person2RuntimeContracts.cpp`.
- **Allowed files:** exact targets. **Read-only dependencies:** all P1 GameState/menu, P3 entity/item, P4 block/UI/package files, all nine PNGs. **Do not implement:** per-level hard-coded behavior, map pixel fixes, player visuals, menus/HUD, audio, persistence, or packaging.
- **Dependencies:** completion of applicable P2 P0 tasks, load/extent/contracts, P1 playable flow, P3/P4 providers. **Provides:** shared code readiness and validation handoff. **Consumes:** all canonical P1/P2 and P2/P3/P4 contracts.
- **Group5 reference / do-not-copy:** behavior reference only. No Group5 engine files, ownership graph, map dimensions, or chatbot code.
- **Notes / SFML:** all nine use C++20/SFML 3.1, 64-pixel cells, two runtime bands, and one ignored legacy band. Wrapper-specific content may remain configuration; shared bugs are fixed once in managers.
- **Checks:** compile/syntax-check managers plus 18 wrappers with C++20/SFML 3.1; runtime wrapper identity/readiness/extent smoke per validation ID; visual and gameplay gates recorded separately below; no map content audit is authorized.
- **Completion dimensions:** `SOURCE_DONE=PARTIAL`; `INTEGRATION_DONE=NO`; `RUNTIME_TESTED=PARTIAL`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **Definition of done:** all nine wrapper entries pass their own validation card through one shared runtime, with any map-data issue recorded only as user-owned `KNOWN_MAP_EDIT_ITEM`. **Suggested commit:** `p2: validate shared runtime across nine levels`.

### Frozen baseline cards

#### `BASE-P2-OWNERSHIP-001` — Unique ownership, typed views, and spawn adoption

- **Owner / priority / requirement / status:** Person 2; P0 baseline; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE` — never reopen without new failing evidence.
- **Purpose / current behavior:** `LevelManager` has one `unique_ptr<GameObject>` owner list, non-owning typed views, pending spawn adoption, and safe view rebuild after erase. Cannon/Bullet and P4 callbacks use the handoff.
- **Missing checklist:** none within frozen scope.
- **Exact target / allowed files:** `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`; changes allowed only when a new failing regression proves necessity. **Read-only dependencies:** P3 entities/items and P4 spawn providers. **Do not implement:** raw-pointer owners or provider-side retained ownership.
- **Dependencies / contracts:** provides `CON-P2-P4-SPAWN-HANDOFF` and owner half of `CON-P2-P3-ENEMY-REMOVAL`; consumes fresh callback object. Group5 raw ownership is `DO_NOT_COPY_ARCHITECTURE`.
- **Notes / SFML / checks:** architecture is SFML-independent; C++20 syntax and existing ownership/spawn/remove runtime assertions remain required. Visual/gameplay not part of frozen scope.
- **Dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO` because no production gameplay flow exists.
- **DoD / commit:** existing 27 checks stay green; no new commit is suggested unless repairing a proven regression (`p2: preserve runtime ownership contract`).

#### `BASE-P2-LIFECYCLE-001` — Physics participation and inactive-owner removal

- **Owner / priority / requirement / status:** Person 2; P0 baseline; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE` — frozen.
- **Purpose / current behavior:** ownership is distinct from physics participation; inactive Blocks are skipped later in the same frame and owners are erased after traversal with view rebuild. Normal Brick/BrickFragment integration passes.
- **Missing checklist:** none within frozen scope.
- **Exact target / allowed files:** `include/Physics/PhysicsEngine.hpp`, `src/Physics/PhysicsEngine.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`; only evidence-driven regression repair. **Read-only dependencies:** all P4 Block files. **Do not implement:** P4 lifecycle behavior or delete objects during traversal.
- **Dependencies / contracts:** provides `CON-P2-P4-BLOCK-LIFECYCLE`; consumes `Block::isExist()`. Group5 ordering may not replace this safer architecture.
- **Notes / SFML / checks:** keep C++20/SFML 3.1 compile and existing headbutt/same-frame/removal assertions. Visual/gameplay are outside this technical baseline.
- **Dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES`; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO` because no production gameplay flow exists.
- **DoD / commit:** existing lifecycle checks remain green; regression-only commit `p2: restore inactive object lifecycle contract`.

#### `BASE-P2-ROCKET-001` — Position-only target lifetime

- **Owner / priority / requirement / status:** Person 2; P1 baseline; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `DONE` for target lifetime only — frozen.
- **Purpose / current behavior:** Rocket resolves target position without retaining a dangling Player pointer and safely handles live/lost targets plus removal.
- **Missing checklist:** none in target-lifetime scope; production spawning/collision remains `P2-PROJECTILE-001`.
- **Exact target / allowed files:** `include/Objects/Environment/Rocket.hpp`, `src/Objects/Environment/Rocket.cpp`, `include/Levels/Managers/LevelManager.hpp`, `src/Levels/Managers/LevelManager.cpp`, `tests/Person2RuntimeContracts.cpp`; regression repair only. **Read-only dependencies:** P3 player/boss files. **Do not implement:** player ownership or boss attack policy.
- **Dependencies / contracts:** consumes safe position resolver; provides lifetime-safe projectile behavior. Do not copy Group5 raw target ownership.
- **Notes / SFML / checks:** preserve SFML 3.1 vector math; existing live/lost target runtime checks stay green. Visual/gameplay not proven.
- **Dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES` for lifetime; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=NO`; `GAMEPLAY_VERIFIED=NO`.
- **DoD / commit:** no dangling access under existing checks; regression-only commit `p2: restore rocket target lifetime safety`.

#### `BASE-P2-RUNTIME-001` — Existing 27-check runtime suite and movement base

- **Owner / priority / requirement / status:** Person 2; P0 baseline; `REQUIRED_BY_PLAN`; `DONE` for audited assertions — frozen.
- **Purpose / current behavior:** current `tests/Person2RuntimeContracts.cpp` executable printed and passed 27 checks; current production syntax passed SFML 3.1. Movement base velocity/force/friction/clamps is exercised indirectly.
- **Missing checklist:** none for the 27 audited checks; isolated tuning/nine-level evidence remains in other tasks.
- **Exact target / allowed files:** `tests/Person2RuntimeContracts.cpp`, `include/Components/MovementComponent.hpp`, `src/Components/MovementComponent.cpp`, and production files reached by existing assertions; append checks, never weaken/delete current assertions without new evidence. **Read-only dependencies:** none beyond their owners' public APIs. **Do not implement:** a new test framework or claim visual/gameplay completion from headless checks.
- **Dependencies / contracts:** protects every P2 task from regression; consumes current public interfaces. Group5 not needed.
- **Notes / SFML / checks:** keep C++20/SFML 3.1 syntax. Compile and run all 27 before and after each P2 implementation task; visual/gameplay remain separately gated.
- **Dimensions:** `SOURCE_DONE=YES`; `INTEGRATION_DONE=YES` for tested scope; `RUNTIME_TESTED=YES`; `VISUALLY_VERIFIED=N/A`; `GAMEPLAY_VERIFIED=NO` because headless checks are not gameplay verification.
- **DoD / commit:** all 27 existing checks pass unchanged; test-only additions use `test(p2): ...`.

## 9. Tests per task

All test work is planned for `tests/Person2RuntimeContracts.cpp` unless a tiny separate executable is necessary to keep the existing harness readable. No framework or new dependency is authorized. `SOURCE_DONE` is not enough to advance to `DONE`; dimensions must be updated independently.

| Task | Minimum compile/runtime check | Visual check | Gameplay check |
|---|---|---|---|
| `P2-EXTENT-001` | Valid/invalid readiness; dynamic widths; one 15-cell logical height | N/A | P1 camera consumes exact value |
| `P2-LAVA-001` | Fatal/nonfatal players; exactly-once event; same-frame exclusion | Lava animation/collider align | One correct death transition |
| `P2-WINFLAG-001` | Base-to-geometry math; one activation/callback | Pole extends upward from marker | Completion reaches deferred P1 flow |
| `P2-ENV-001` | Pipe dimensions; trampoline impulse; H/V platform bounds/carry | Primitive/art and hitbox align | Stand, launch, and ride |
| `P2-PROJECTILE-001` | Spawn/adopt/hit/expire/target-loss/off-world | Trajectory/hitbox align | Cannon and one Rocket source work |
| `P2-LOAD-001` | Missing/invalid/valid fixture and safe reload | P1 later displays valid/error state | Every selected wrapper loads truthfully |
| `P2-BOUNDS-001` | Both edges, overshoot, dynamic/narrow worlds | No jitter at viewport edge | Reach legal edge, never escape |
| `P2-PIT-001` | Threshold boundaries and duplicate suppression for the active 1P player | N/A | Agreed death/respawn policy |
| `P2-CLEANUP-001` | Each owner category off-world; view rebuild; legal airborne retention | N/A | Stable counts in extended session |
| `P2-CONTACT-ENEMY-001` | Goomba/Heriss/Koopa, invulnerability and removal ordering in 1P | Contact/bounce alignment | Score/death outcomes once |
| `P2-CONTACT-ITEM-001` | Coin/Mushroom/power-up and spawned-frame order in 1P | Pickup alignment | State/score/HUD update once |
| `P2-VARIANT-WIRE-001` | Dynamic type/config per variant; unsupported diagnostic; ownership | 64×64 correct variant art | Representative payload; Cloud excluded |
| `P2-NINE-LEVEL-001` | Nine wrapper identity/readiness/extent smokes | Each level rendered via production flow | Each separate level validation passes |

Global compile gate per task: affected production and test translation units compile with `-std=c++20` against SFML 3.1.0. Preserve the 27 baseline runtime checks. The full CMake/windres/package gate is P4 release work and must not be represented as a P2 source failure.

## 10. Integration checks

### Nine distinct level validation entries

These are validation IDs, not duplicate implementation tasks. All depend on `P2-NINE-LEVEL-001`; engine defects discovered in several levels produce one shared task/fix. Map pixels are not edited by P2, and a data-only issue is recorded as user-owned `KNOWN_MAP_EDIT_ITEM`.

| Validation ID | Exact wrapper | Source/load gate | Runtime gate | Visual gate | Gameplay gate | Current status |
|---|---|---|---|---|---|---|
| `LV-W1-LV1` | `W1_LV1` | exact identity and truthful readiness | extent/bounds/pit/contact smoke | production render | start → representative hazard/enemy/item/block → death and completion in separate executions | `BLOCKED` |
| `LV-W1-LV2` | `W1_LV2` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W1-LV3` | `W1_LV3` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W2-LV1` | `W2_LV1` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W2-LV2` | `W2_LV2` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W2-LV3` | `W2_LV3` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W3-LV1` | `W3_LV1` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W3-LV2` | `W3_LV2` | same, separately recorded | same, separately recorded | production render | same gate | `BLOCKED` |
| `LV-W3-LV3` | `W3_LV3` | same, separately recorded | same, separately recorded | production render | boss defeat emits exactly one final-level completion; no WinFlag | `BLOCKED` |

### Cross-owner vertical-slice order

1. P1 selects and owns one P3 player under resolved `DEC-PLAYER-OWNER`, and P1 owns the P2 active level (`CON-P1-P2-ACTIVE-LEVEL`).
2. P2 reports truthful load readiness/diagnostic and extent (`CON-P1-P2-LEVEL-LOAD`, `CON-P1-P2-WORLD-EXTENT`).
3. P1 calls P2 update once per frame and clamps its camera from P2 extent; P2 never owns the view.
4. P2 resolves Blocks/bounds, then player/enemy and player/item contacts, then fatal hazards/pit/completion, then flushes/removes owners. State replacement waits until traversal returns.
5. P3 supplies behavior outcomes; P1/P4 consume score/death/progression/HUD/audio outcomes. P2 does not mutate menus, lives, save data, or visuals.
6. First-slice acceptance uses one representative level and includes rendered player/world, movement/camera, one Block interaction, one enemy, one item, one death path, and one completion path. Then the same shared runtime is validated against all nine IDs.

### Merge-risk and regression gates

- `LevelManager.hpp/.cpp` is the primary merge hotspot. One named P2 integrator serializes contract changes and updates all callers in the same task.
- `PhysicsEngine.hpp/.cpp` changes for bounds and contacts must preserve block collision/headbutt/trampoline/moving-platform checks.
- `MapManager.hpp/.cpp` changes must preserve the adopted 45-row sketch contract, first-two-band runtime parsing, ignored legacy band, dynamic width, variants, diagnostics, and `CELL_SIZE=64`.
- P4 Block and P3 behavior API changes arrive through accepted cross-owner requests; P2 integrates only after provider tests and contract fields match.
- No task may regress `unique_ptr` owners to shared/raw ownership, retain callback raw pointers, delete during traversal, reopen normal Brick, or inspect/edit level images.

## 11. Final Person 2 Definition of Done

Person 2 is complete only when all of the following are true:

- All 13 remaining P2 task cards are `DONE`; each independently records `SOURCE_DONE=YES`, `INTEGRATION_DONE=YES`, `RUNTIME_TESTED=YES`, and the applicable visual/gameplay dimensions as `YES` (never inferred from compile success).
- All four frozen baseline cards remain `DONE`, and all original 27 runtime checks still pass unchanged.
- `LevelManager` remains the sole `unique_ptr` owner of map/runtime objects; typed views remain non-owning; P1 owns active-state lifetime, selected-player ownership matches the approved `DEC-PLAYER-OWNER`, and P2 only borrows players.
- Load readiness is explicit and truthful under the human-approved asset root; failed levels are inert and diagnosed.
- World extent is derived from dynamic width, one 15-row logical gameplay band, and `MapFormat::TILE_SIZE`; bounds, pit death, cleanup, and P1 camera consume that same value.
- Fatal events and completion are once-only, identify the affected borrowed player where applicable, and never trigger an in-traversal state replacement.
- Player/enemy and player/item contacts cover required representative behaviors, respect P3 ownership of behavior, and forward outcomes to P1/P4 without P2 owning score/lives/persistence.
- P4 Block lifecycle/spawn/variant/visual-size contracts are honored; normal Brick and BrickFragment baselines are not reopened; Cloud behavior follows resolved `DEC-CLOUD-SEMANTICS`.
- White WinFlag markers are treated as base/anchor tiles. `DEC-WINFLAG-POLISH` may select additional animation, but cannot reverse anchor correctness.
- `Pipe`, `Lava`, `Trampoline`, `MovingBlock`, `Cannon`, `Bullet`, `Rocket`, and `WinFlag` have compile, focused runtime, and applicable visual/gameplay evidence under C++20/SFML 3.1.
- `LV-W1-LV1` through `LV-W3-LV3` each have a separate completed validation record through one shared engine. No engine code is duplicated per level, no map content is audited/edited by P2, and any data-only finding remains a user-owned `KNOWN_MAP_EDIT_ITEM`.
- Group5 remains reference-only and its LLM/Ollama/chatbot feature remains excluded.

At that point P2's subsystem is ready for the whole-game gate: production menu/selection → valid stage load → rendered play → camera/bounds → representative Block/enemy/item interactions → death/restart or completion → all nine level validations. P2 completion alone does not claim whole-game completion until P1, P3, P4, persistence, HUD/audio, and release gates also pass.

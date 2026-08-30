# Group4 Super Mario — Master Task Board

This is the canonical roll-up for task identity, owner, priority, status, execution lane, dependencies, and completion dimensions. Detailed task cards live in the matching `P*_EXECUTION_PLAN.md`; changing detail must not change a task ID.

## Board rules

- Allowed statuses: `DONE`, `READY`, `IN_PROGRESS`, `BLOCKED`, `TESTING`, `DEFERRED`, `EXCLUDED`.
- Allowed execution lanes: `READY_INDEPENDENT`, `READY_WITH_STABLE_CONTRACT`, `BLOCKED_BY_OTHER_TASK`, `LATE_INTEGRATION`.
- `BLOCKED` means at least one named contract, decision, or predecessor prevents safe completion. The unblocked source portion may be split only by adding a stable child checklist, never by inventing a new top-level ID during implementation.
- A task moves `READY → IN_PROGRESS → TESTING → DONE`. Use `BLOCKED` whenever an external gate stops meaningful completion; use `DEFERRED` only after an explicit scope/schedule choice.
- Completion dimensions are recorded as `S/I/R/V/G` = source / integration / runtime / visual / gameplay. Initial remaining-task value is `0/0/0/0/0`. Baselines show verified scope only.
- A human decision is not complete merely because a recommended default exists.

## Counts at planning baseline

| Measure | Count |
|---|---:|
| Remaining implementation tasks | 45 |
| Verified-baseline records | 7 |
| Total task records | 52 |
| `READY` | 21 |
| `BLOCKED` | 24 |
| `DONE` baselines | 7 |
| P0 / P1 / P2 / P3 remaining | 25 / 18 / 2 / 0 |
| First-vertical-slice blockers | 25 P0 tasks |
| Separate level validations | 9 |

## EPIC-P1-RUNTIME-FLOW — Person 1 application, session, states, and transitions

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P1-APP-001` | Production application entry and `MyApp` loop | P0 | `REQUIRED_BY_PLAN` | `DONE` | `READY_INDEPENDENT` | None | State lifecycle integrated; active gameplay services are later consumers | SFML 3.1 window/clock/event loop; production state entry; safe shutdown; no Brick-demo raw owner | 1/1/1/1/1 |
| `P1-STATE-001` | State base and stack lifecycle | P0 | `REQUIRED_BY_PLAN` | `DONE` | `READY_INDEPENDENT` | None | Provides lifecycle used by all P1 states | Virtual input/update/render; deferred push/pop/replace; shared non-owning context; unique ownership; quit semantics | 1/1/1/NA/1 |
| `P1-MENU-001` | Main, name, and settings navigation | P1 | `REQUIRED_BY_PLAN` | `IN_PROGRESS` | `READY_INDEPENDENT` | P4 GUI/Audio/profile routes for completion | `CON-P1-P4-GUI`, `CON-P1-P4-AUDIO-EVENTS` | Visually tested Main/Name/Settings preview; Leaderboard/profile/audio handoffs remain | 0/0/1/1/0 |
| `P1-SELECT-001` | Character/world/level selection and session ownership | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P3 factory/owner target is ready; P4 GUI and in-memory progression provider remain | `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P4-PROGRESSION`, `CON-P1-P4-GUI` | Choose 1P character; create and transfer one stable owner; read current unlock values; select one of exactly nine stage IDs | 0/0/0/0/0 |
| `P1-GAME-001` | `GameState` active runtime orchestration | P0 | `REQUIRED_BY_PLAN` | `IN_PROGRESS` | `READY_WITH_STABLE_CONTRACT` | Injected P2/P3 runtime slice is green; production selection, P2 asset-root load route, and P4 transition consumers remain | `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P2-LEVEL-LOAD`, `CON-P1-P2-DEATH`, `CON-P1-P2-COMPLETION` | Own one player/level; reject failed load; update/render once; bridge callbacks after update; clear borrowed views before teardown; fire request adoption | 0/0/1/0/0 |
| `P1-CAMERA-001` | Dynamic clamped gameplay camera | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `TESTING` | `READY_WITH_STABLE_CONTRACT` | Source/runtime integration complete; visual gameplay and P4 HUD overlay acceptance remain | `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA` | Query dynamic extent; focus the owned 1P player; apply clamped world view; render P2 level; restore the prior UI view | 1/1/1/0/0 |
| `P1-EVENT-001` | `GameEventMediator` event bridge | P0 | `REQUIRED_BY_PLAN` | `TESTING` | `READY_WITH_STABLE_CONTRACT` | P2/P3 callback producer integration is green; P4 HUD/audio/persistence consumers remain | `CON-P1-P2-DEATH`, `CON-P1-P2-COMPLETION`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-HUD-DATA`, `CON-P1-P4-AUDIO-EVENTS` | Level callbacks queue typed score/coin/lives/death/completion events and flush only after P2 update returns | 1/0/1/NA/0 |
| `P1-DEATH-001` | Death, lives, restart, and Game Over flow | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P2 death/P3 reset providers and P1 event receipt are ready; P4 lives/GUI consumers remain | `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-GUI` | Receive the affected player once; update in-memory lives; return fatal deaths to spawn; show deferred DeathMenu/Game Over | 0/0/0/0/0 |
| `P1-WIN-001` | Completion, unlock, next-level, and final flow | P0 | `REQUIRED_BY_PLAN`; `REQUIRED_BY_TEAM` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P2 completion and P1 event receipt are ready; P4 GUI/in-memory progression provider remains | `CON-P1-P2-COMPLETION`, `CON-P1-P4-PROGRESSION`, `CON-P1-P4-GUI` | Receive validated stage completion; update next of nine in memory; final-game state; no out-of-range stage | 0/0/0/0/0 |
| `P1-PAUSE-001` | Pause/resume/restart/exit | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | `P1-STATE-001`, `P1-GAME-001`, approved restart behavior | State lifecycle and death/reset contracts | Freeze gameplay without corrupting clock; resume; safe restart; return to menu | 0/0/0/0/0 |

**P1 evidence update — 2026-08-30 (`P1-pre`):** `SuperMario` and `Person2RuntimeContracts` build/link with C++20/SFML 3.1; P2 CTest remains `1/1` green. `Person1StateFlowContracts` passes queued state ownership/destruction, mediator lifetime/dedup/error cleanup, and camera edge cases. `Person1ApplicationSmoke` renders visually inspected Main/Name/Settings PNGs; production launch/close also exits cleanly. No P1 owning raw pointer or manual allocation remains. The installed MinGW lacks ASan/UBSan runtimes, so sanitizer evidence is explicitly pending a release toolchain rather than claimed.

**P1 runtime evidence update — 2026-08-30 (`c2bbce5`):** `GameState` now uniquely owns one P3 player and one P2 level, injects only a borrowed player view, rejects unloaded/invalid worlds, binds all five P2 result callbacks into the queued mediator, calls level update/render once, restores the prior render view, and clears callbacks/views before level-then-player destruction. The focused P1 contract passes update/render counts, score/coin delivery, exactly-once pit death, fire input/request adoption and cooldown, failed-load suppression, camera restoration, and lifetime order. Full production build and CTest `5/5` pass; no P4 CMake ownership change is retained.

## EPIC-P2-WORLD-RUNTIME — Person 2 map, physics, environment, and runtime integration

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P2-LOAD-001` | Stage asset path and explicit load readiness | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | Resolved `DEC-ASSET-ROOT`; P1 executable context; P4 package/copy provider; P3 construction provider | `CON-P1-P2-LEVEL-LOAD`, `CON-P1-P2-ACTIVE-LEVEL`, `CON-P2-P3-ENTITY-CONSTRUCTION` | Nine exact filenames and explicit loaded state exist; replace CWD fallback with the shared executable-adjacent resolver and complete failure-path integration | 0/0/0/0/0 |
| `P2-EXTENT-001` | Explicit dynamic world extent | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 provider complete; P1 camera consumption is downstream | `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA` | Map-derived dynamic bounds are exposed only after load; source and runtime contract checks pass | 1/0/1/NA/0 |
| `P2-BOUNDS-001` | Player and world-edge bounds | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime scope complete | `CON-P1-P2-WORLD-EXTENT` | Dynamic left/right clamp, outward-velocity cancellation, exact-edge/overshoot/narrow-world handling and two loaded widths pass | 1/1/1/0/0 |
| `P2-PIT-001` | Map-derived pit/death detection | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 provider complete; P1/P3 fatal-respawn flow is downstream | `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET` | Map-bottom plus one-cell margin emits one fatal callback and skips later contacts; source/runtime pass | 1/0/1/NA/0 |
| `P2-CLEANUP-001` | Off-world owned-object cleanup | P1 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `TESTING` | `READY_WITH_STABLE_CONTRACT` | Long-session gameplay acceptance is downstream | `CON-P2-P3-ENEMY-REMOVAL` | Map-derived enemy/item/Fireball culling, inactive predicates, deferred owner removal, view rebuild, and legal-airborne retention pass | 1/1/1/NA/0 |
| `P2-CONTACT-ENEMY-001` | Player/enemy contact orchestration | P0 | `REQUIRED_BY_PLAN` | `TESTING` | `READY_WITH_STABLE_CONTRACT` | P1 callback-to-event binding plus visual/gameplay acceptance | `CON-P2-P3-PLAYER-ENEMY`, `CON-P1-P2-DEATH` | Relative-motion AABB classification drives typed stomp/side outcomes once; bounce, shell, invulnerability, score and death callbacks pass | 1/1/1/0/0 |
| `P2-CONTACT-ITEM-001` | Player/item collection orchestration | P0 | `REQUIRED_BY_PLAN` | `TESTING` | `READY_WITH_STABLE_CONTRACT` | P1/P4 callback-to-session/HUD binding plus visual/gameplay acceptance | `CON-P2-P3-PLAYER-ITEM`, `CON-P1-P4-SCORE-COINS-LIVES` | Item Block resolution, safe-growth denial, exactly-once form/delta outcome, deferred cleanup and callback surfaces pass | 1/1/1/0/0 |
| `P2-LAVA-001` | Correct Lava death semantics | P0 | `REQUIRED_BY_PLAN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 provider complete; P1 death transition is downstream | `CON-P1-P2-DEATH` | Small player dies once; Big/Fire downgrade without false death callback; source/runtime pass | 1/0/1/0/0 |
| `P2-ENV-001` | Pipe, Trampoline, and MovingBlock verification/integration | P1 | `REQUIRED_BY_PLAN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 source/runtime scope complete | `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-VISUAL-SIZE` | Pipe collider, Trampoline launch, MovingBlock continuous endpoint/square-loop motion, carry and parser registration pass focused runtime checks | 1/1/1/0/0 |
| `P2-PROJECTILE-001` | Cannon/Bullet/Rocket production collision and culling | P1 | `REQUIRED_BY_PLAN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P1 fire-input binding and visual/gameplay acceptance are downstream | `CON-P1-P2-DEATH`, `CON-P2-P3-PROJECTILE-SPAWN` | Typed P3 request validation, base-Enemy Boss consumption, deferred Bullet/Fireball ownership, damage, collision and cleanup pass | 1/1/1/0/0 |
| `P2-WINFLAG-001` | Base-anchor WinFlag semantics and completion | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `DONE` | `READY_WITH_STABLE_CONTRACT` | P2 provider complete; P1 deferred win transition is downstream | `CON-P1-P2-COMPLETION` | Base-anchor geometry, bounded slide and deferred exactly-once completion pass runtime checks | 1/0/1/0/0 |
| `P2-VARIANT-WIRE-001` | Block and payload variant construction wiring | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P4-PAYLOAD-BLOCK-001`, P3 payload types | `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF` | Preserve supported parser variants; call P4 constructors; adopt spawn; exclude deferred CloudPlatform wiring | 0/0/0/0/0 |
| `P2-NINE-LEVEL-001` | Shared engine readiness for all nine stage wrappers | P1 | `REQUIRED_BY_TEAM`; `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | Load, extent, interactions, variants, transitions; user resolves map items | All P1↔P2 and relevant runtime contracts | One shared code path; nine wrapper/load checks; no per-level engine fork; hand off to `LV-*` validations | 0/0/0/0/0 |

**P2 evidence update — 2026-08-29:** baseline implementation commit `419d03d` plus the current P2 bounds/physics/environment working tree; fresh `build` targets `SuperMario` and `Person2RuntimeContracts` link with C++20/SFML 3.1, CTest passes `1/1`, and the direct runtime executable reports `64 PASS / 0 FAIL`. The original 27 ownership/lifecycle checks remain green. The seven verified P2 source/runtime cards are `DONE`; downstream P1/P3/P4 visual/gameplay consumption remains recorded independently. The only observed compiler diagnostic is P4-owned `BrickFragment::reactToCollision(int)` not using its required override parameter; it is a non-blocking warning, not a P2 failure.

## EPIC-P3-GAMEPLAY-ENTITIES — Person 3 players, enemies, items, and factory

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P3-FOUNDATION-001` | Entity/living/character foundation checks | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | `CON-P3-P4-ANIMATION`, P2 movement consumption | Remove noisy construction side effect; define health/movement/animation invariants; focused checks | 0/0/0/0/0 |
| `P3-PLAYER-VISUAL-001` | Mario/Luigi rendering and animation setup | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | P4 AnimationComponent existing API | `CON-P3-P4-ANIMATION` | Load/register frames; state-key selection; render; collision/visual alignment for both players | 0/0/0/0/0 |
| `P3-PLAYER-STATE-001` | Safe growth, power, damage, and invulnerability | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | Existing player/physics APIs | Player outcome part of contact contracts | Honor `canGrow`; preserve feet when size changes; deterministic downgrade/death; focused checks | 0/0/0/0/0 |
| `P3-PLAYER-RESET-001` | Explicit reset/reconstruction API | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `READY` | `READY_WITH_STABLE_CONTRACT` | None; approved reset policy | `CON-P1-P3-PLAYER-RESET`, `CON-P1-P3-PLAYER-OWNERSHIP` | Reset health/power/motion/invulnerability/position per policy without changing runtime owner; distinguish fatal spawn reset from nonfatal power loss | 0/0/0/0/0 |
| `P3-GOOMBA-001` | Complete Goomba behavior, contact API, and visuals | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 block physics | `CON-P2-P3-PLAYER-ENEMY`, `CON-P2-P3-ENEMY-REMOVAL`, `CON-P3-P4-ANIMATION` | Visible walk/squish; stomp result; expiry/death; score outcome; no direct session mutation | 0/0/0/0/0 |
| `P3-KOOPA-001` | Complete Koopa/shell behavior, contact API, and visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 physics | Same enemy/animation contracts | Walk/shell/kick transitions; hazardous shell result; visible states; deterministic lifecycle | 0/0/0/0/0 |
| `P3-AIR-HERISS-001` | FlyingKoopa and Heriss behavior/contact/visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 participation policy | Same enemy/animation contracts | Wing loss without gravity regression; Heriss harmful-stomp result; visuals/lifecycle | 0/0/0/0/0 |
| `P3-BOSS-001` | Bowser and Petey required attacks/state/visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | P2 runtime spawn and collision surfaces at integration | `CON-P2-P3-PROJECTILE-SPAWN`, enemy removal, and animation contracts | Implement fire/slam/spike requests/outcomes; health/death; visible state; deterministic timing; no chatbot scope | 0/0/0/0/0 |
| `P3-ITEM-001` | Coin/Mushroom collection, world behavior, and visuals | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P2 item orchestration; P1/P4 outcome sink | `CON-P2-P3-PLAYER-ITEM`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P3-P4-ANIMATION` | Coin score/coin outcome; Mushroom emerge/move/reverse/grow; exactly-once inactive transition; visuals | 0/0/0/0/0 |
| `P3-FIRE-001` | FireBuff shooting/projectile behavior | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | Projectile spawn/ownership contract with P2; animation/input state | `CON-P2-P3-PROJECTILE-SPAWN`, item, and animation contracts | Collectible fire state; shoot action/cooldown; non-owning spawn request; collision outcome | 0/0/0/0/0 |
| `P3-FACTORY-001` | EntityFactory failure, player creation, and visual-init policy | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | P4 animation/resource policy | `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P2-P3-ENTITY-CONSTRUCTION` | Explicit invalid-type failure; unique_ptr returns; single-player/session handoff; initialized renderable outputs | 0/0/0/0/0 |

## EPIC-P4-PRESENTATION-DATA — Person 4 animation, blocks, UI/audio, persistence, release

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P4-ANIMATION-001` | AnimationComponent consumer contract and checks | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | `CON-P3-P4-ANIMATION` | Define missing-key/empty-frame behavior; non-copy resource lifetime; SFML 3.1 frame switching checks | 0/0/0/0/0 |
| `P4-BLOCK-SIZE-001` | 64×64 Block visual/physics invariant | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `READY` | `READY_INDEPENDENT` | Existing Block API | `CON-P2-P4-BLOCK-VISUAL-SIZE` | One size path scales 16×16 art to 64×64 while keeping hitbox/position aligned | 0/0/0/0/0 |
| `P4-QUESTION-BLOCK-001` | CoinBlock and MushroomBlock automated validation | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing spawn handoff and fixed offsets | `CON-P2-P4-SPAWN-HANDOFF`, `CON-P2-P4-BLOCK-COLLISION` | Migrate Coin test to SFML 3.1; assert counts/bounce/empty state/one-tile payload spawn | 0/0/0/0/0 |
| `P4-PAYLOAD-BLOCK-001` | Payload Brick and Mushroom/1-Up/Star block variants | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-BLOCK-ACTOR-ELIGIBILITY`, P3 payload types, P2 variant wiring | `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF` | Add variant/payload API; actor context; one-shot spawn; preserve normal Brick baseline | 0/0/0/0/0 |
| `P4-CLOUD-001` | CloudBlock implementation under approved semantics | P1 | `DEFERRED` | `DEFERRED` | `BLOCKED_BY_OTHER_TASK` | Reopen `DEC-CLOUD-SEMANTICS` before implementation | `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-BLOCK-COLLISION` | CloudPlatform/CloudBlock is excluded from the current release; do not alter map markers or rewrite MovingBlock | 0/0/0/0/0 |
| `P4-AUDIO-001` | AudioSystem music/SFX service | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None for core; events later | `CON-P1-P4-AUDIO-EVENTS` | Resource cache; volume/music/SFX controls; missing-file safety; no ownership of gameplay objects | 0/0/0/0/0 |
| `P4-GUI-001` | Reusable menu GUI controls | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None for core | `CON-P1-P4-GUI` | Minimum production selection/death/win controls first; then remaining keyboard focus, layout, resize, and readability; P1 keeps state navigation | 0/0/0/0/0 |
| `P4-HUD-001` | Gameplay HUD / LevelGUI | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P1 event/session data; decisions affecting lives | `CON-P1-P4-HUD-DATA`, `CON-P1-P4-SCORE-COINS-LIVES` | Render score/coins/lives/time/current stage; screen-space view; update from snapshots, not world ownership | 0/0/0/0/0 |
| `P4-PERSISTENCE-001` | UserData save/load and progression | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | Durable portion: `DEC-PERSISTENCE`, `DEC-MULTIPLAYER-DEATH`; P1 semantic event surface. The in-memory nine-stage provider checklist is explicitly unblocked. | `CON-P1-P4-PERSISTENCE`, `CON-P1-P4-PROGRESSION`, `CON-P1-P4-SCORE-COINS-LIVES` | First land/test the in-memory nine-stage mapping; full card adds versioned schema, approved save timing, atomic/recoverable writes, and safe missing/corrupt data | 0/0/0/0/0 |
| `P4-LEADERBOARD-001` | Leaderboard storage and state implementation | P2 | `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | `P4-PERSISTENCE-001`, P1 state/UI flow | Persistence and GUI contracts | Stable CSV/schema; ordering/ties; name validation; safe malformed rows; P4 owns LeaderboardState implementation | 0/0/0/0/0 |
| `P4-PACKAGE-001` | C++20/SFML 3.1 build and release package | P2 | `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | `DEC-ASSET-ROOT`; all production tasks; build-path issue | Load/persistence asset-root contracts | Pin/use SFML 3.1; register tests; clean build/link; copy approved assets; launch/package smoke check | 0/0/0/0/0 |

## VERIFIED COMPLETED BASELINE

| Baseline task | Owner | Pri | Status | Frozen scope | Evidence/gate | S/I/R/V/G |
|---|---:|---:|---|---|---|---|
| `BASE-P2-OWNERSHIP-001` | P2 | P0 | `DONE` | `unique_ptr` owner list, typed non-owning views, separate physics participation | Existing runtime ownership assertions | 1/1/1/NA/0 |
| `BASE-P2-LIFECYCLE-001` | P2 | P0 | `DONE` | pending adoption, inactive cleanup, same-frame inactive Block exclusion | Existing runtime lifecycle assertions | 1/1/1/NA/0 |
| `BASE-P2-ROCKET-001` | P2 | P1 | `DONE` | position-only resolver and lost-target safety | Live/lost-target runtime assertions | 1/1/1/0/0 |
| `BASE-P2-RUNTIME-001` | P2 | P0 | `DONE` | 27-check Person 2 runtime suite | 27/27 passed at audit | 1/1/1/NA/0 |
| `BASE-P4-BRICK-001` | P4 | P0 | `DONE` | normal Brick one-shot bottom-hit and four-fragment behavior | Real headbutt/runtime assertions | 1/1/1/0/0 |
| `BASE-P4-FRAGMENT-001` | P4 | P1 | `DONE` | BrickFragment positioning/update/expiry and P2 adoption | Runtime owner/view removal assertions | 1/1/1/0/0 |
| `BASE-P4-BLOCK-INTEGRATION-001` | P4 | P1 | `DONE` | shared constants, public lifecycle, popped Coin, Mushroom spawn offset | Source/integration evidence; combined runtime payload scope remains open | 1/1/0/0/0 |

The zero gameplay fields are deliberate: the audit verified technical runtime contracts, not the absent production gameplay flow. `NA` is limited to technical baselines with no standalone visual output. Neither notation reopens frozen source/runtime scope; extending tasks own later visual/gameplay proof.

## LEVEL VALIDATION — separate nine-stage acceptance entries

These entries are validation records, not extra engine implementation tasks. Map content remains user-owned.

| Validation ID | Stage | Status | Owner / participants | Required prerequisites | Required evidence |
|---|---|---|---|---|---|
| `LV-W1-LV1` | W1_LV1 | `BLOCKED` | P2 / P1,P3,P4 | P2-NINE-LEVEL-001 and user map readiness | Successful load; spawn; extent/camera/bounds/pit; representative block/enemy/item/hazard; flag; death/win transition |
| `LV-W1-LV2` | W1_LV2 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same stage-specific runtime/visual/gameplay record |
| `LV-W1-LV3` | W1_LV3 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same plus world-end transition |
| `LV-W2-LV1` | W2_LV1 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same stage-specific record |
| `LV-W2-LV2` | W2_LV2 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same stage-specific record |
| `LV-W2-LV3` | W2_LV3 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same plus world-end transition |
| `LV-W3-LV1` | W3_LV1 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same stage-specific record |
| `LV-W3-LV2` | W3_LV2 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same stage-specific record |
| `LV-W3-LV3` | W3_LV3 | `BLOCKED` | P2 / P1,P3,P4 | Same plus P3 boss-defeated outcome and P1 final transition | Boss defeat emits exactly one completion; no WinFlag is required; then final-game flow and persistence |

For every `LV-*` entry, record the exact build, commit, player count, decision set, observed load result, and failures. A map-data failure is tagged `KNOWN_MAP_EDIT_ITEM` and returned to the user; an engine failure updates the shared owning task instead of creating a level-specific engine fork.

## Required priority and workflow views

These are synchronized indexes over the canonical task rows above, not duplicate task cards. Update a row first, then update every affected view in the same planning change.

### P0 VERTICAL SLICE

| Owner | Task IDs | Count |
|---|---|---:|
| P1 | `P1-APP-001`, `P1-STATE-001`, `P1-SELECT-001`, `P1-GAME-001`, `P1-CAMERA-001`, `P1-EVENT-001`, `P1-DEATH-001`, `P1-WIN-001` | 8 |
| P2 | `P2-LOAD-001`, `P2-EXTENT-001`, `P2-BOUNDS-001`, `P2-PIT-001`, `P2-CONTACT-ENEMY-001`, `P2-CONTACT-ITEM-001`, `P2-LAVA-001`, `P2-WINFLAG-001` | 8 |
| P3 | `P3-PLAYER-VISUAL-001`, `P3-PLAYER-STATE-001`, `P3-PLAYER-RESET-001`, `P3-GOOMBA-001`, `P3-ITEM-001`, `P3-FACTORY-001` | 6 |
| P4 | `P4-ANIMATION-001`, `P4-BLOCK-SIZE-001`, `P4-GUI-001` | 3 |

### P1 REQUIRED GAMEPLAY

| Owner | Task IDs | Count |
|---|---|---:|
| P1 | `P1-MENU-001`, `P1-PAUSE-001` | 2 |
| P2 | `P2-CLEANUP-001`, `P2-ENV-001`, `P2-PROJECTILE-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001` | 5 |
| P3 | `P3-FOUNDATION-001`, `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001`, `P3-FIRE-001` | 5 |
| P4 | `P4-QUESTION-BLOCK-001`, `P4-PAYLOAD-BLOCK-001`, `P4-AUDIO-001`, `P4-HUD-001`, `P4-PERSISTENCE-001` | 5 |

### READY

14 tasks: `P3-FOUNDATION-001`, `P3-PLAYER-VISUAL-001`, `P3-PLAYER-STATE-001`, `P3-PLAYER-RESET-001`, `P3-GOOMBA-001`, `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001`, `P3-FACTORY-001`; `P4-ANIMATION-001`, `P4-BLOCK-SIZE-001`, `P4-QUESTION-BLOCK-001`, `P4-AUDIO-001`, `P4-GUI-001`.

### IN PROGRESS

2 tasks: `P1-MENU-001`, `P1-GAME-001`.

### TESTING

5 tasks: `P1-CAMERA-001`, `P1-EVENT-001`; `P2-CLEANUP-001`, `P2-CONTACT-ENEMY-001`, `P2-CONTACT-ITEM-001`.

### BLOCKED

14 tasks: `P1-SELECT-001`, `P1-DEATH-001`, `P1-WIN-001`, `P1-PAUSE-001`; `P2-LOAD-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001`; `P3-ITEM-001`, `P3-FIRE-001`; `P4-PAYLOAD-BLOCK-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`, `P4-LEADERBOARD-001`, `P4-PACKAGE-001`.

### DONE

9 tasks: `P1-APP-001`, `P1-STATE-001`; `P2-EXTENT-001`, `P2-BOUNDS-001`, `P2-PIT-001`, `P2-LAVA-001`, `P2-ENV-001`, `P2-PROJECTILE-001`, `P2-WINFLAG-001`.

### LATE INTEGRATION

`P1-PAUSE-001`, `P2-NINE-LEVEL-001`, `P4-LEADERBOARD-001`, and `P4-PACKAGE-001`. Their workflow status remains `BLOCKED` until their named prerequisites clear.

### DEFERRED

1 task: `P4-CLOUD-001`; CloudPlatform/CloudBlock is outside the current release until `DEC-CLOUD-SEMANTICS` is reopened.

### EXCLUDED

No implementation task is created for Group5 LLM/Ollama/chatbot behavior; it is `EXCLUDED_FROM_GROUP4`. Map design and known map-data edits remain user-owned `KNOWN_MAP_EDIT_ITEM` work, not engine implementation tasks. Group5 remains reference-only.

## Merge-conflict watch list

Coordinate before any parallel edit to:

- `src/Levels/Managers/LevelManager.cpp`
- `include/Levels/Managers/LevelManager.hpp`
- `src/Physics/PhysicsEngine.cpp`
- `src/Levels/Managers/MapManager.cpp`
- `src/Entities/Players/PlayerManager.cpp`
- `include/Entities/Players/PlayerManager.hpp`
- `include/Objects/Blocks/Block.hpp` and `src/Objects/Blocks/Block.cpp`
- `src/States/Base/GameState.cpp` and its header
- `src/main.cpp`
- `CMakeLists.txt`

Use cross-owner requests, small provider-first commits, and contract tests as specified in `04_USAGE_AND_PROGRESS_WORKFLOW.md`.

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
| `P1-APP-001` | Production application entry and `MyApp` loop | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | Later consumes active State contract | Implement missing `.cpp`; window/clock/event loop; replace Brick test entry; safe shutdown | 0/0/0/0/0 |
| `P1-STATE-001` | State base and stack lifecycle | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | Provides lifecycle used by all P1 states | Virtual input/update/render; deferred push/pop/replace; shared data lifetime; quit semantics | 0/0/0/0/0 |
| `P1-MENU-001` | Main, name, and settings navigation | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | `P1-STATE-001` interface can be drafted concurrently | `CON-P1-P4-GUI`, `CON-P1-P4-AUDIO-EVENTS` | Build navigation model; validate name/settings; consume P4 widgets/audio when available | 0/0/0/0/0 |
| `P1-SELECT-001` | Character/world/level selection and session ownership | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-PLAYER-OWNER`; `P3-FACTORY-001`; `P4-GUI-001`; P4 in-memory progression provider checklist | `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P4-PROGRESSION`, `CON-P1-P4-GUI` | Choose 1P/2P; create stable owners; read current in-memory unlock values; select one of exactly nine stage IDs | 0/0/0/0/0 |
| `P1-GAME-001` | `GameState` active runtime orchestration | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P1-STATE-001`, `P1-SELECT-001`, `P2-LOAD-001`; active-level/load contracts | `CON-P1-P2-ACTIVE-LEVEL`, `CON-P1-P2-LEVEL-LOAD`, `CON-P1-P2-DEATH`, `CON-P1-P2-COMPLETION` | Own level; inject borrowed players; update/render once; surface load error; defer state replacement | 0/0/0/0/0 |
| `P1-CAMERA-001` | Dynamic clamped gameplay camera | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-EXTENT-001`; camera contract | `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA` | Follow selected target; clamp to world; handle world smaller than viewport; restore UI view | 0/0/0/0/0 |
| `P1-EVENT-001` | `GameEventMediator` event bridge | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Contract cards define payloads; implementations may arrive later | `CON-P1-P2-DEATH`, `CON-P1-P2-COMPLETION`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-HUD-DATA`, `CON-P1-P4-AUDIO-EVENTS` | Typed events; no runtime ownership; exactly-once dispatch; unsubscribe/lifetime rules | 0/0/0/0/0 |
| `P1-DEATH-001` | Death, lives, restart, and Game Over flow | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-MULTIPLAYER-DEATH`, `DEC-RESPAWN`, `P2-PIT-001`, `P3-PLAYER-RESET-001`, `P4-GUI-001` | `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-GUI` | Consume affected-player event once; update in-memory lives; reset/reload per decision; emit downstream session event; show deferred DeathMenu/Game Over | 0/0/0/0/0 |
| `P1-WIN-001` | Completion, unlock, next-level, and final flow | P0 | `REQUIRED_BY_PLAN`; `REQUIRED_BY_TEAM` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-WINFLAG-001`; `P4-GUI-001`; P4 in-memory progression provider checklist | `CON-P1-P2-COMPLETION`, `CON-P1-P4-PROGRESSION`, `CON-P1-P4-GUI` | Exactly-once completion; update next of nine in memory; emit downstream completion event; final-game state; no out-of-range stage | 0/0/0/0/0 |
| `P1-PAUSE-001` | Pause/resume/restart/exit | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | `P1-STATE-001`, `P1-GAME-001`, approved restart behavior | State lifecycle and death/reset contracts | Freeze gameplay without corrupting clock; resume; safe restart; return to menu | 0/0/0/0/0 |

## EPIC-P2-WORLD-RUNTIME — Person 2 map, physics, environment, and runtime integration

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P2-LOAD-001` | Stage asset path and explicit load readiness | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-ASSET-ROOT`; P3 factory construction provider | `CON-P1-P2-LEVEL-LOAD`, `CON-P1-P2-ACTIVE-LEVEL`, `CON-P2-P3-ENTITY-CONSTRUCTION` | One asset resolver; correct nine names; store/expose success/error; fail safely on path/parser/construction error | 0/0/0/0/0 |
| `P2-EXTENT-001` | Explicit dynamic world extent | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `READY` | `READY_INDEPENDENT` | None | `CON-P1-P2-WORLD-EXTENT`, `CON-P1-P2-CAMERA` | Derive pixel rect from parsed width and 15-row logical height; expose only after load | 0/0/0/0/0 |
| `P2-BOUNDS-001` | Player and world-edge bounds | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-EXTENT-001` | `CON-P1-P2-WORLD-EXTENT` | Clamp/resolve left-right edges; preserve dynamic width; no window-derived limit | 0/0/0/0/0 |
| `P2-PIT-001` | Map-derived pit/death detection | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-EXTENT-001`, `DEC-MULTIPLAYER-DEATH`, `DEC-RESPAWN` | `CON-P1-P2-DEATH`, `CON-P1-P3-PLAYER-RESET` | Detect below logical world plus agreed margin; one callback/player; stop later contacts that frame | 0/0/0/0/0 |
| `P2-CLEANUP-001` | Off-world owned-object cleanup | P1 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P2-EXTENT-001` | `CON-P2-P3-ENEMY-REMOVAL` | Deactivate/remove enemies/items/projectiles beyond threshold through existing owner cleanup | 0/0/0/0/0 |
| `P2-CONTACT-ENEMY-001` | Player/enemy contact orchestration | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P3 enemy contact APIs; P1 event sink | `CON-P2-P3-PLAYER-ENEMY`, `CON-P1-P2-DEATH` | Side/stomp classification; bounce/damage/shell results; once/contact ordering; score/death output | 0/0/0/0/0 |
| `P2-CONTACT-ITEM-001` | Player/item collection orchestration | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P3-ITEM-001`; score/lives event surface | `CON-P2-P3-PLAYER-ITEM`, `CON-P1-P4-SCORE-COINS-LIVES` | Item world physics; overlap; exactly-once collection; inactive cleanup; outcome dispatch | 0/0/0/0/0 |
| `P2-LAVA-001` | Correct Lava death semantics | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing death callback | `CON-P1-P2-DEATH` | True lethal result; callback only on actual death; one-shot per affected player/frame | 0/0/0/0/0 |
| `P2-ENV-001` | Pipe, Trampoline, and MovingBlock verification/integration | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing physics/block contracts | `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-VISUAL-SIZE` | Focused tests; collider/launch/carry correctness; parser integration; no Cloud decision leakage | 0/0/0/0/0 |
| `P2-PROJECTILE-001` | Cannon/Bullet/Rocket production collision and culling | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing unique_ptr spawn/lifetime baseline | `CON-P1-P2-DEATH`, `CON-P2-P3-PROJECTILE-SPAWN` | Preserve spawn adoption; add internal projectile collision/culling; accept P3 attack requests; provide Rocket creation path; no target-pointer regression | 0/0/0/0/0 |
| `P2-WINFLAG-001` | Base-anchor WinFlag semantics and completion | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `READY` | `READY_INDEPENDENT` | Core anchor direction is known; polish decision is not required for core fix | `CON-P1-P2-COMPLETION` | Interpret white marker as base/activation tile; extend visuals upward; one callback; optional delay isolated | 0/0/0/0/0 |
| `P2-VARIANT-WIRE-001` | Block/payload/Cloud variant construction wiring | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `P4-PAYLOAD-BLOCK-001`, `DEC-CLOUD-SEMANTICS`, P3 payload types | `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF` | Preserve parser variant; call P4 constructors; adopt spawn; distinguish P2 MovingBlock from approved Cloud meaning | 0/0/0/0/0 |
| `P2-NINE-LEVEL-001` | Shared engine readiness for all nine stage wrappers | P1 | `REQUIRED_BY_TEAM`; `REQUIRED_BY_PLAN` | `BLOCKED` | `LATE_INTEGRATION` | Load, extent, interactions, variants, transitions; user resolves map items | All P1↔P2 and relevant runtime contracts | One shared code path; nine wrapper/load checks; no per-level engine fork; hand off to `LV-*` validations | 0/0/0/0/0 |

## EPIC-P3-GAMEPLAY-ENTITIES — Person 3 players, enemies, items, and factory

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P3-FOUNDATION-001` | Entity/living/character foundation checks | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | `CON-P3-P4-ANIMATION`, P2 movement consumption | Remove noisy construction side effect; define health/movement/animation invariants; focused checks | 0/0/0/0/0 |
| `P3-PLAYER-VISUAL-001` | Mario/Luigi rendering and animation setup | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | P4 AnimationComponent existing API | `CON-P3-P4-ANIMATION` | Load/register frames; state-key selection; render; collision/visual alignment for both players | 0/0/0/0/0 |
| `P3-PLAYER-STATE-001` | Safe growth, power, damage, and invulnerability | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | Existing player/physics APIs | Player outcome part of contact contracts | Honor `canGrow`; preserve feet when size changes; deterministic downgrade/death; focused checks | 0/0/0/0/0 |
| `P3-PLAYER-RESET-001` | Explicit reset/reconstruction API | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-RESPAWN`, `DEC-MULTIPLAYER-DEATH`, `DEC-PLAYER-OWNER` | `CON-P1-P3-PLAYER-RESET`, `CON-P1-P3-PLAYER-OWNERSHIP` | Reset health/power/motion/invulnerability/position per policy without changing runtime owner | 0/0/0/0/0 |
| `P3-GOOMBA-001` | Complete Goomba behavior, contact API, and visuals | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 block physics | `CON-P2-P3-PLAYER-ENEMY`, `CON-P2-P3-ENEMY-REMOVAL`, `CON-P3-P4-ANIMATION` | Visible walk/squish; stomp result; expiry/death; score outcome; no direct session mutation | 0/0/0/0/0 |
| `P3-KOOPA-001` | Complete Koopa/shell behavior, contact API, and visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 physics | Same enemy/animation contracts | Walk/shell/kick transitions; hazardous shell result; visible states; deterministic lifecycle | 0/0/0/0/0 |
| `P3-AIR-HERISS-001` | FlyingKoopa and Heriss behavior/contact/visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing P2 participation policy | Same enemy/animation contracts | Wing loss without gravity regression; Heriss harmful-stomp result; visuals/lifecycle | 0/0/0/0/0 |
| `P3-BOSS-001` | Bowser and Petey required attacks/state/visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | P2 runtime spawn and collision surfaces at integration | `CON-P2-P3-PROJECTILE-SPAWN`, enemy removal, and animation contracts | Implement fire/slam/spike requests/outcomes; health/death; visible state; deterministic timing; no chatbot scope | 0/0/0/0/0 |
| `P3-ITEM-001` | Coin/Mushroom collection, world behavior, and visuals | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | P2 item orchestration; P1/P4 outcome sink | `CON-P2-P3-PLAYER-ITEM`, `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P3-P4-ANIMATION` | Coin score/coin outcome; Mushroom emerge/move/reverse/grow; exactly-once inactive transition; visuals | 0/0/0/0/0 |
| `P3-FIRE-001` | FireBuff shooting/projectile behavior | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | Projectile spawn/ownership contract with P2; animation/input state | `CON-P2-P3-PROJECTILE-SPAWN`, item, and animation contracts | Collectible fire state; shoot action/cooldown; non-owning spawn request; collision outcome | 0/0/0/0/0 |
| `P3-FACTORY-001` | EntityFactory failure, player creation, and visual-init policy | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-PLAYER-OWNER`; P4 animation/resource policy | `CON-P1-P3-PLAYER-OWNERSHIP`, `CON-P2-P3-ENTITY-CONSTRUCTION` | Explicit invalid-type failure; unique_ptr returns; separate player/session handoff; initialized renderable outputs | 0/0/0/0/0 |

## EPIC-P4-PRESENTATION-DATA — Person 4 animation, blocks, UI/audio, persistence, release

| Task | Title | Pri | Requirement | Status | Lane | Blocking dependencies | Primary contracts | Subtask/checklist summary | S/I/R/V/G |
|---|---|---:|---|---|---|---|---|---|---|
| `P4-ANIMATION-001` | AnimationComponent consumer contract and checks | P0 | `REQUIRED_BY_PLAN` | `READY` | `READY_INDEPENDENT` | None | `CON-P3-P4-ANIMATION` | Define missing-key/empty-frame behavior; non-copy resource lifetime; SFML 3.1 frame switching checks | 0/0/0/0/0 |
| `P4-BLOCK-SIZE-001` | 32×32 Block visual/physics invariant | P0 | `REQUIRED_BY_PLAN`; `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `READY` | `READY_INDEPENDENT` | Existing Block API | `CON-P2-P4-BLOCK-VISUAL-SIZE` | One size path scales 16×16 art to 32×32 while keeping hitbox/position aligned | 0/0/0/0/0 |
| `P4-QUESTION-BLOCK-001` | CoinBlock and MushroomBlock automated validation | P1 | `REQUIRED_BY_PLAN` | `READY` | `READY_WITH_STABLE_CONTRACT` | Existing spawn handoff and fixed offsets | `CON-P2-P4-SPAWN-HANDOFF`, `CON-P2-P4-BLOCK-COLLISION` | Migrate Coin test to SFML 3.1; assert counts/bounce/empty state/one-tile payload spawn | 0/0/0/0/0 |
| `P4-PAYLOAD-BLOCK-001` | Payload Brick and Mushroom/1-Up/Star block variants | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-BLOCK-ACTOR-ELIGIBILITY`, P3 payload types, P2 variant wiring | `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF` | Add variant/payload API; actor context; one-shot spawn; preserve normal Brick baseline | 0/0/0/0/0 |
| `P4-CLOUD-001` | CloudBlock implementation under approved semantics | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | `BLOCKED_BY_OTHER_TASK` | `DEC-CLOUD-SEMANTICS`; P2 mapping | `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-BLOCK-COLLISION` | Implement only approved static/moving distinction; collision/render/lifecycle; no P2 MovingBlock rewrite | 0/0/0/0/0 |
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
| `LV-W3-LV3` | W3_LV3 | `BLOCKED` | P2 / P1,P3,P4 | Same | Same plus final-game completion and persistence |

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
| P4 | `P4-QUESTION-BLOCK-001`, `P4-PAYLOAD-BLOCK-001`, `P4-CLOUD-001`, `P4-AUDIO-001`, `P4-HUD-001`, `P4-PERSISTENCE-001` | 6 |

### READY

21 tasks: `P1-APP-001`, `P1-STATE-001`, `P1-MENU-001`, `P1-EVENT-001`; `P2-EXTENT-001`, `P2-LAVA-001`, `P2-ENV-001`, `P2-PROJECTILE-001`, `P2-WINFLAG-001`; `P3-FOUNDATION-001`, `P3-PLAYER-VISUAL-001`, `P3-PLAYER-STATE-001`, `P3-GOOMBA-001`, `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001`; `P4-ANIMATION-001`, `P4-BLOCK-SIZE-001`, `P4-QUESTION-BLOCK-001`, `P4-AUDIO-001`, `P4-GUI-001`.

### IN PROGRESS

None at the planning baseline.

### BLOCKED

24 tasks: `P1-SELECT-001`, `P1-GAME-001`, `P1-CAMERA-001`, `P1-DEATH-001`, `P1-WIN-001`, `P1-PAUSE-001`; `P2-LOAD-001`, `P2-BOUNDS-001`, `P2-PIT-001`, `P2-CLEANUP-001`, `P2-CONTACT-ENEMY-001`, `P2-CONTACT-ITEM-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001`; `P3-PLAYER-RESET-001`, `P3-ITEM-001`, `P3-FIRE-001`, `P3-FACTORY-001`; `P4-PAYLOAD-BLOCK-001`, `P4-CLOUD-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`, `P4-LEADERBOARD-001`, `P4-PACKAGE-001`.

### TESTING

None at the planning baseline.

### LATE INTEGRATION

`P1-PAUSE-001`, `P2-NINE-LEVEL-001`, `P4-LEADERBOARD-001`, and `P4-PACKAGE-001`. Their workflow status remains `BLOCKED` until their named prerequisites clear.

### DEFERRED

No required implementation task is deferred at the planning baseline.

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

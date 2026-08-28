# Person 3 Execution Plan — Entities, Players, Enemies, Factory, and Items

Planning baseline: `TEAM_CURRENT_STATE_AUDIT.md` at Group4 `f242c4f0e64a484a44727568d9d7f6d698798232`  
Plan verdict: `PLAN_SUFFICIENT_BUT_INTEGRATION_CONTRACTS_ARE_MISSING`  
Target: C++20 and SFML 3.1.0  
Scope rule: this file plans Person 3 work only. It does not authorize Person 3 to edit another owner's production files.

## 1. Component checklist and status summary

### 1.1 Task status

The five completion dimensions are independent. A task remains open until all required dimensions are `YES`; a source implementation alone is not completion.

| Task ID | Component | Priority | Requirement class | Workflow status | SOURCE_DONE | INTEGRATION_DONE | RUNTIME_TESTED | VISUALLY_VERIFIED | GAMEPLAY_VERIFIED |
|---|---|---:|---|---|---|---|---|---|---|
| `P3-FOUNDATION-001` | GameObject/LivingEntity/Character/Enemy foundation cleanup and contract checks | P1 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-PLAYER-VISUAL-001` | Mario/Luigi texture, animation, and render setup | P0 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-PLAYER-STATE-001` | Safe growth, power, damage, and player-state behavior | P0 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-PLAYER-RESET-001` | Explicit reset/reconstruction API | P0 | `IMPLIED_BY_CURRENT_GROUP4_DESIGN` | `BLOCKED` | NO | NO | NO | NO | NO |
| `P3-GOOMBA-001` | Goomba behavior, contact API, lifecycle, and visuals | P0 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-KOOPA-001` | Koopa/shell behavior, contact API, lifecycle, and visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-AIR-HERISS-001` | FlyingKoopa and Heriss behavior/contact APIs/visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-BOSS-001` | Bowser and PeteyPiranha attacks, state, lifecycle, and visuals | P1 | `REQUIRED_BY_PLAN` | `READY` | NO | NO | NO | NO | NO |
| `P3-ITEM-001` | Coin/Mushroom/item collection effects, world behavior, and visuals | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | NO | NO | NO | NO | NO |
| `P3-FIRE-001` | FireBuff shooting and projectile request contract | P1 | `REQUIRED_BY_PLAN` | `BLOCKED` | NO | NO | NO | NO | NO |
| `P3-FACTORY-001` | EntityFactory failure, player construction, and render-init policy | P0 | `REQUIRED_BY_PLAN` | `BLOCKED` | NO | NO | NO | NO | NO |

Status counts: 11 remaining tasks; 7 `READY`; 4 `BLOCKED`; 6 P0; 5 P1; 0 P2/P3. There is no Person 3 verified-baseline card: current P3 behavior is useful partial source, but no P3-owned component has all five completion dimensions verified.

`STATUS_CHANGED_SINCE_AUDIT: NO` — current Group4 `HEAD` matches the audit and no P3-owned production path is locally modified.

### 1.2 Owned production-file accounting

Every existing P3-owned production header/source pair is assigned below. `TASK` means the file may be edited only by the named P3 task(s), after dependency and contract gates are satisfied.

| P3-owned production files | Accounting | Task ID(s) | Current disposition |
|---|---|---|---|
| `include/Core/GameObject.hpp`; `src/Core/GameObject.cpp` | `TASK` | `P3-FOUNDATION-001` | Position/size/hitbox synchronization works; constructor logging and foundation checks remain. |
| `include/Entities/Base/LivingEntity.hpp`; `src/Entities/Base/LivingEntity.cpp` | `TASK` | `P3-FOUNDATION-001` | Health/death and owned component slots exist; animation is not initialized. |
| `include/Entities/Base/Character.hpp`; `src/Entities/Base/Character.cpp` | `TASK` | `P3-FOUNDATION-001` | Grounded jump contract exists. |
| `include/Entities/Base/Enemy.hpp`; `src/Entities/Base/Enemy.cpp` | `TASK` | `P3-FOUNDATION-001` | Damage/points/stomp flags exist; integrated contact result does not. |
| `include/Entities/Players/PlayerManager.hpp`; `src/Entities/Players/PlayerManager.cpp` | `TASK` | `P3-PLAYER-STATE-001`, `P3-PLAYER-RESET-001`, `P3-ITEM-001`, `P3-FIRE-001` | Input, buffs, damage downgrade, and invulnerability are partial. Coordinate edits through the earliest active task. |
| `include/Entities/Players/Mario.hpp`; `src/Entities/Players/Mario.cpp` | `TASK` | `P3-PLAYER-VISUAL-001` | Tuning and animation-key choice exist; texture/frames/component do not. |
| `include/Entities/Players/Luigi.hpp`; `src/Entities/Players/Luigi.cpp` | `TASK` | `P3-PLAYER-VISUAL-001` | Tuning and animation-key choice exist; texture/frames/component do not. |
| `include/Entities/EntityFactory.hpp`; `src/Entities/EntityFactory.cpp` | `TASK` | `P3-FACTORY-001` | Unique ownership exists; unknown names silently create defaults and player return type is too broad for the intended owner bridge. |
| `include/Entities/Enemies/Goomba.hpp`; `src/Entities/Enemies/Goomba.cpp` | `TASK` | `P3-GOOMBA-001` | Walk and timed squish exist. |
| `include/Entities/Enemies/Koopa.hpp`; `src/Entities/Enemies/Koopa.cpp` | `TASK` | `P3-KOOPA-001` | Walk, shell, and kick states exist. |
| `include/Entities/Enemies/FlyingKoopa.hpp`; `src/Entities/Enemies/FlyingKoopa.cpp` | `TASK` | `P3-AIR-HERISS-001` | Sine flight and wing loss exist. |
| `include/Entities/Enemies/Heriss.hpp`; `src/Entities/Enemies/Heriss.cpp` | `TASK` | `P3-AIR-HERISS-001` | Walk exists; current empty stomp handler cannot tell the runtime that contact is harmful. |
| `include/Entities/Enemies/Bowser.hpp`; `src/Entities/Enemies/Bowser.cpp` | `TASK` | `P3-BOSS-001` | Health and attack flags/timers exist; attacks have no world effect. |
| `include/Entities/Enemies/PeteyPiranha.hpp`; `src/Entities/Enemies/PeteyPiranha.cpp` | `TASK` | `P3-BOSS-001` | Emerge/retract motion exists; spikes/contact/rendering do not. |
| `include/Objects/Items/PowerUpObject.hpp`; `src/Objects/Items/PowerUpObject.cpp` | `TASK` | `P3-ITEM-001` | Existence lifecycle and empty collision hooks exist. |
| `include/Objects/Items/Item.hpp`; `src/Objects/Items/Item.cpp` | `TASK` | `P3-ITEM-001` | Name/value state exists. |
| `include/Objects/Items/Coin.hpp`; `src/Objects/Items/Coin.cpp` | `TASK` | `P3-ITEM-001` | Static/popped lifetime exists; collection only deactivates. |
| `include/Objects/Items/Mushroom.hpp`; `src/Objects/Items/Mushroom.cpp` | `TASK` | `P3-ITEM-001` | Horizontal motion and growth-on-collect exist; emergence, full world collision, and rendering do not. |
| `include/Objects/Items/PlayerBuff.hpp`; `src/Objects/Items/PlayerBuff.cpp` | `TASK` | `P3-ITEM-001`, `P3-PLAYER-STATE-001` | Timed owned buff foundation exists. |
| `include/Objects/Items/FireBuff.hpp`; `src/Objects/Items/FireBuff.cpp` | `TASK` | `P3-FIRE-001` | Fire state toggles; ammunition/request/shoot behavior does not. |

Accounting totals: 20 existing header/source pairs; 20 `TASK`; 0 `VERIFIED_DONE`; 0 `NO_CHANGE`; 0 `DEFERRED`; 0 `EXCLUDED`. Group5's LLM/Ollama/chatbot feature is scope-level `EXCLUDED`, not a P3 production component and not a task.

## 2. Exact current baseline

- `GameObject` owns an SFML texture/sprite, synchronizes position to sprite and public hitbox, synchronizes size to the hitbox, renders the sprite, stores a non-owning mediator pointer, and prints on every construction.
- `LivingEntity` owns `MovementComponent` and `AnimationComponent` slots by `std::unique_ptr`; current concrete constructors create movement only. Health/dead transitions work for ordinary damage. `updateAnimation` is empty at the base.
- `Character` tracks grounded/jumping and applies a jump velocity only while grounded.
- `PlayerManager` currently creates a 64x64 hitbox and grows directly to 64x128, with movement/jump, owned buffs, small/big/fire state, fire→big→small→dead damage downgrade, and temporary invulnerability. Clearance and foot-position preservation remain P3 work; the current release is 1P-only.
- Mario and Luigi provide distinct movement/jump tuning. Their update code requests `idle`/`big_idle`, but no texture is loaded, no `AnimationComponent` is allocated, and no frames are registered.
- Goomba, Koopa, FlyingKoopa, Heriss, Bowser, and PeteyPiranha contain ordinary deterministic movement/state logic except Bowser currently chooses with `rand()`. None initializes visible textures/animations. P2 can apply block physics to selected enemies and remove dead enemies, but no player/enemy contact loop exists.
- Goomba squishes for 0.5 seconds; Koopa enters shell/kick states; FlyingKoopa loses wings; Heriss's stomp method intentionally does nothing. These methods are not yet sufficient to communicate all runtime contact outcomes.
- Bowser only toggles breathing-fire/jump-slam flags and PeteyPiranha only changes y-position. Neither produces its required attack effect.
- `EntityFactory` returns `std::unique_ptr`, which is the correct ownership direction. Unknown enemies default to Goomba, unknown items default to Coin, and most player names default to Mario; those silent fallbacks can hide map/selection errors.
- `PowerUpObject` has an existence flag. Coin collection only deactivates it; Mushroom collection directly sets big; no score/coin result is emitted. P2 does not currently call item collection or apply item/block physics.
- `FireBuff` only enables/disables fire state; no shooting input, projectile request, cooldown, or ammunition use exists.
- Current production translation units passed C++20/SFML 3.1 syntax at audit time. There is no P3-specific runnable test and no visual/gameplay proof for P3 components.
- P2's current unique-owner/borrowed-player contracts are fixed baselines: P2 owns map/runtime entities; selected P3 players are borrowed; typed views never own. Person 3 must not regress this to raw owning pointers.

## 3. Remaining tasks in dependency order

### 3.1 Execution waves

| Wave | Tasks | Entry gate | Exit evidence | Parallel classification |
|---|---|---|---|---|
| 0A — independent foundations | `P3-FOUNDATION-001`, `P3-PLAYER-STATE-001` | Current audited source | Focused source checks and headless behavior checks | `READY_INDEPENDENT`. |
| 0B — existing-contract consumers | `P3-PLAYER-VISUAL-001`, `P3-GOOMBA-001` | Published P4 animation API and existing P2 block-physics contract | Focused behavior checks plus visual capture | `READY_WITH_STABLE_CONTRACT`. |
| 1 — remaining entity internals | `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001` | Foundation conventions stable; no conflicting edit to Enemy base | Deterministic state tests and visual capture | `READY_WITH_STABLE_CONTRACT`; P2 integration waits. |
| 2 — blocked ownership/outcome surfaces | `P3-PLAYER-RESET-001`, `P3-ITEM-001`, `P3-FACTORY-001` | Required decisions/contracts below are `APPROVED`/`READY` outside this file | P1/P2 integration tests plus P3 checks | `BLOCKED_BY_OTHER_TASK`/human decision. |
| 3 — fire handoff | `P3-FIRE-001` | Player state stable and P2 accepts a fire-request handoff under `CON-P2-P3-PROJECTILE-SPAWN` | Projectile request, adoption, cleanup, and gameplay check | `BLOCKED_BY_OTHER_TASK`. |

### 3.2 P3 contribution to the first playable vertical slice

Execute `P3-PLAYER-VISUAL-001` and `P3-PLAYER-STATE-001`, then `P3-GOOMBA-001`, then unblock the provider checklists inside `P3-ITEM-001` and `P3-FACTORY-001`. The vertical slice is not P3-complete until the owner approved by `DEC-PLAYER-OWNER` (P1 recommended) holds a visible selected player, P2 borrows it, one Goomba contact resolves once, one Coin or Mushroom effect resolves once, P2 removes inactive owned objects safely, and P1/P4 show the resulting session data. P3 does not implement the P1 state loop, P2 orchestration, or P4 HUD to make this pass.

### 3.3 Same-file sequencing and merge risk

- `PlayerManager.*`: complete `P3-PLAYER-STATE-001` before reset/item/fire edits; rebase each later task onto it. Never run these four tasks as concurrent file edits.
- `Enemy.*`: land `P3-FOUNDATION-001` before concrete enemy contact APIs. Concrete enemy pairs may then proceed in parallel.
- Animation key/texture setup across Player and Enemy files must use the same published `CON-P3-P4-ANIMATION` naming/error policy.
- `EntityFactory.*` lands after constructor/render-init policies settle, otherwise every concrete type change conflicts there.

## 4. Files allowed for Person 3 tasks

Only the following production paths are in P3's edit boundary:

- `SUPERMARIO/CS202_Group4_SuperMario/include/Core/GameObject.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Core/GameObject.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Entities/Base/*.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Entities/Base/*.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Entities/Players/*.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Entities/Players/*.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Entities/Enemies/*.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Entities/Enemies/*.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Entities/EntityFactory.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Entities/EntityFactory.cpp`
- `SUPERMARIO/CS202_Group4_SuperMario/include/Objects/Items/*.hpp`
- `SUPERMARIO/CS202_Group4_SuperMario/src/Objects/Items/*.cpp`

Task-owned focused checks may be added only at the exact `tests/P3*.cpp` paths named in each task card. Changes to `CMakeLists.txt` require a cross-owner edit request to P4/release coordination; direct C++20/SFML 3.1 compile/run commands are acceptable evidence until that request lands. Asset files are read-only inputs; map PNG contents are out of scope and must not be inspected or edited.

## 5. Files Person 3 may read but not edit

| Read-only dependency | Owner | Why P3 may inspect it | Required action if a change is needed |
|---|---:|---|---|
| `include/src/Components/MovementComponent.*` | P2 | Movement semantics and velocity contract | File a cross-owner request referencing the blocked task and contract. |
| `include/src/Physics/PhysicsEngine.*` | P2 | `canGrow`, grounding, block contact, and orchestration boundaries | P2 implements any physics/runtime change. |
| `include/src/Levels/Managers/{MapManager,LevelManager}.*` | P2 | Factory consumers, unique ownership, borrowed players, contact/removal timing | P2 implements adoption, contact loops, and cleanup. |
| `include/src/Objects/Environment/{Bullet,Rocket,Lava,WinFlag,Cannon}.*` | P2 | Existing projectile/hazard lifetime patterns | Do not repurpose or edit without an approved request. |
| `include/src/Components/AnimationComponent.*` | P4 | Published registration/play API | P4 implements component changes; P3 initializes and consumes it in P3-owned types. |
| `include/src/Objects/Blocks/*` | P4, except P2 MovingBlock | Payload spawn and block collision inputs | P4 implements block behavior; P2 maps/adopts payloads. |
| `include/src/Core/GameEventMediator.*`; `include/src/States/Base/GameState.*` | P1 | Outcome/event and selected-player owner-coordination surfaces | P1 owns event dispatch/state flow; player owner remains subject to `DEC-PLAYER-OWNER`. |
| `include/src/Core/UserData.*`; `include/src/UI/*` | P4 | Score/coin/lives and HUD data shape | P4 owns session persistence/display. |
| `src/main.cpp`; `include/Core/MyApp.hpp`; future `src/Core/MyApp.cpp` | P1 | Production construction path only | P1 owns startup. |
| `CMakeLists.txt`, runtime asset-copy/package rules | P4/release coordination | Test target and asset delivery | Submit cross-owner request; never silently edit. |

If a P3 task proves a required change in a read-only file, stop that task at `SOURCE_DONE` or earlier, record the exact API delta and reproducer in the master board's cross-owner request queue, name the receiving owner, and leave `INTEGRATION_DONE=NO` until the owner lands it.

## 6. Inputs and open gates

### 6.1 Required contract inputs

- `CON-P1-P3-PLAYER-OWNERSHIP`: P1 owns the selected-player `std::unique_ptr` under resolved `DEC-PLAYER-OWNER`; P2 receives a non-owning `PlayerManager*` view; P3 only supplies implementations/construction.
- `CON-P1-P3-PLAYER-RESET`: P1 invokes a P3 reset/reconstruction API only at a safe state boundary and reinjects stable borrowed pointers before P2 resumes.
- `CON-P2-P3-ENTITY-CONSTRUCTION`: P2 supplies validated map type/position; P3 returns a non-null render-ready enemy/item `unique_ptr` or explicit failure; P2 becomes sole runtime owner.
- `CON-P2-P3-PLAYER-ENEMY`: P2 classifies/orchestrates contact after movement; P3 owns enemy/player state transitions and reports deterministic outcomes.
- `CON-P2-P3-PLAYER-ITEM`: P2 orchestrates overlap, item/block/world physics, safe-growth clearance, and one-shot timing; P3 owns effect semantics and item lifecycle state.
- `CON-P2-P3-ENEMY-REMOVAL`: P3 exposes death/inactive state; P2 alone erases its `unique_ptr` owner after traversal and rebuilds views.
- `CON-P2-P3-PROJECTILE-SPAWN`: P3 emits a value/move-only boss/fire request; P2 constructs/adopts one runtime owner after traversal and owns collision/cleanup.
- `CON-P3-P4-ANIMATION`: P4 provides the SFML 3.1 animation component contract; P3 owns entity texture load, frame registration, key selection, sprite scaling/origin, and visible use.
- `CON-P1-P4-SCORE-COINS-LIVES` and `CON-P1-P4-HUD-DATA`: P3 produces typed gameplay outcomes through the agreed P1 event boundary. P3 does not mutate P4 UserData or HUD directly.

### 6.2 Human decisions — all remain `OPEN`

| Decision ID | P3 effect while open |
|---|---|
| `DEC-PLAYER-OWNER` | Blocks final player factory/owner signature and replacement timing. Recommended contract is P1 unique ownership, P2 borrowing. |
| `DEC-MULTIPLAYER-DEATH` | Blocks reset scope and whether one or both players are reconstructed. |
| `DEC-RESPAWN` | Blocks reset defaults, retained power state, and same-level reconstruction timing. |
| `DEC-BLOCK-ACTOR-ELIGIBILITY` | Blocks final player-power/headbutt eligibility data exposed to P2/P4; P3 must not encode the block rule. |
| `DEC-ASSET-ROOT` | Blocks final durable entity texture path/loading policy. |
| `DEC-PERSISTENCE` | Blocks which score/coin/power outcomes persist across death/level/session. |

`DEC-CLOUD-SEMANTICS` is resolved as deferred from the current release and `DEC-WINFLAG-POLISH` is resolved as animation-gated. Neither blocks a P3-owned source task. No task may resolve any decision by implementation side effect.

### 6.3 Fixed inputs

- Exactly nine levels exist in the completion matrix: `W1_LV1` through `W3_LV3`; do not duplicate P3 engine behavior per level.
- Adopted maps remain dynamic width and 45 image rows at `CELL_SIZE=64`; P2 consumes two logical 15-row runtime bands and ignores the final legacy band.
- P2's `std::unique_ptr` runtime owner and typed non-owning views are fixed baselines.
- The Group5 chatbot/LLM/Ollama/ChatUI system is `EXCLUDED`. Ordinary enemy behavior is required and is not “special AI.”

## 7. Outputs and contracts Person 3 must provide

| Contract ID | P3 role | P3 output | Consumer and timing | Failure behavior |
|---|---|---|---|---|
| `CON-P1-P3-PLAYER-OWNERSHIP` | Construction/type provider; never runtime owner | Concrete `std::unique_ptr<PlayerManager>` creation or an agreed equivalent preserving dynamic type | The owner approved by `DEC-PLAYER-OWNER` takes ownership before GameState loads; P2 only borrows after owner is stable | Invalid selection returns explicit failure; never silently creates another character. |
| `CON-P1-P3-PLAYER-RESET` | Reset/reconstruction provider | Deterministic reset API/result that clears transient state and places at P1/P2-provided spawn without retaining references | P1 coordinates between frames/reloads with the approved session owner | Invalid policy/input fails without deleting/replacing the live player in place. |
| `CON-P2-P3-ENTITY-CONSTRUCTION` | Enemy/item construction provider | Non-null, render-ready `unique_ptr<GameObject>` of the validated requested type and position, or explicit failure | P2 calls during load and takes sole ownership before registration | Unknown/null/asset failure is diagnosed and fails readiness; never substitute another type. |
| `CON-P2-P3-PLAYER-ENEMY` | Behavior provider | Contact capabilities and outcome: stompability, damage, score value, state transition, bounce/request data | P2 calls once per classified contact after movement and before cleanup | Unsupported/invalid contact produces no mutation and a diagnosable result. |
| `CON-P2-P3-PLAYER-ITEM` | Behavior provider | One-shot collection outcome, item existence, movement/contact response, and proposed growth bounds | P2 calls after movement/before cleanup and supplies clearance for feet-anchored 64×128 growth | Already-inactive item is a no-op; denied clearance leaves player and item unchanged; no double award. |
| `CON-P2-P3-ENEMY-REMOVAL` | Lifecycle provider | Stable `isDead`/inactive result after behavior update | P2 checks after traversal and owns erasure/view rebuild | P3 never deletes itself or any P2-owned object. |
| `CON-P2-P3-PROJECTILE-SPAWN` | Boss/fire request provider | Value or move-only attack kind/origin/direction/target identity with no retained raw pointer | P2 accepts at action edge, materializes/adopts after traversal, then owns collision/cleanup | Invalid/rejected request creates no partial owner and is diagnosable. |
| `CON-P3-P4-ANIMATION` | Consumer in P3-owned entities | Valid registered keys, texture-load result, scale/origin, state-to-key selection | P3 initializes before first render and plays once/update | Missing texture/key is reported and level/startup fails visibly according to the published contract; no warning flood per frame. |
| `CON-P1-P4-SCORE-COINS-LIVES` | Gameplay-outcome producer only | Typed score/coin/life-impact facts from enemy/item resolution | P1 mediates, P4 stores; emit exactly once per outcome | If no sink is installed, gameplay object lifecycle remains safe and loss is diagnosable. |
| `CON-P1-P4-HUD-DATA` | Indirect producer | State changes sufficient for P1/P4 to refresh HUD | P1/P4 consume after event application, not directly from P3 pointers | P3 never owns HUD state or retains HUD references. |

Contract invariants: P3 callbacks/results may contain values, IDs, or short-lived references documented for the current call only; they may not transfer player ownership, retain P2-owned raw pointers, mutate P1 state stacks during P2 traversal, or delete P2 owners. All spawn ownership crosses as `std::unique_ptr` or a value request that P2 materializes and owns.

## 8. Per-task implementation cards

### P3-FOUNDATION-001 — Entity/living/character foundation cleanup and contract checks

- **Owner:** Person 3.
- **Priority / requirement / status:** P1; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make the smallest shared P3 foundation deterministic and testable before concrete player/enemy work, without adding a new hierarchy or moving P2/P4 responsibilities into it.
- **Exact current behavior:** `GameObject` keeps position/size, sprite/texture, public hitbox, and a non-owning mediator pointer. `setPosition` synchronizes sprite/hitbox; `setSize` synchronizes the hitbox. Its constructor prints for every object. `LivingEntity` owns nullable movement/animation components, provides health/dead/move/update hooks, and syncs sprite/position after movement. `Character` jumps only when grounded. `Enemy` exposes facing, damage, point value, stomp state, direction reversal, and a default lethal stomp.
- **Missing behavior checklist:**
  - [ ] Remove unconditional constructor output and ensure normal construction/update emits no console noise.
  - [ ] Specify and test health/dead invariants (`health<=0` implies dead; damage on dead is a no-op; no accidental resurrection through ordinary setters).
  - [ ] Specify and test position/size/hitbox/sprite synchronization available through public observation.
  - [ ] Specify and test grounded/jump and enemy direction/stomp behavior used by P2.
  - [ ] Keep component ownership as `std::unique_ptr` and mediator as non-owning; document pointer lifetime at the setter.
  - [ ] Do not add a universal active flag: specialized enemy/item lifecycle plus P2 ownership already covers current requirements.
- **Exact target files:** `include/Core/GameObject.hpp`, `src/Core/GameObject.cpp`, `include/Entities/Base/LivingEntity.hpp`, `src/Entities/Base/LivingEntity.cpp`, `include/Entities/Base/Character.hpp`, `src/Entities/Base/Character.cpp`, `include/Entities/Base/Enemy.hpp`, `src/Entities/Base/Enemy.cpp`, `tests/P3FoundationContracts.cpp`.
- **Allowed edit files:** only the exact target files above.
- **Read-only dependencies:** `include/src/Components/MovementComponent.*`, `include/src/Components/AnimationComponent.*`, `include/src/Physics/PhysicsEngine.*`.
- **Do not implement:** P2 physics/collision/removal, P1 mediator/state flow, P4 animation internals, a new event bus, serialization, or per-level logic.
- **Dependencies:** none for source cleanup; tests compile against current P2/P4 component APIs.
- **Contracts provided / consumed:** provides stable base observations used by `CON-P2-P3-PLAYER-ENEMY` and `CON-P2-P3-ENEMY-REMOVAL`; consumes ownership portions of `CON-P3-P4-ANIMATION` and current P2 movement semantics.
- **Group5 reference:** none needed. **Do not copy:** Group5 raw-pointer owner/mediator patterns, its architecture, or chatbot code.
- **Implementation/integration notes:** prefer existing getters and virtual hooks; add only the minimum observation needed by a deterministic check. Treat `GameEventMediator*` as borrowed and never delete/store a state owner through it.
- **SFML 3.1 requirements:** use SFML 3.1 constructors and vector/rect APIs already present; no SFML 2 event or rect syntax.
- **Compile check:** compile the eight production translation units and `tests/P3FoundationContracts.cpp` with `-std=c++20` and SFML 3.1 headers; warnings introduced by the task are zero.
- **Runtime check:** the focused executable asserts position/size synchronization, one grounded jump, one direction reversal, health-to-dead transition, repeated-damage no-op, and quiet construction.
- **Visual check:** no new art is required; a one-frame render smoke check must not crash with a valid target. This dimension may be marked `VISUALLY_VERIFIED=YES` from the later entity visual integration, not from console/source review.
- **Gameplay check:** the first vertical slice must use these base contracts without duplicate movement or self-deletion.
- **Definition of Done:** all checklist items pass; public behavior is documented; no cross-owner file changed; all five dimension fields are updated with linked evidence, with gameplay/visual evidence allowed to come from the integrated slice.
- **Suggested commit:** `person3: stabilize entity foundation contracts`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-GOOMBA-001 — Complete Goomba behavior, contact API, lifecycle, and visuals

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** deliver the representative enemy required by the first vertical slice with deterministic stomp/harm/lifecycle behavior and visible animation.
- **Exact current behavior:** Goomba starts with one health, one damage, 100 points, a 64x64 hitbox, leftward walking, and a MovementComponent. `onStomped` changes it to a 64x32 hitbox, zero damage, and a squished state; after 0.5 seconds `dead=true`. P2 applies generic gravity/block collision and removes dead enemies. No player contact invokes this path, shrinking does not explicitly preserve the foot position, and no texture/animation component exists.
- **Missing behavior checklist:**
  - [ ] Expose a deterministic stompable/harmful contact capability/result for P2 without moving side classification into Goomba.
  - [ ] Preserve the bottom edge when entering the 16-pixel squish hitbox.
  - [ ] Make stomp idempotent; expose the 100-point award exactly once; suppress damage while squished/dead.
  - [ ] Register/play walk and squished visuals with correct 64-pixel world scale.
  - [ ] Ensure wall reversal still comes only through the P2 physics contract.
  - [ ] Verify death becomes visible to P2 only after the intended squish interval and never self-deletes.
- **Exact target files:** `include/Entities/Enemies/Goomba.hpp`, `src/Entities/Enemies/Goomba.cpp`, `tests/P3GoombaContracts.cpp`.
- **Allowed edit files:** exact target files only.
- **Read-only dependencies:** `Enemy.*`, P2 `PhysicsEngine.*`/`LevelManager.*`, P4 `AnimationComponent.*`, entity texture assets, P1/P4 score event surfaces.
- **Do not implement:** P2 overlap/side classification, stomp bounce, unique-owner deletion, P1 scoring, P4 HUD, per-level Goomba subclasses, or Group5 special AI.
- **Dependencies:** `P3-FOUNDATION-001`; `CON-P3-P4-ANIMATION` for visuals. Source behavior can land before P2 integration.
- **Contracts provided / consumed:** provides Goomba behavior under `CON-P2-P3-PLAYER-ENEMY` and lifecycle under `CON-P2-P3-ENEMY-REMOVAL`; produces one score outcome toward `CON-P1-P4-SCORE-COINS-LIVES`; consumes `CON-P3-P4-ANIMATION`.
- **Group5 reference:** its player/enemy ordering may be used only as a contact-order technique. **Do not copy:** raw-pointer deletion, mediator ownership, 64-pixel scale, or chatbot/LLM code.
- **Implementation/integration notes:** P2 classifies top-vs-side contact and applies player bounce/damage; P3 mutates only Goomba state and returns value outcomes. Never retain the borrowed player pointer.
- **SFML 3.1 requirements:** valid SFML 3.1 rect/texture APIs, stable texture lifetime, 64x64 walk visual and bottom-aligned squish visual.
- **Compile check:** syntax-check Goomba production and focused test against C++20/SFML 3.1 after foundation changes.
- **Runtime check:** assert walk direction, one reversal, first/repeated stomp, bottom-preserving shrink, 0.5-second boundary, one score outcome, damage suppression, dead state, and P2-safe removal observation.
- **Visual check:** capture walk frames in both directions and squished frame aligned to the same floor.
- **Gameplay check:** stomp one Goomba and take one side hit in the vertical slice; confirm bounce/damage and score happen once and the P2 owner is removed after traversal.
- **Definition of Done:** behavior/result API is deterministic and idempotent, visuals align, P2/P1/P4 integration passes once-only checks, and all five dimensions carry evidence.
- **Suggested commit:** `person3: complete Goomba contact and visuals`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-KOOPA-001 — Complete Koopa and shell behavior/contact API/visuals

- **Owner:** Person 3.
- **Priority / requirement / status:** P1; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make Koopa's walk→shell→kick lifecycle explicit enough for P2 to orchestrate player/enemy/block contacts safely.
- **Exact current behavior:** Koopa walks with a 64x96 hitbox and 200-point value. First `onStomped` enters a stationary 64x56 shell; a second calls `kickShell(true)` regardless of contact geometry; a stomp on a moving shell sets dead. A kicked shell rebuilds its MovementComponent at speed 300. No player runtime calls these methods, no shell-vs-enemy behavior is integrated, bottom alignment is not preserved explicitly, and no visuals exist.
- **Missing behavior checklist:**
  - [ ] Define explicit walk, stationary-shell, moving-shell, and dead transitions with idempotent result values.
  - [ ] Preserve bottom alignment when reducing height.
  - [ ] Require P2 to supply kick direction from contact geometry; remove the arbitrary rightward policy from the generic stomp path.
  - [ ] Expose moving-shell harmful/contact capability and one-shot score outcomes without owning the collision loop.
  - [ ] Register walk/shell/moving-shell visuals and facing behavior.
  - [ ] Verify P2 wall reversal and removal remain compatible with the state machine.
- **Exact target files:** `include/Entities/Enemies/Koopa.hpp`, `src/Entities/Enemies/Koopa.cpp`, `tests/P3KoopaContracts.cpp`.
- **Allowed edit files:** exact target files only.
- **Read-only dependencies:** P2 PhysicsEngine/LevelManager, P4 AnimationComponent, P1/P4 score sinks, entity assets.
- **Do not implement:** collision pair iteration, shell-vs-enemy overlap, owner deletion, block physics, score storage/HUD, new P2 projectile types, or raw owning pointers.
- **Dependencies:** `P3-FOUNDATION-001`; settled result vocabulary from `P3-GOOMBA-001`; animation contract.
- **Contracts provided / consumed:** provides state/contact behavior under `CON-P2-P3-PLAYER-ENEMY` and dead state under `CON-P2-P3-ENEMY-REMOVAL`; produces score facts for `CON-P1-P4-SCORE-COINS-LIVES`; consumes `CON-P3-P4-ANIMATION`.
- **Group5 reference:** shell interaction ordering may be adapted if useful. **Do not copy:** Group5 raw ownership, central mediator coupling, exact tuning, or chatbot code.
- **Implementation/integration notes:** prefer a small enum/result over ambiguous combinations of `inShell`, `shellKicked`, and `stomped`, if this reduces invalid states; this is not authorization for a shared abstraction beyond Koopa/FlyingKoopa.
- **SFML 3.1 requirements:** register only valid frame rectangles; mirror/orient with SFML 3.1 transforms while retaining floor alignment.
- **Compile check:** compile Koopa, FlyingKoopa consumer compatibility, and focused checks under C++20/SFML 3.1.
- **Runtime check:** assert walk→stationary shell, left/right kick, moving-shell reversal, repeated contacts, bottom alignment, score once, and terminal dead/removal signal.
- **Visual check:** capture walk, stationary shell, and moving shell in both directions at 64-pixel world scale.
- **Gameplay check:** in integrated runtime, stomp, kick from each side, hit a wall, hit one enemy, and be hit by a moving shell; each result occurs once with safe owner cleanup.
- **Definition of Done:** no ambiguous/arbitrary direction remains, state transitions and outcomes are tested, visuals are correct, P2 integration passes, and all five dimensions link evidence.
- **Suggested commit:** `person3: complete Koopa shell state contract`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-AIR-HERISS-001 — Complete FlyingKoopa and Heriss behavior/contact APIs/visuals

- **Owner:** Person 3.
- **Priority / requirement / status:** P1; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** finish the two non-default enemy contact policies: a gravity-exempt flying state that becomes Koopa, and a non-stompable spiked enemy.
- **Exact current behavior:** FlyingKoopa uses sine y-motion around `initialY`, horizontal direct position motion, and loses wings on the first stomp before delegating later stomps to Koopa; P2 intentionally excludes it from generic gravity while flying. Heriss walks under generic physics and its `onStomped` is an empty method. Neither has visible animation initialization or an integrated player contact path.
- **Missing behavior checklist:**
  - [ ] Preserve FlyingKoopa's gravity exemption only while wings are active, then enter the tested Koopa contract without position/velocity discontinuity.
  - [ ] Make wing-loss and subsequent stomp/kick outcomes explicit and idempotent.
  - [ ] Expose Heriss as non-stompable/harmful so a top contact damages the player instead of becoming a silent no-op.
  - [ ] Register fly/wing-loss/Koopa and Heriss walk visuals at correct scale/direction.
  - [ ] Define death/off-world signal only; P2 retains removal ownership.
  - [ ] Add deterministic checks independent of map contents.
- **Exact target files:** `include/Entities/Enemies/FlyingKoopa.hpp`, `src/Entities/Enemies/FlyingKoopa.cpp`, `include/Entities/Enemies/Heriss.hpp`, `src/Entities/Enemies/Heriss.cpp`, `tests/P3AirHerissContracts.cpp`.
- **Allowed edit files:** exact target files only.
- **Read-only dependencies:** `Koopa.*`, P2 participation policy/physics/contact runtime, P4 AnimationComponent, P1/P4 outcome sinks.
- **Do not implement:** P2 gravity-participation sets, overlap ordering, off-world erase, player damage/death transitions, camera, or map-specific behavior.
- **Dependencies:** `P3-FOUNDATION-001`; `P3-KOOPA-001` before final FlyingKoopa integration; `CON-P3-P4-ANIMATION`.
- **Contracts provided / consumed:** provides capabilities/results via `CON-P2-P3-PLAYER-ENEMY`, lifecycle via `CON-P2-P3-ENEMY-REMOVAL`, score facts via `CON-P1-P4-SCORE-COINS-LIVES`; consumes animation contract.
- **Group5 reference:** none is required beyond optional comparison of interaction order. **Do not copy:** raw owners, map tuning, 64-pixel assumptions, or chatbot/LLM components.
- **Implementation/integration notes:** keep ordinary deterministic state logic local. Do not label or implement it as special AI. P2 decides when gravity policy switches based on the P3-exposed state.
- **SFML 3.1 requirements:** current texture/rect/transform API only; make wing and spike silhouettes readable at 64-pixel scale.
- **Compile check:** syntax-check both enemy pairs and focused test with Koopa/base dependencies under C++20/SFML 3.1.
- **Runtime check:** assert sine motion bounds, first wing-loss, transition to Koopa behavior, repeated stomp safety, Heriss top-contact capability, wall reversal, and lifecycle signals.
- **Visual check:** capture FlyingKoopa with/without wings and Heriss walking in both directions; verify hitbox/art alignment.
- **Gameplay check:** stomp FlyingKoopa twice through P2, collide with Heriss from top/side, and confirm expected bounce/damage/score/removal without double resolution.
- **Definition of Done:** both special contact policies are explicit, tested, visible, integrated without P3 ownership changes, and all dimensions are evidenced.
- **Suggested commit:** `person3: finish flying Koopa and Heriss behavior`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-BOSS-001 — Complete Bowser and PeteyPiranha attacks/state/visuals

- **Owner:** Person 3.
- **Priority / requirement / status:** P1; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** turn boss timers/flags into deterministic, observable behavior requests that P2 can materialize/integrate and that players can fight.
- **Exact current behavior:** Bowser has 10 health, damage 2, 5000 points, walk movement, and every three seconds uses `rand()%2` to set either a one-second breathing-fire flag or jump velocity/`jumpingSlam`; no fire object or slam effect is produced. PeteyPiranha has five health, 2500 points, toggles emerged every 2.5 seconds, and directly sets y to base/base-48; no spike attack or player/contact contract exists. Neither has initialized visuals.
- **Missing behavior checklist:**
  - [ ] Replace ambient `rand()` dependence with a deterministic/test-injectable attack sequence/timing.
  - [ ] Expose value/owned-request outputs for Bowser fire and slam effects and Petey spike attacks under the P2/P3 contact contract; never retain P2/player pointers.
  - [ ] Define vulnerable/harmful windows, damage reception, terminal death, one-shot score/completion outcome, and repeated-hit suppression.
  - [ ] Complete jump-slam landing reset and Petey emerge/retract/pipe-relative contact states.
  - [ ] Register required idle/walk/attack/hurt/dead/emerge visuals and align current boss hitboxes to 64-pixel tiles.
  - [ ] Verify P2 adopts/materializes attack requests with `std::unique_ptr`, resolves world/player contact, and cleans them up.
- **Exact target files:** `include/Entities/Enemies/Bowser.hpp`, `src/Entities/Enemies/Bowser.cpp`, `include/Entities/Enemies/PeteyPiranha.hpp`, `src/Entities/Enemies/PeteyPiranha.cpp`, `tests/P3BossContracts.cpp`. Add a P3-owned attack-result/value header under `include/Entities/Enemies/` only if the existing boss headers cannot express the handoff without duplication; do not add a new class until the contract is approved.
- **Allowed edit files:** exact target files; any proposed new P3 value header must be recorded on the master board before creation.
- **Read-only dependencies:** P2 LevelManager/PhysicsEngine and projectile/adoption patterns, P4 AnimationComponent, P1 completion/score events, entity assets.
- **Do not implement:** P2 world object ownership/contact loops, direct player damage, P1 boss-level completion/score storage, P4 HUD/audio, map-specific boss placement, raw owning callbacks, or chatbot/LLM behavior.
- **Dependencies:** `P3-FOUNDATION-001`; result conventions from representative enemies; P2 acceptance of attack-request timing under `CON-P2-P3-PLAYER-ENEMY`; animation contract. Internal deterministic timers may proceed first.
- **Contracts provided / consumed:** provides boss contact/outcome data through `CON-P2-P3-PLAYER-ENEMY`, attack requests through `CON-P2-P3-PROJECTILE-SPAWN`, death through `CON-P2-P3-ENEMY-REMOVAL`, and score facts through `CON-P1-P4-SCORE-COINS-LIVES`; consumes `CON-P3-P4-ANIMATION`.
- **Group5 reference:** boss timing/attack ideas may be behavior references only if present. **Do not copy:** raw projectiles, mediator ownership, exact balance, 64-pixel world assumptions, or special chatbot AI.
- **Implementation/integration notes:** “AI” here means no external model/service; use an explicit deterministic state/timer sequence. P2 owns all spawned runtime attack objects after a unique/value handoff.
- **SFML 3.1 requirements:** valid texture/rect/transform usage and no per-frame missing-animation warnings; render boss/attack states at intended world scale.
- **Compile check:** compile both bosses and focused deterministic test under C++20/SFML 3.1, with a stub consumer for attack requests.
- **Runtime check:** fixed-step tests assert exact attack sequence/timing, fire/slam/spike request counts, vulnerability windows, damage/death, one score/completion result, and no request after death.
- **Visual check:** capture each required state and verify hitbox/art/pipe alignment; observe attack cue before harmful contact.
- **Gameplay check:** defeat each boss through the production P2/P1 flow; each attack can affect the player, can be cleaned up, and boss death advances exactly once.
- **Definition of Done:** attacks have deterministic world effects through P2, bosses are visible/fightable, death/outcome is once-only, no ownership regression exists, and all five dimensions are evidenced.
- **Suggested commit:** `person3: complete deterministic boss attacks`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-PLAYER-VISUAL-001 — Mario/Luigi texture, animation, and render setup

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make both selectable players visibly render and animate through P4's component API in the first playable slice.
- **Exact current behavior:** Mario and Luigi construct movement components and distinct tuning. Their `updateAnimation` requests only `idle` or `big_idle`; `LivingEntity::animationComponent` remains null, `entityTexture` is never loaded, frames are not registered, no directional sprite policy exists, and all player forms therefore render no usable art.
- **Missing behavior checklist:**
  - [ ] Load the agreed Mario/Luigi texture using the approved `DEC-ASSET-ROOT` result or a temporary injected/readiness surface that does not hard-code a competing root.
  - [ ] Construct `AnimationComponent` once per player and register the agreed small/big/fire idle, walk, jump, and transition keys required by reachable states.
  - [ ] Choose animation from movement, grounded/jumping, direction, power state, damage/transform state; call `play` once per update.
  - [ ] Scale/origin the sprite to match the 64x64 or 64x128 hitbox while preserving feet and horizontal facing.
  - [ ] Expose texture/animation initialization failure without warning every frame.
  - [ ] Verify Mario and Luigi use their intended atlas regions and remain distinguishable in 1P and 2P.
- **Exact target files:** `include/Entities/Players/PlayerManager.hpp`, `src/Entities/Players/PlayerManager.cpp`, `include/Entities/Players/Mario.hpp`, `src/Entities/Players/Mario.cpp`, `include/Entities/Players/Luigi.hpp`, `src/Entities/Players/Luigi.cpp`, `tests/P3PlayerVisualContracts.cpp`.
- **Allowed edit files:** exact target files only; texture PNGs are read-only.
- **Read-only dependencies:** `include/src/Components/AnimationComponent.*`, P4's `CON-P3-P4-ANIMATION` card, runtime asset-root/package policy, `assets/textures/Mario_Luigi.png`.
- **Do not implement:** AnimationComponent internals, asset packaging, menus/player ownership, camera, physics, HUD, map parsing, or new art.
- **Dependencies:** `P3-FOUNDATION-001` for settled base behavior; published `CON-P3-P4-ANIMATION`; final durable path waits on `DEC-ASSET-ROOT` even though frame/key preparation may proceed.
- **Contracts provided / consumed:** consumes `CON-P3-P4-ANIMATION` and `CON-P1-P3-PLAYER-OWNERSHIP`; provides render-ready players to P1/P2 integration.
- **Group5 reference:** atlas/frame coordinates may be checked as `REFERENCE_ONLY` if the same asset is proven identical. **Do not copy:** Group5 raw texture ownership, scale assumptions (`CELL_SIZE=64`), event loop, player owners, or chatbot code.
- **Implementation/integration notes:** keep texture lifetime inside the owning player so the reference-bound sprite/component never outlives it. Do not share a temporary local texture. Register only reachable keys, then expand when a task adds a reachable state.
- **SFML 3.1 requirements:** use `sf::IntRect({x,y},{w,h})`/other valid SFML 3.1 forms, `sf::Texture::loadFromFile`, and current sprite transform APIs; do not use SFML 2 constructors.
- **Compile check:** syntax-check all six target production translation units plus the focused check against installed SFML 3.1.
- **Runtime check:** construct/destroy both players repeatedly; switch small/big/fire, idle/walk/jump/direction states; assert no missing-key flood and stable texture/component lifetime.
- **Visual check:** capture both characters standing, moving, jumping, and in each reachable power state against a contrasting background at 64-pixel tile scale; verify feet do not jump when frame/form changes.
- **Gameplay check:** in the first vertical slice, select each character (and both in 2P), enter a level, move/jump, take damage, and remain visible/correctly oriented.
- **Definition of Done:** both players load or fail explicitly, every reachable state has a registered visual, scaling aligns with hitboxes, 1P/2P visual checks pass, and all five completion dimensions link evidence.
- **Suggested commit:** `person3: render and animate Mario and Luigi`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-PLAYER-STATE-001 — Safe growth, power, and damage-state behavior

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `REQUIRED_BY_PLAN`; `READY`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make player power transitions deterministic, collision-safe, and usable by P2 contacts and P1 death flow.
- **Exact current behavior:** `PlayerManager` tracks small/big/fire, buffs, invulnerability, transform flags, and 1P/2P input. `setBig`/`setFire` immediately resize the hitbox from its current top-left; no clearance query or foot preservation occurs. Damage downgrades fire→big→small, then reduces health; invulnerability times out. Fire state implies big. Transform fields are not driven, flashing is not rendered, and input includes no fire action.
- **Missing behavior checklist:**
  - [ ] Define one canonical small/big/fire transition path and invariant (`fire => big`).
  - [ ] Preserve the player's feet when height changes; reject/defer growth when P2 reports insufficient clearance through the agreed safe-growth input.
  - [ ] Prevent item double-application and damage re-entry during transform/invulnerability.
  - [ ] Make damage, invulnerability expiry, dead input suppression, and buff removal deterministic under large/small `dt` values.
  - [ ] Expose only the minimal state/capability P2/P4 need for contact/block eligibility; do not decide `DEC-BLOCK-ACTOR-ELIGIBILITY` here.
  - [ ] Add state checks for small→big→fire→big→small→dead and blocked growth.
- **Exact target files:** `include/Entities/Players/PlayerManager.hpp`, `src/Entities/Players/PlayerManager.cpp`, `include/Objects/Items/PlayerBuff.hpp`, `src/Objects/Items/PlayerBuff.cpp`, `tests/P3PlayerStateContracts.cpp`.
- **Allowed edit files:** exact target files only.
- **Read-only dependencies:** `include/src/Physics/PhysicsEngine.*` (`canGrow`), block views/collision order in `LevelManager.*`, P4 Block interfaces, P1 death event contract.
- **Do not implement:** a second physics query, P2 block traversal, P4 block eligibility, P1 lives/death transition, reset/respawn policy (`P3-PLAYER-RESET-001`), fire projectile behavior (`P3-FIRE-001`), or direct UserData mutation.
- **Dependencies:** `P3-FOUNDATION-001`; safe-growth integration consumes a P2-provided clearance result. The internal state machine can land before P2 integration.
- **Contracts provided / consumed:** provides player state/capabilities and proposed feet-anchored growth bounds under `CON-P2-P3-PLAYER-ENEMY` and `CON-P2-P3-PLAYER-ITEM`; consumes P2's clearance result, `CON-P1-P3-PLAYER-OWNERSHIP`, and the resolved `DEC-BLOCK-ACTOR-ELIGIBILITY` rule for later external use.
- **Group5 reference:** none is necessary for Group4's power-state rules. **Do not copy:** Group5 health/owner architecture or direct UserData mutation.
- **Implementation/integration notes:** P2 should perform clearance against its current block set and pass a decision/result; P3 applies or defers the form transition. Do not let P3 cache a block vector or PhysicsEngine pointer.
- **SFML 3.1 requirements:** state logic is headless; any flash/visibility integration uses current SFML 3.1 sprite color APIs through the visual task.
- **Compile check:** syntax-check production and focused test with C++20/SFML 3.1; compile together with P2 headers without editing them.
- **Runtime check:** assert all transition sequences, feet preservation, denied growth, repeated same-frame collection/damage, invulnerability boundaries, dead input suppression, and buff expiry.
- **Visual check:** verify each form's hitbox/sprite alignment and flashing/transition presentation with `P3-PLAYER-VISUAL-001`.
- **Gameplay check:** collect Mushroom under open and low-ceiling cases, take two/three staged hits as appropriate, and confirm exactly one death callback only after the actual lethal transition.
- **Definition of Done:** the canonical state machine and safe-growth handoff are documented and tested; no direct cross-owner mutation exists; integration proves damage/growth in real P2 world geometry; five dimensions are updated separately.
- **Suggested commit:** `person3: make player power transitions collision safe`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-PLAYER-RESET-001 — Explicit reset/reconstruction API

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `IMPLIED_BY_CURRENT_GROUP4_DESIGN`; `BLOCKED`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** give P1 a safe, explicit way to reset or reconstruct a selected player after death/reload while preserving P2's borrowed-pointer lifetime rules.
- **Exact current behavior:** no reset/reconstruction API exists. P1 does not own production players. P2 stores borrowed `PlayerManager*`. `PlayerManager` holds movement state, buffs, power flags, health/dead, grounded/jumping, invulnerability/transform timers, and position; ad hoc reuse would leave transient state behind.
- **Missing behavior checklist:**
  - [ ] Record approved semantics from `DEC-PLAYER-OWNER`, `DEC-MULTIPLAYER-DEATH`, and `DEC-RESPAWN` without resolving them in code.
  - [ ] Provide the agreed reset-in-place and/or reconstruction entry point with explicit spawn position and retained-session inputs.
  - [ ] Reset velocity, forces, buffs, form/health per policy, dead/grounded/jump/invulnerability/transform timers, and visual state deterministically.
  - [ ] Ensure P1 replaces/reinjects borrowed pointers only between P2 update traversals.
  - [ ] Cover one-player and two-player identity/control preservation.
  - [ ] Return explicit failure for invalid spawn/policy input without partially resetting the live object.
- **Exact target files:** `include/Entities/Players/PlayerManager.hpp`, `src/Entities/Players/PlayerManager.cpp`, `include/Entities/Players/Mario.hpp`, `src/Entities/Players/Mario.cpp`, `include/Entities/Players/Luigi.hpp`, `src/Entities/Players/Luigi.cpp`, `tests/P3PlayerResetContracts.cpp`.
- **Allowed edit files:** exact target files only, and only after blockers are cleared.
- **Read-only dependencies:** P1 `GameState`/selection owner API, P2 `LevelManager::setPlayers`/load/spawn data, P4 UserData persistence.
- **Do not implement:** P1 ownership/state transitions, decrement lives, Game Over, level reload, P2 spawn extraction, P2 pointer replacement, P4 persistence, or a policy while its decision is open.
- **Dependencies:** `P3-PLAYER-STATE-001`; approved `DEC-PLAYER-OWNER`, `DEC-MULTIPLAYER-DEATH`, `DEC-RESPAWN`; ready `CON-P1-P3-PLAYER-RESET` and `CON-P1-P3-PLAYER-OWNERSHIP`.
- **Contracts provided / consumed:** provides the P3 half of `CON-P1-P3-PLAYER-RESET`; consumes `CON-P1-P3-PLAYER-OWNERSHIP`; P2 lifetime integration remains P1/P2 work.
- **Group5 reference:** its fresh-player reconstruction followed by map-spawn placement is a behavior reference only. **Do not copy:** Group5 raw owners, direct health/lives policy, mediator, or menu architecture.
- **Implementation/integration notes:** replacement is safer than in-place reset only if P1 can replace owners and reinject all borrowed views between frames; otherwise a complete reset-in-place contract is needed. The human decision selects this, not P3.
- **SFML 3.1 requirements:** recreate/rebind reference-bound texture/sprite/AnimationComponent state without dangling references; use SFML 3.1 transforms.
- **Compile check:** compile P3 reset check plus P1/P2 public headers under C++20/SFML 3.1.
- **Runtime check:** seed every transient field, reset at a safe boundary, assert approved retained/cleared fields, stable identity/control, valid animation lifetime, and no stale movement/buff.
- **Visual check:** after reset/reconstruction, the player appears once at the supplied spawn with the approved form and no stale flash/frame.
- **Gameplay check:** exercise pit/enemy death in 1P and each affected-player case in 2P; prove no P2 dangling pointer and correct restart/Game Over handoff.
- **Definition of Done:** decisions are approved and cited, the API is atomic/deterministic, P1/P2 integration replaces or resets only between traversals, 1P/2P checks pass, and all five dimensions carry evidence.
- **Suggested commit:** `person3: add explicit player reset contract`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-ITEM-001 — Coin/Mushroom collection effects, world behavior, and visuals

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `REQUIRED_BY_PLAN`; `BLOCKED`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make map and block-spawned items produce one deterministic gameplay outcome, participate safely in the world, render visibly, and hand session facts to P1/P4 without direct coupling.
- **Exact current behavior:** `PowerUpObject` has `name`, `exist`, empty collision hooks, and visible-only-if-existing render. `Item` adds an integer value. Coin currently uses a 54x64 hitbox; static coins do nothing until a missing runtime collection call, popped coins move up/down and self-expire after 0.5 seconds, and `onCollect` only clears `exist`. Mushroom is 64x64, walks right using its own MovementComponent, reverses only if its hook is called, ignores the `popped` argument, applies `setBig(true)`, and deactivates. P2 owns/removes items and P4's callback can spawn Coin/Mushroom, but P2 has no player/item or item/block loop. No item initializes art.
- **Missing behavior checklist:**
  - [ ] Define a typed, one-shot collection result for coin count/score, growth request, and no-op/denied growth; never mutate UserData/HUD directly.
  - [ ] Make static/popped Coin lifecycles and values explicit; collection/expiry must not double-award.
  - [ ] Complete Mushroom emergence from a block, gravity/block motion capability, wall reversal, and collection state while leaving orchestration to P2.
  - [ ] Reuse the canonical player growth path from `P3-PLAYER-STATE-001`, including denied/deferred growth semantics.
  - [ ] Register Coin/Mushroom visuals/animations and align them to 64-pixel world tiles.
  - [ ] Prove raw block-spawn callback payload is adopted immediately by P2 and never retained/owned by P3/P4.
- **Exact target files:** `include/Objects/Items/PowerUpObject.hpp`, `src/Objects/Items/PowerUpObject.cpp`, `include/Objects/Items/Item.hpp`, `src/Objects/Items/Item.cpp`, `include/Objects/Items/Coin.hpp`, `src/Objects/Items/Coin.cpp`, `include/Objects/Items/Mushroom.hpp`, `src/Objects/Items/Mushroom.cpp`, `include/Objects/Items/PlayerBuff.hpp`, `src/Objects/Items/PlayerBuff.cpp`, `include/Entities/Players/PlayerManager.hpp`, `src/Entities/Players/PlayerManager.cpp`, `tests/P3ItemContracts.cpp`.
- **Allowed edit files:** exact target files only after blockers clear.
- **Read-only dependencies:** P2 LevelManager/PhysicsEngine and ownership callbacks, P4 CoinBlock/MushroomBlock/AnimationComponent, P1 GameEventMediator, P4 UserData/HUD, item texture assets.
- **Do not implement:** P2 overlap/block physics/adoption, P4 block payload/variant behavior, P1 event storage, P4 UserData/HUD, direct level-state transitions, map parsing, or unique-owner deletion.
- **Dependencies:** `P3-PLAYER-STATE-001`, ready `CON-P2-P3-PLAYER-ITEM`, published outcome fields in `CON-P1-P4-SCORE-COINS-LIVES`, animation contract; `DEC-PERSISTENCE` affects retained outcomes, not once-only item lifecycle. Remains `BLOCKED` until cross-owner contracts are approved.
- **Contracts provided / consumed:** provides collection/motion/lifecycle results and proposed growth bounds under `CON-P2-P3-PLAYER-ITEM`; consumes P2's clearance result and `CON-P3-P4-ANIMATION`; produces score/coin facts for `CON-P1-P4-SCORE-COINS-LIVES` and indirect `CON-P1-P4-HUD-DATA`.
- **Group5 reference:** player/power-up and power-up/block interaction ordering may be adapted as a technique. **Do not copy:** Group5 raw-pointer vectors, mediator-held owners, 64-pixel tuning, direct UserData calls, or chatbot code.
- **Implementation/integration notes:** P2 detects overlap and calls a P3 item effect exactly once; P3 marks the item inactive atomically with producing the result; P1/P4 apply/display the result. Mushroom may expose movement/collision inputs, but must not cache P2's block list.
- **SFML 3.1 requirements:** valid texture/rect APIs, stable texture lifetime, correct sprite scaling; no SFML 2 test syntax.
- **Compile check:** compile all item production units, PlayerManager consumer, and focused tests under C++20/SFML 3.1.
- **Runtime check:** static coin, popped coin timeout, coin collect-vs-expire race, Mushroom emerge/walk/reverse, open/blocked growth, repeated collect, inactive update/render, and ownership handoff all assert exactly-once behavior.
- **Visual check:** capture static/popped Coin and emerging/walking Mushroom against 64-pixel blocks; verify no clipping or half-scale art.
- **Gameplay check:** collect a map Coin, a CoinBlock Coin, and a MushroomBlock Mushroom; each updates the agreed session/HUD value once, Mushroom interacts with blocks, and P2 removes it safely.
- **Definition of Done:** collection outcomes are typed/once-only, world behavior and art are integrated, no direct P1/P4/P2 ownership coupling was added, and every completion dimension has evidence.
- **Suggested commit:** `person3: complete item collection behavior`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-FIRE-001 — FireBuff shooting and projectile behavior contract

- **Owner:** Person 3.
- **Priority / requirement / status:** P1; `REQUIRED_BY_PLAN`; `BLOCKED`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** turn fire state into a usable player ability while preserving the player owner approved by `DEC-PLAYER-OWNER` and P2 runtime projectile ownership.
- **Exact current behavior:** `FireBuff` optionally lasts for a duration and calls `setFire(true/false)`. It stores an unused `fireballCount=10`. Player input has movement/jump only. There is no shoot action, cooldown, request/result, projectile type choice, P2 spawn adoption, block/enemy/world collision, or cleanup integration.
- **Missing behavior checklist:**
  - [ ] Publish the fire-input/cooldown/ammunition semantics and a value or move-only handoff accepted by P2 under `CON-P2-P3-PROJECTILE-SPAWN`.
  - [ ] Add 1P/2P shoot bindings without interfering with movement/jump.
  - [ ] Emit at most one request per allowed press/cooldown, with direction/spawn position/value data and no retained player/projectile pointers.
  - [ ] Consume ammunition/cooldown deterministically; suppress shooting when non-fire, dead, transforming, or paused by P1.
  - [ ] Coordinate P2 projectile adoption, world/block/enemy contact, off-world cleanup, and owner removal.
  - [ ] Add player fire/shoot visual state through the existing animation contract.
- **Exact target files:** `include/Objects/Items/FireBuff.hpp`, `src/Objects/Items/FireBuff.cpp`, `include/Entities/Players/PlayerManager.hpp`, `src/Entities/Players/PlayerManager.cpp`, `tests/P3FireContracts.cpp`. Any new P3-owned fire-request value header must be approved and recorded before creation.
- **Allowed edit files:** exact target files only after contract approval.
- **Read-only dependencies:** P2 LevelManager/projectile/physics/off-world cleanup, P4 AnimationComponent/AudioSystem, P1 pause/input state, existing SFML input API.
- **Do not implement:** P2 projectile ownership/contact/culling, edit P2 Bullet/Rocket, P1 pause/state handling, P4 sound, direct enemy deletion/score, map-specific fire rules, or raw owning pointers.
- **Dependencies:** `P3-PLAYER-STATE-001`, `P3-ITEM-001`, approved P2 adoption/timing within `CON-P2-P3-PROJECTILE-SPAWN`; source remains `BLOCKED` until the handoff shape is ready.
- **Contracts provided / consumed:** provides shoot capability/request via `CON-P2-P3-PROJECTILE-SPAWN`; item acquisition uses `CON-P2-P3-PLAYER-ITEM`; enemy effects ultimately use `CON-P2-P3-PLAYER-ENEMY` and `CON-P2-P3-ENEMY-REMOVAL`; consumes `CON-P3-P4-ANIMATION`.
- **Group5 reference:** none is required. **Do not copy:** Group5 projectile owners, event architecture, input loop, or chatbot/LLM feature.
- **Implementation/integration notes:** use edge-triggered input or a documented repeat policy rather than spawning every frame. P2 owns the projectile immediately after handoff and may materialize a value request into its own projectile type.
- **SFML 3.1 requirements:** current `sf::Keyboard::Key` syntax, valid sprite/texture APIs, and no SFML 2 event-field access.
- **Compile check:** compile FireBuff, PlayerManager, and a fake P2 request consumer under C++20/SFML 3.1.
- **Runtime check:** assert non-fire suppression, press/cooldown behavior, both directions, 1P/2P bindings, ammunition/expiry, damage removing fire, no requests after death, and one owner handoff/request per shot.
- **Visual check:** verify fire-form and shoot frames align in both directions; projectile creation cue begins at the correct side of the player.
- **Gameplay check:** acquire fire state, shoot one enemy/block/world edge, observe P2 collision/cleanup and one score outcome, then take damage and verify shooting is disabled.
- **Definition of Done:** request/ownership contract is explicit, input and cooldown are deterministic, P2 integration/cleanup and visuals pass, and all five dimensions are evidenced.
- **Suggested commit:** `person3: add fire ability request contract`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

### P3-FACTORY-001 — EntityFactory failure, player construction, and render-init policy

- **Owner:** Person 3.
- **Priority / requirement / status:** P0; `REQUIRED_BY_PLAN`; `BLOCKED`. `STATUS_CHANGED_SINCE_AUDIT: NO`.
- **Purpose:** make construction errors explicit and deliver concrete, render-ready player/enemy/item owners to P1/P2 without hidden defaults or ownership ambiguity.
- **Exact current behavior:** factory methods return `std::unique_ptr`. Enemy aliases cover all six enemy types but unknown values silently create Goomba. Item recognizes Mushroom and otherwise creates Coin. Player recognizes Luigi aliases and otherwise creates Mario, returning `std::unique_ptr<Character>`; it redundantly sets hitbox position after `setPosition`. P2 consumes enemy/item factories; no P1 production code consumes the player factory. Render initialization is left to constructors but currently absent.
- **Missing behavior checklist:**
  - [ ] Define accepted canonical names/aliases and return explicit failure for unknown/unsupported strings; never silently substitute a different gameplay object.
  - [ ] Return or expose a player type P1 can own and P2 can borrow as `PlayerManager*` without unsafe downcasting.
  - [ ] Preserve `std::unique_ptr` transfer for enemies/items/players; remove redundant position mutation.
  - [ ] Define whether concrete constructors or the factory owns render initialization; implement exactly one path using `CON-P3-P4-ANIMATION`.
  - [ ] Surface texture/construction failure distinctly from unknown type according to the approved asset/readiness contract.
  - [ ] Test every accepted type and every invalid-family case.
- **Exact target files:** `include/Entities/EntityFactory.hpp`, `src/Entities/EntityFactory.cpp`, `tests/P3EntityFactoryContracts.cpp`.
- **Allowed edit files:** exact target files only after owner/signature contract approval.
- **Read-only dependencies:** all concrete P3 constructors, P1 selection/GameState owner API, P2 MapManager/LevelManager factory call sites, P4 AnimationComponent/asset/package readiness.
- **Do not implement:** player/session ownership policy or P1 owner-coordination code, P2 map decoder/adoption, P4 animation internals/asset root, exception-heavy registry abstractions, a service locator, map aliases not present in the approved palette, or raw ownership.
- **Dependencies:** approved `DEC-PLAYER-OWNER`, ready `CON-P1-P3-PLAYER-OWNERSHIP`, and `CON-P2-P3-ENTITY-CONSTRUCTION`; `P3-PLAYER-VISUAL-001`; relevant enemy/item constructors; `DEC-ASSET-ROOT` for durable readiness. Remains `BLOCKED` until signature/owner contract is approved.
- **Contracts provided / consumed:** provides player owners through `CON-P1-P3-PLAYER-OWNERSHIP` and render-ready enemy/item owners through `CON-P2-P3-ENTITY-CONSTRUCTION`; consumes `CON-P3-P4-ANIMATION`.
- **Group5 reference:** no factory architecture should be copied. **Do not copy:** raw allocation/owner patterns, fallbacks that hide map errors, 64-pixel assumptions, or chatbot services.
- **Implementation/integration notes:** a small `nullptr` plus caller-readable error/result is preferable to inventing a registry. P1 takes the player `unique_ptr`; P2 takes enemy/item `unique_ptr`; no factory singleton owns instances.
- **SFML 3.1 requirements:** construction must leave sprite/texture references valid; any readiness result reflects SFML 3.1 `loadFromFile` behavior.
- **Compile check:** compile factory plus all concrete constructors and focused test under C++20/SFML 3.1; compile P1/P2 public call sites against the changed signature before merge.
- **Runtime check:** construct every canonical/accepted alias at a known position, prove correct dynamic type/position/owner transfer/readiness, and prove invalid enemy/item/player returns explicit failure with no allocation leak.
- **Visual check:** render one instance of every constructed visible type; invalid construction renders nothing and reports once.
- **Gameplay check:** P1 selects Mario/Luigi through the production factory owner path and P2 loads representative enemy/item types without fallback substitution; invalid data produces a visible load error, not a playable wrong object.
- **Definition of Done:** ownership/signature and failure policy are approved, every factory path is tested, all returned gameplay types are render-ready or fail explicitly, consumers compile/integrate, and all five dimensions are evidenced.
- **Suggested commit:** `person3: make entity factory failures explicit`

Completion dimensions: `SOURCE_DONE=NO`, `INTEGRATION_DONE=NO`, `RUNTIME_TESTED=NO`, `VISUALLY_VERIFIED=NO`, `GAMEPLAY_VERIFIED=NO`.

## 9. Tests required per task

Each task leaves the smallest runnable deterministic check. Tests do not inspect or edit map images. Until P4/release coordination adds test targets, record the exact C++20/SFML 3.1 compile/link command and executable exit result in the master board; syntax-only evidence cannot satisfy `RUNTIME_TESTED`.

| Task ID | Focused runnable check | Required assertions | Negative/edge assertions | Evidence needed to leave `TESTING` |
|---|---|---|---|---|
| `P3-FOUNDATION-001` | `tests/P3FoundationContracts.cpp` | position/size sync, grounded jump, direction reversal, health/death | repeated damage; null optional components; quiet construction | exit 0 plus assertion count/log |
| `P3-PLAYER-VISUAL-001` | `tests/P3PlayerVisualContracts.cpp` plus off-screen/window capture | both types initialize; all reachable keys switch; texture lifetime stable | missing asset/key reported once; destruction/reconstruction | exit 0 and reviewed captures for each character/form |
| `P3-PLAYER-STATE-001` | `tests/P3PlayerStateContracts.cpp` | small/big/fire chain, feet preservation, invulnerability, buffs | denied growth, repeated damage/collect, dead input, large `dt` | exit 0 plus integrated low-ceiling case |
| `P3-PLAYER-RESET-001` | `tests/P3PlayerResetContracts.cpp` | every transient field resets per approved policy; identity/controls/spawn | invalid policy/spawn atomic failure; 2P unaffected-player case | exit 0 plus P1/P2 pointer-lifetime run |
| `P3-GOOMBA-001` | `tests/P3GoombaContracts.cpp` | walk, stomp, bottom alignment, timer, points once | repeated stomp, side capability, no self-delete | exit 0 plus P2 contact/removal check |
| `P3-KOOPA-001` | `tests/P3KoopaContracts.cpp` | walk/shell/left-right kick/moving-shell/death | arbitrary-direction prevention, repeated contacts, wall reversal | exit 0 plus shell-vs-player/enemy integration |
| `P3-AIR-HERISS-001` | `tests/P3AirHerissContracts.cpp` | flight bound, wing loss, Koopa transition, harmful Heriss top | repeated wing loss/stomp; gravity-policy switch | exit 0 plus P2 contact/participation run |
| `P3-BOSS-001` | `tests/P3BossContracts.cpp` | exact attack timeline/count, vulnerable windows, damage/death/outcome | no requests after death; fixed-step repeatability; invalid consumer | exit 0 plus production attack adoption/cleanup |
| `P3-ITEM-001` | `tests/P3ItemContracts.cpp` | Coin/Mushroom once-only effects, emergence/motion/reversal/lifecycle | collect-expire race, inactive no-op, denied growth, handoff | exit 0 plus P2/P1/P4 item→HUD integration |
| `P3-FIRE-001` | `tests/P3FireContracts.cpp` | key edge, cooldown/ammunition, direction/request, owner handoff | non-fire/dead/paused suppression, held key, buff removal | exit 0 plus one production projectile lifecycle |
| `P3-FACTORY-001` | `tests/P3EntityFactoryContracts.cpp` | all canonical types/aliases, dynamic type, position, owner transfer | unknown enemy/item/player, asset failure, no silent fallback | exit 0 plus P1/P2 consumer compilation/run |

Test constraints:

- Do not add a testing framework or fixture system. Use standard assertions and the minimum SFML context required.
- No test owns a production object after moving its `std::unique_ptr`; no test treats a typed P2 view as an owner.
- A render screenshot proves appearance only; it does not replace the runtime assertions. A passing headless test does not set `VISUALLY_VERIFIED=YES`.
- The existing P2 27-check suite must remain green after any P3 contract change. It is regression evidence, not a substitute for the P3 task checks.
- Any legacy SFML 2-style test syntax must be updated only in the task-owned test file or through the cross-owner request process; do not broaden a P3 task into unrelated legacy cleanup.

## 10. Integration checks

### 10.1 Required cross-owner gates

| Integration check | Provider → consumer | Pass condition | Owning evidence task(s) |
|---|---|---|---|
| Production selected-player lifetime | P3 factory → owner approved by `DEC-PLAYER-OWNER` (P1 recommended) → P2 borrowed view | Approved owner retains one/two players for the entire active level; P2 updates them without deletion/dangling during reload/reset | `P3-FACTORY-001`, `P3-PLAYER-RESET-001` plus P1/P2 task IDs |
| Player graphics | P4 Animation API → P3 players → P1/P2 render | both selected characters and reachable forms render at correct hitbox scale in the production GameState | `P3-PLAYER-VISUAL-001` |
| Player state and safe growth | P2 clearance/contact → P3 state | growth preserves feet and is denied/deferred under a low ceiling; damage becomes lethal only at the correct state | `P3-PLAYER-STATE-001` |
| Representative enemy contact | P2 classifier → P3 Goomba → P1/P4 outcome | one stomp bounces/scores/removes once; side contact damages once | `P3-GOOMBA-001` |
| Full enemy contact variants | P2 classifier → P3 Koopa/FlyingKoopa/Heriss | shell, wing loss, non-stompable contact, score, and cleanup follow explicit results | `P3-KOOPA-001`, `P3-AIR-HERISS-001` |
| Boss/fire attack handoff | P3 value/move-only request → P2 world owner → target/outcome | attacks materialize after traversal with one owner, collide, expire, and never retain raw target owners | `P3-BOSS-001`, `P3-FIRE-001`, `CON-P2-P3-PROJECTILE-SPAWN` |
| Item spawn/collection | P4 block → P2 owner/contact → P3 effect → P1/P4 data | raw block callback is immediately adopted; Coin/Mushroom effect and HUD/session update occur once | `P3-ITEM-001` |
| Fire projectile handoff | P3 request → P2 owner/contact/removal | one allowed input yields one adopted projectile/request and safe cleanup; no retained player pointer | `P3-FIRE-001` |
| Enemy/item removal | P3 lifecycle → P2 cleanup | dead/inactive P2-owned object is excluded as required in-frame, erased only after traversal, typed views rebuilt | all enemy/item tasks; `CON-P2-P3-ENEMY-REMOVAL` |

### 10.2 First vertical-slice integration sequence

1. The owner approved by `DEC-PLAYER-OWNER` (P1 recommended) owns a factory-created Mario or Luigi; P1 passes a borrowed pointer to a successfully loaded P2 level.
2. P3 player art is initialized before the first render; P2 moves/grounds the player and P1 camera follows through its own contract.
3. P2 classifies one Goomba contact; P3 returns exactly one stomp or harm outcome; P1/P4 apply and display score/death data.
4. P4 triggers one representative Coin/Mushroom block; P2 immediately adopts the P3 payload, applies world/contact ordering, and P3 returns one item result.
5. P2 cleanup removes inactive/dead owned objects after traversal and leaves all borrowed player pointers valid under the approved owner lifetime.
6. P1 completes, pauses, dies/restarts, or exits only after P2 update returns. P3 reset occurs only at that safe boundary.

### 10.3 Nine-level contribution

P3 does not create nine variants of entity code. For each board validation entry `LV-W1-LV1`, `LV-W1-LV2`, `LV-W1-LV3`, `LV-W2-LV1`, `LV-W2-LV2`, `LV-W2-LV3`, `LV-W3-LV1`, `LV-W3-LV2`, and `LV-W3-LV3`, P3 supplies evidence that every encountered P3 type constructs explicitly, is visible, produces its defined outcomes, and reaches a valid lifecycle end. Missing/invalid factory names fail level readiness rather than becoming a different entity. Map-data edits remain user-owned and out of scope.

### 10.4 Cross-owner edit request workflow

When a P3 check proves that a P1/P2/P4 file must change:

1. Stop before editing the file; keep the P3 task status at `BLOCKED`, `IN_PROGRESS`, or `TESTING` as factually appropriate.
2. Add a cross-owner request to `01_MASTER_TASK_BOARD.md` naming requester task, receiving owner/task, exact file/symbol, minimal API/behavior delta, reproducer, contract ID, and required-by date/wave.
3. The receiving owner accepts/rejects/refines it and owns the production change plus their test.
4. P3 rebases/merges the provider change, runs its focused check and the P2 27-check regression suite, then records integration evidence.
5. Close the request only when both provider and consumer checks pass. Do not mark the P3 task `DONE` while `INTEGRATION_DONE=NO`.

### 10.5 Merge and regression checklist

- [ ] No concurrent edit to `PlayerManager.*`; state → reset → item → fire order is preserved.
- [ ] Factory signature change is coordinated with every P1/P2 caller in one integration window.
- [ ] Animation keys used by P3 are registered before first play; missing assets/keys do not flood logs.
- [ ] P2 unique ownership and non-owning typed views remain intact; no `new`/`delete` owner bridge is introduced.
- [ ] P2's 27 runtime checks still pass, including Rocket target lifetime, inactive cleanup, same-frame inactive Block exclusion, normal Brick, and BrickFragment lifecycle.
- [ ] Direct production syntax passes C++20/SFML 3.1.0; no SFML 2 API is introduced.
- [ ] 1P and 2P contact/reset/input checks pass under the approved decisions.
- [ ] Visual captures show hitbox/art alignment at `CELL_SIZE=64` and no invisible required entity/item.
- [ ] No Group5 chatbot/LLM/Ollama/ChatUI source or architecture entered Group4.

## 11. Final Person 3 Definition of Done

Person 3 is complete only when all statements below are true and linked from the master board:

- All 11 P3 tasks are `DONE`; every task independently records `SOURCE_DONE=YES`, `INTEGRATION_DONE=YES`, `RUNTIME_TESTED=YES`, `VISUALLY_VERIFIED=YES`, and `GAMEPLAY_VERIFIED=YES` where required by its card. A partially implemented class or a compile-only check is not done.
- All 20 existing P3-owned production header/source pairs remain accounted for, and any newly approved P3 production file is added to the inventory and exactly one task before implementation.
- Mario and Luigi are factory-constructible, visibly distinct, correctly animated/scaled, safe through small/big/fire/damage/reset transitions, and valid under 1P/2P ownership/lifetime rules.
- Goomba, Koopa, FlyingKoopa, Heriss, Bowser, and PeteyPiranha have deterministic ordinary behavior, complete contact/state/visual contracts, once-only score/death outcomes, and safe P2-owned cleanup. Boss attacks have real integrated world effects.
- Coin, Mushroom, PowerUpObject, PlayerBuff, and FireBuff produce one approved effect, render visibly, and integrate with P2 world ownership and P1/P4 session/HUD surfaces without direct cross-owner mutation.
- Factory errors are explicit; unknown values never silently construct another gameplay type; successful returns use `std::unique_ptr` with the P1/P2 receiver becoming the sole owner.
- P3 never owns P1 state/session, P2 levels/runtime entities after transfer, P4 animation/HUD/blocks, or borrowed player views. No retained raw pointer can outlive its owner.
- All required human decisions cited by P3 tasks are approved outside this plan and reflected through the canonical contracts; no implementation silently resolved an `OPEN` decision.
- Each focused P3 check passes, all current P2 runtime regression checks pass, production syntax/build evidence targets C++20/SFML 3.1.0, and the full application has no P3-origin crash, dangling pointer, double effect, silent fallback, or per-frame error flood.
- The first playable vertical slice and all nine distinct level validation entries pass the P3 portions of construction, rendering, interaction, outcome, and lifecycle verification. P3 engine code is shared; no level-specific duplication or map-image inspection/edit is introduced.
- The whole-game flow can start, select/own visible players, load a level, move/collide, interact with a representative enemy and block payload, update score/coins/lives/HUD, die/restart/Game Over, complete/advance/final-complete, persist through the approved P1/P4 path, and exit cleanly—with P3 supplying only its owned behavior/contracts.
- Group5 remains reference-only. Its special LLM/Ollama/chatbot system is `EXCLUDED` and contributes no task, source, test, or Definition-of-Done requirement.

Final P3 completion authority: Person 3 may propose `DONE`; the integration owner for each consumed contract confirms `INTEGRATION_DONE`, the designated tester confirms runtime/gameplay evidence, and the visual reviewer confirms captures. Any failed dimension returns only the affected task to `READY`/`BLOCKED`/`TESTING`; verified unrelated baselines stay closed.

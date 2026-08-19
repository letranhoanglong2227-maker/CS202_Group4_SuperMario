# Person 4 Execution Plan

Planning authority: `TEAM_CURRENT_STATE_AUDIT.md` (2026-08-19). This file controls future execution only; it does not authorize implementation. The whole-project verdict remains `PLAN_SUFFICIENT_BUT_INTEGRATION_CONTRACTS_ARE_MISSING`.

## 1. Component checklist and status summary

The three `BASE-*` cards are DONE and frozen at their audited contract scope. Do not reopen them because adjacent visual, payload, or full-game verification remains unfinished; use the remaining task that owns that missing dimension.

| P4 production component / file | Disposition | Task(s) | Status | SOURCE_DONE | INTEGRATION_DONE | RUNTIME_TESTED | VISUALLY_VERIFIED | GAMEPLAY_VERIFIED |
|---|---|---|---|---|---|---|---|---|
| `include/Components/AnimationComponent.hpp`, `src/Components/AnimationComponent.cpp` | TASK | `P4-ANIMATION-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Core/Constants.hpp` | VERIFIED_DONE / frozen | `BASE-P4-BLOCK-INTEGRATION-001` | DONE | [x] | [x] | [x] | n/a | n/a |
| `include/Core/UserData.hpp`, `src/Core/UserData.cpp` | TASK; preserve current in-memory behavior | `P4-PERSISTENCE-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Objects/Blocks/Block.hpp`, `src/Objects/Blocks/Block.cpp` — visual-size scope | TASK | `P4-BLOCK-SIZE-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| same Block pair — public lifecycle scope | VERIFIED_DONE / frozen | `BASE-P4-BLOCK-INTEGRATION-001` | DONE | [x] | [x] | [x] | n/a | n/a |
| `include/Objects/Blocks/SolidBlock.hpp`, `src/Objects/Blocks/SolidBlock.cpp` | TASK | `P4-BLOCK-SIZE-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Objects/Blocks/Brick.hpp`, `src/Objects/Blocks/Brick.cpp` — normal Brick path | VERIFIED_DONE / frozen | `BASE-P4-BRICK-001` | DONE | [x] | [x] | [x] | [ ] | [ ] |
| same Brick pair — special/payload behavior only | TASK | `P4-PAYLOAD-BLOCK-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Objects/Blocks/BrickFragment.hpp`, `src/Objects/Blocks/BrickFragment.cpp` | VERIFIED_DONE / frozen | `BASE-P4-FRAGMENT-001` | DONE | [x] | [x] | [x] | [ ] | [ ] |
| `include/Objects/Blocks/CoinBlock.hpp`, `src/Objects/Blocks/CoinBlock.cpp` — base behavior validation | TASK | `P4-QUESTION-BLOCK-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| same CoinBlock pair — corrected popped-Coin scope | VERIFIED_DONE / frozen | `BASE-P4-BLOCK-INTEGRATION-001` | DONE | [x] | [x] | [ ] | [ ] | [ ] |
| `include/Objects/Blocks/MushroomBlock.hpp`, `src/Objects/Blocks/MushroomBlock.cpp` — base behavior validation | TASK | `P4-QUESTION-BLOCK-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| same MushroomBlock pair — special variant scope | TASK | `P4-PAYLOAD-BLOCK-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| same MushroomBlock pair — corrected one-tile spawn scope | VERIFIED_DONE / frozen | `BASE-P4-BLOCK-INTEGRATION-001` | DONE | [x] | [x] | [ ] | [ ] | [ ] |
| `include/Objects/Blocks/CloudBlock.hpp`, `src/Objects/Blocks/CloudBlock.cpp` | TASK | `P4-CLOUD-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/Audio/AudioSystem.hpp`, `src/Audio/AudioSystem.cpp` | TASK | `P4-AUDIO-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/UI/GUI.hpp`, `src/UI/GUI.cpp` | TASK | `P4-GUI-001` | READY | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/UI/LevelGUI.hpp`, `src/UI/LevelGUI.cpp` | TASK | `P4-HUD-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/States/Menus/LeaderboardState.hpp`, `src/States/Menus/LeaderboardState.cpp` | TASK; P4-owned despite state directory | `P4-LEADERBOARD-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `CMakeLists.txt`, runtime asset-copy/install rules, `assets/**`, `SaveData/`, expected `LeaderBoard.csv` | TASK TARGETS only; do not alter content without the card's gate | `P4-PACKAGE-001`; data/schema portions also `P4-PERSISTENCE-001`, `P4-LEADERBOARD-001` | BLOCKED | [ ] | [ ] | [ ] | [ ] | [ ] |
| `include/src/Objects/Blocks/MovingBlock.*` | EXCLUDED from P4; P2-owned | P2 task | EXCLUDED | n/a | n/a | n/a | n/a | n/a |

Remaining P4 tasks: 11 total; 5 READY; 6 BLOCKED; 3 P0; 6 P1; 2 P2; 0 P3. Frozen baseline cards: 3 DONE. No current P4 task is IN_PROGRESS or TESTING.

## 2. Exact current baseline

- `AnimationComponent` binds borrowed sprite/texture references, registers keyed frame vectors, switches immediately to the first frame, and loops on a single interval. It logs duplicate/missing keys, swallows duplicate registration errors, drops surplus `dt`, and has no focused test or stable P3-facing failure contract.
- `Block::isExist()` is public and is consumed successfully by P2. `Core/Constants.hpp` contains the corrected shared collision-side values. These are frozen facts, not new tasks.
- P2 currently calls virtual `GameObject::setSize({32,32})` for one-tile blocks. `Block::setSizeBlock` can scale a sprite after a non-empty texture rectangle exists, but it is not the method P2 calls. Current 16x16 block frames therefore do not have a proven invariant that visual size equals the 32x32 physics tile.
- Normal Brick bottom-hit behavior is current and tested: one eligible collision call from below makes it inactive once, spawns four real `BrickFragment` objects, and P2 immediately adopts/removes them through its `std::unique_ptr` runtime. Do not reopen or rewrite that path without new evidence.
- `BrickFragment::setPosition` now updates sprite/position through the common API; four fragments animate/move and mark themselves inactive after falling past the current fixed cutoff. Its implementation and P2 handoff/lifecycle are frozen at the tested scope.
- `CoinBlock` spawns `Coin(position - {0,32}, true)`, decrements a positive count, bounces, and switches to Empty. `MushroomBlock` spawns `Mushroom(position - {0,32}, true)` once and bounces. Preserve those corrected popped/offset facts. The CoinBlock interactive test still uses SFML 2 syntax and is not valid SFML 3.1 runtime evidence; MushroomBlock syntax passes but was not run interactively.
- P2's parser exposes variants for coin Brick and Mushroom/1-Up/Star block markers, but P2 currently constructs generic `Brick` or `MushroomBlock` without forwarding the variant. Special/payload Brick and variant behavior remain P4 implementation, with P2 mapping and P3 payload dependencies.
- `CloudBlock`, `AudioSystem`, `GUI`, `LevelGUI`, and `LeaderboardState` are empty shells.
- `UserData` currently stores name, lives, score, coins, current/unlocked world/level, and character in memory. It supports score/coins/lives mutations, a 100-coin life conversion, nonnegative lives, session reset, and monotonic unlock updates. It has no validation boundary, serialization, load/save error surface, or persisted file.
- Audio and font assets exist; `SaveData/.gitkeep` exists; expected `LeaderBoard.csv` is missing. `TextureBlockManager` currently loads `../assets/textures/Items_Blocks.png`, while P2 stages use another assumed asset path. `DEC-ASSET-ROOT` is OPEN.
- `CMakeLists.txt` requires C++20 but FetchContent pins SFML 3.0.0. Direct audit syntax against installed SFML 3.1 passed for all production translation units. The full build stops earlier in FreeType `windres` because workspace paths contain spaces.
- No map PNG was opened, parsed, scanned, or edited for this plan. Map-data work remains user-owned. Exactly nine identities, `W1_LV1` through `W3_LV3`, remain the team requirement.
- Group5 is reference-only. Its LLM/Ollama/chatbot feature is excluded, and its raw-pointer architecture must not be copied.

## 3. Remaining tasks in dependency order

| Order | Card | Status | Priority | Start/finish gate |
|---:|---|---|---|---|
| 0A | `BASE-P4-BRICK-001` | DONE | P0 | Frozen normal-Brick scope; downstream tasks do not rewrite it |
| 0B | `BASE-P4-FRAGMENT-001`, `BASE-P4-BLOCK-INTEGRATION-001` | DONE | P1 | Frozen fragment/correction scopes; downstream tasks add only missing behavior/evidence |
| 1 | `P4-ANIMATION-001` entity-facing animation contract | READY | P0 | Can start; finish with P3 consumer evidence |
| 2 | `P4-BLOCK-SIZE-001` 32x32 block visual invariant | READY | P0 | Can start against existing P2 size call; finish with runtime render evidence |
| 3 | `P4-GUI-001` reusable GUI controls | READY | P0 | Minimum production selection/death/win controls block the first slice; no state logic ownership transfer |
| 4 | `P4-QUESTION-BLOCK-001` Coin/Mushroom block validation | READY | P1 | Preserve spawn facts; finish with corrected SFML 3.1 checks and P2 adoption |
| 5 | `P4-AUDIO-001` AudioSystem | READY | P1 | Core can start; final paths consume OPEN `DEC-ASSET-ROOT` and P1 events |
| 6 | `P4-PAYLOAD-BLOCK-001` special Brick and block variants | BLOCKED | P1 | `DEC-BLOCK-ACTOR-ELIGIBILITY`, P2 variant forwarding, P3 payload APIs |
| 7 | `P4-CLOUD-001` CloudBlock behavior | BLOCKED | P1 | OPEN `DEC-CLOUD-SEMANTICS`; P2 currently maps marker to MovingBlock |
| 8 | `P4-HUD-001` gameplay HUD | BLOCKED | P1 | P1 event/data contract and closed multiplayer lives semantics |
| 9 | `P4-PERSISTENCE-001` UserData save/load/progression | BLOCKED | P1 | OPEN `DEC-PERSISTENCE`, `DEC-RESPAWN`, and profile implications of multiplayer policy |
| 10 | `P4-LEADERBOARD-001` leaderboard CSV/state | BLOCKED | P2 | Persistence schema, P1 State/GUI integration |
| 11 | `P4-PACKAGE-001` SFML 3.1 build/release/assets | BLOCKED | P2 | OPEN `DEC-ASSET-ROOT`; production flow and all required runtime tasks |

A recommended default in a decision card is not authorization. Change a BLOCKED card to READY only after its decision/provider evidence is recorded in `01_MASTER_TASK_BOARD.md` and reflected here.

## 4. Files allowed to edit

P4 execution is limited to these production surfaces, and only under the card naming them:

- `SUPERMARIO/CS202_Group4_SuperMario/include/Components/AnimationComponent.hpp`, `src/Components/AnimationComponent.cpp`
- `include/Core/Constants.hpp` only if a new evidence-backed task explicitly reopens the frozen constants baseline; otherwise no change
- `include/Core/UserData.hpp`, `src/Core/UserData.cpp`
- `include/Objects/Blocks/Block.hpp`, `src/Objects/Blocks/Block.cpp`
- P4 block pairs: `SolidBlock`, `Brick`, `BrickFragment`, `CoinBlock`, `MushroomBlock`, and `CloudBlock`
- `include/Audio/AudioSystem.hpp`, `src/Audio/AudioSystem.cpp`
- `include/UI/GUI.hpp`, `src/UI/GUI.cpp`, `include/UI/LevelGUI.hpp`, `src/UI/LevelGUI.cpp`
- `include/States/Menus/LeaderboardState.hpp`, `src/States/Menus/LeaderboardState.cpp`
- planned P4 checks under `tests/Person4AnimationContracts.cpp`, `tests/Person4BlockContracts.cpp`, and `tests/Person4DataContracts.cpp`; modernizing `tests/TestCoinBlock.cpp` is allowed only inside `P4-QUESTION-BLOCK-001`
- `CMakeLists.txt`, release/install rules, runtime copies under build/package output, `SaveData/`, and `LeaderBoard.csv` only within the exact persistence/leaderboard/package cards after their gates close
- asset files only when an approved P4 task explicitly identifies a missing/corrected P4-owned asset; asset-root/layout work must not modify map PNG content

Frozen normal-Brick/fragment/constants/lifecycle lines are not an invitation for cleanup. If a remaining task must touch the same file, preserve those behaviors and rerun their regression checks.

Any needed edit outside this list uses the cross-owner request in `04_USAGE_AND_PROGRESS_WORKFLOW.md`: consuming task, exact file/symbol, requested contract delta, reason, owner, and test impact. Wait for the owner to land it.

## 5. Files read-only/reference

- P2-owned: `include/src/Objects/Blocks/MovingBlock.*`, all `Levels/Managers/**`, `Levels/Stages/**`, `Physics/**`, `MovementComponent.*`, and environment objects.
- P3-owned: `include/src/Core/GameObject.*`, all `Entities/**`, and all `Objects/Items/**`, including Coin, Mushroom, PowerUpObject, PlayerBuff, and FireBuff.
- P1-owned: `src/main.cpp`, `Core/MyApp.*`, `Core/GameEventMediator.*`, `States/Base/**`, and every menu except P4's `LeaderboardState`.
- Shared evidence/reference: `TEAM_CURRENT_STATE_AUDIT.md`, the execution-control documents, `PLAN.md`, and Group5 only where a card says a reference is useful.
- Map images: strictly out of inspection/edit scope for these tasks. P4 may verify copied file presence/hash in packaging only after `DEC-ASSET-ROOT`; it must not inspect pixels or take over user map edits.

Never implement the excluded Group5 chatbot/LLM feature. Runtime adoption/test authorship does not transfer P4's BrickFragment implementation to P2, and directory placement does not transfer LeaderboardState to P1.

## 6. Inputs expected from other persons

| Input | Provider | Consuming P4 card(s) | Acceptance evidence |
|---|---|---|---|
| Animation keys, frame sets, action changes, sprite/texture lifetime | P3 | `P4-ANIMATION-001` | `CON-P3-P4-ANIMATION`; one player and enemy animate visibly without missing-key spam |
| Collision side and actor/power eligibility context | P2 + P3, human policy | `P4-QUESTION-BLOCK-001`, `P4-PAYLOAD-BLOCK-001`, `P4-CLOUD-001` | `CON-P2-P4-BLOCK-COLLISION`; CLOSED `DEC-BLOCK-ACTOR-ELIGIBILITY` |
| P2's 32x32 physics size call and block construction timing | P2 | `P4-BLOCK-SIZE-001` | `CON-P2-P4-BLOCK-VISUAL-SIZE`; render/physics rectangle equality check |
| Immediate raw callback adoption into `std::unique_ptr`, inactive cleanup, no same-frame recollision | P2 | question/payload/fragment regression | `CON-P2-P4-SPAWN-HANDOFF`, `CON-P2-P4-BLOCK-LIFECYCLE`; 27-check suite |
| Forwarded map variant value without map-pixel work in P4 | P2 | `P4-PAYLOAD-BLOCK-001` | `CON-P2-P4-BLOCK-VARIANTS`; constructor table check |
| Coin/Mushroom/1-Up/Star payload construction and behavior | P3 | `P4-PAYLOAD-BLOCK-001`, `P4-QUESTION-BLOCK-001` | spawn type/position/ownership checks; no P4 item implementation |
| State commands, text-entry semantics, selected/focused control | P1 | `P4-GUI-001`, `P4-LEADERBOARD-001` | `CON-P1-P4-GUI`; P1 menus consume P4 controls without P4 transitions |
| Typed score/coin/lives/death/completion/audio events and safe subscription lifetime | P1 | `P4-AUDIO-001`, `P4-HUD-001`, `P4-PERSISTENCE-001` | P1/P4 canonical contract checks; exactly-once updates |
| Active profile/session and approved save/progression timing | P1 + human decision | persistence/leaderboard | `DEC-PERSISTENCE`, `DEC-MULTIPLAYER-DEATH`, and `DEC-RESPAWN` are `RESOLVED`; P1 flow checks. HUD instead consumes the current P1 snapshot and is not gated on durable persistence. |
| Runtime asset-root convention and P2 stage loading compliance | human decision + P2 | audio, blocks, GUI, package | CLOSED `DEC-ASSET-ROOT`; run from build and packaged directories |

## 7. Outputs and contracts provided

| P4 output | Consumers | Guarantee |
|---|---|---|
| Deterministic animation component contract | P3 entities, P4 blocks | Borrowed sprite/texture lifetimes are explicit; switching applies frame zero; invalid input has a testable policy |
| Block visual-size synchronization | P2 runtime/camera, players | 16x16 source frames render at the exact P2-supplied world size, normally 32x32, without changing physics size |
| Block reaction/lifecycle and payload trigger behavior | P2 collision/runtime | Frozen normal Brick remains once-only; active/empty/inactive state and callback timing are explicit |
| Spawned payload object at agreed position/state | P2 owner, P3 behavior | P4 creates through P3 API and immediately hands off through P2 callback; P4 retains no owning raw pointer |
| Variant-aware P4 constructor/behavior surface | P2 mapping | Unknown variant fails safely; no pixel decoding in P4 |
| Audio cue/music API | P1 mediator/states | Buffers outlive sounds; missing files report failure without crashing; no gameplay policy inside AudioSystem |
| Reusable GUI controls | P1 states | `CON-P1-P4-GUI`: P4 reports value intent and renders; P1 owns navigation/state changes |
| Screen-space LevelGUI | P1 GameState | `CON-P1-P4-HUD-DATA`: P4 renders a value snapshot; P1 owns the gameplay view/order |
| Validated UserData persistence/progression and leaderboard data | P1 flow/UI | Parsing validates bounds, writes are loss-resistant, and exactly nine level identities are representable |
| SFML 3.1 release/build and coherent runtime layout | whole team | One approved asset root works from the packaged executable; no hidden dependency on repository CWD |

These outputs implement the canonical `CON-*` cards named below. P4 must not unilaterally edit their provider/consumer, ownership, direction, timing, or failure semantics.

## 8. Per-task implementation notes

### BASE-P4-BRICK-001 — Normal Brick break behavior

- **Task ID:** `BASE-P4-BRICK-001`
- **Title:** Verified normal Brick bottom-hit break behavior
- **Owner:** Person 4
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** DONE
- **Purpose:** Freeze the known-good normal Brick contract so payload work cannot regress it.
- **Exact current behavior:** one `COLLISION_BOTTOM` call while unbroken marks the Brick broken/inactive/transparent and invokes the spawn callback for exactly four real `BrickFragment` objects; repeated calls do not respawn.
- **Missing behavior checklist:** [x] shared bottom-side constant; [x] once-only inactive transition; [x] four fragments; [x] no repeated spawn. Special/payload Brick is intentionally excluded.
- **Exact target files:** `include/Objects/Blocks/Brick.hpp`; `src/Objects/Blocks/Brick.cpp`.
- **Allowed edit files:** none under this frozen card; overlapping payload edits must preserve and re-test this behavior.
- **Read-only dependency files:** P2 LevelManager/Physics and P4 BrickFragment/current constants.
- **Do not implement:** payload/actor eligibility, visual-size work, fragment polish, or any cleanup disguised as baseline work.
- **Dependencies:** satisfied by current source and audited runtime suite.
- **Contracts provided:** normal-Brick portion of `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-LIFECYCLE`, `CON-P2-P4-SPAWN-HANDOFF`.
- **Contracts consumed:** P2 bottom-collision call and immediate callback adoption.
- **Group5 reference if useful:** not needed.
- **Do-not-copy warning:** do not replace this with Group5 raw-pointer ownership or architecture.
- **Implementation notes:** frozen; reopen only with new reproducible evidence and a new task/change record.
- **SFML 3.1 notes:** current production syntax passed against 3.1.
- **Compile checks:** current Brick/Fragment syntax passed at audit time.
- **Runtime checks:** P2's 27-check executable passed the normal Brick once-only spawn/adoption/removal contract.
- **Visual checks:** not established by the audit; downstream block-size/question/payload visual checks own this dimension.
- **Gameplay checks:** not established in production GameState; downstream vertical slice owns it.
- **Definition of done:** already DONE at contract scope; regression must remain green after every overlapping task.
- **Suggested commit message:** none — frozen audited baseline; do not recommit.
- **Completion dimensions:** SOURCE_DONE [x]; INTEGRATION_DONE [x]; RUNTIME_TESTED [x]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### BASE-P4-FRAGMENT-001 — BrickFragment lifecycle integration

- **Task ID:** `BASE-P4-FRAGMENT-001`
- **Title:** Verified BrickFragment implementation and P2 lifecycle handoff
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** DONE
- **Purpose:** Preserve P4 implementation ownership and the tested P2 adoption/removal boundary.
- **Exact current behavior:** each fragment starts at the Brick position through `setPosition`, has an initial velocity/gravity, plays a two-frame rotation, and marks itself inactive below its current cutoff; P2 adopts callback pointers immediately and removes inactive owners.
- **Missing behavior checklist:** [x] real fragment type; [x] position/sprite sync; [x] update/animation; [x] inactive lifecycle; [x] P2 adoption/removal. World-derived cutoff polish is not part of this frozen card.
- **Exact target files:** `include/Objects/Blocks/BrickFragment.hpp`; `src/Objects/Blocks/BrickFragment.cpp`.
- **Allowed edit files:** none under the baseline; any future cutoff change requires a separate evidence-backed task and P2 extent contract.
- **Read-only dependency files:** P2 LevelManager; P4 Block/Animation; P3 GameObject.
- **Do not implement:** P2 ownership/cleanup, new fragment physics engine behavior, payload blocks, or visual tuning.
- **Dependencies:** satisfied.
- **Contracts provided:** fragment portion of `CON-P2-P4-BLOCK-LIFECYCLE` and `CON-P2-P4-SPAWN-HANDOFF`.
- **Contracts consumed:** immediate P2 unique ownership after callback.
- **Group5 reference if useful:** none.
- **Do-not-copy warning:** do not move implementation ownership to P2 or reintroduce retained owning raw pointers.
- **Implementation notes:** frozen; implementation ownership and runtime ownership remain distinct.
- **SFML 3.1 notes:** current source syntax passed.
- **Compile checks:** current fragment translation unit passed direct audit syntax.
- **Runtime checks:** spawn, adoption, expiry, and removal passed in the 27-check suite.
- **Visual checks:** no audited interactive evidence; later block visual regression may verify it without reopening this card.
- **Gameplay checks:** no production GameState evidence; vertical slice owns it.
- **Definition of done:** already DONE at tested lifecycle scope and remains a required regression.
- **Suggested commit message:** none — frozen audited baseline; do not recommit.
- **Completion dimensions:** SOURCE_DONE [x]; INTEGRATION_DONE [x]; RUNTIME_TESTED [x]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### BASE-P4-BLOCK-INTEGRATION-001 — Shared block integration corrections

- **Task ID:** `BASE-P4-BLOCK-INTEGRATION-001`
- **Title:** Verified collision constants, public lifecycle, and corrected payload spawn facts
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN` and `IMPLIED_BY_CURRENT_GROUP4_DESIGN`
- **Current status:** DONE
- **Purpose:** Prevent stale findings from reopening corrected shared block boundaries.
- **Exact current behavior:** collision sides come from `Core/Constants.hpp`; `Block::isExist()` is public; CoinBlock constructs `Coin(..., true)` one tile above; MushroomBlock constructs `Mushroom(..., true)` one tile above.
- **Missing behavior checklist:** [x] one shared collision definition; [x] public lifecycle query; [x] popped Coin state; [x] 32-pixel vertical spawn offset. Full payload collection/variant/visual verification remains in other cards.
- **Exact target files:** `include/Core/Constants.hpp`; `include/Objects/Blocks/Block.hpp`; CoinBlock and MushroomBlock source.
- **Allowed edit files:** none under this card; overlapping tasks preserve these exact facts.
- **Read-only dependency files:** P2 Physics/LevelManager; P3 Coin/Mushroom APIs.
- **Do not implement:** P2 adoption, P3 item behavior, variants, actor policy, or 32x32 sprite scaling.
- **Dependencies:** satisfied at audited source/integration boundary.
- **Contracts provided:** baseline portions of `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-LIFECYCLE`, `CON-P2-P4-SPAWN-HANDOFF`.
- **Contracts consumed:** P2 collision/adoption; P3 popped item constructors.
- **Group5 reference if useful:** not needed.
- **Do-not-copy warning:** no duplicate constants or Group5 raw-pointer lifetime.
- **Implementation notes:** frozen corrections; legacy TestCoinBlock syntax failure is test modernization, not evidence these source facts regressed.
- **SFML 3.1 notes:** production source passed; TestCoinBlock remains SFML 2-style.
- **Compile checks:** production translation units passed direct 3.1 syntax.
- **Runtime checks:** normal Brick lifecycle is covered; full Coin/Mushroom runtime remains `P4-QUESTION-BLOCK-001`.
- **Visual checks:** not established; remaining cards own visual proof.
- **Gameplay checks:** not established end-to-end; P2/P3/P1 vertical slice owns it.
- **Definition of done:** already DONE at source/contract fact scope; all four facts remain unchanged after future work.
- **Suggested commit message:** none — frozen audited baseline; do not recommit.
- **Completion dimensions:** SOURCE_DONE [x]; INTEGRATION_DONE [x]; RUNTIME_TESTED [ ] (full combined scope unverified); VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-ANIMATION-001 — Entity-facing animation contract

- **Task ID:** `P4-ANIMATION-001`
- **Title:** Entity-facing AnimationComponent contract
- **Owner:** Person 4
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Make frame registration/playback predictable and safe for P3 player/enemy/item rendering and P4 blocks.
- **Exact current behavior:** borrowed sprite/texture references are bound; frame-zero applies on key change; timer advances one frame and resets to zero; duplicate/missing keys print messages; invalid interval/empty frames and large `dt` behavior are undefined or weak.
- **Missing behavior checklist:** [ ] document borrowed lifetime; [ ] explicit result/policy for duplicate, missing, and empty animations; [ ] validate positive interval; [ ] preserve surplus time and advance deterministically; [ ] switch resets to frame zero; [ ] avoid per-frame error spam; [ ] stable P3 action-key contract; [ ] focused runnable checks.
- **Exact target files:** `include/Components/AnimationComponent.hpp`; `src/Components/AnimationComponent.cpp`; planned `tests/Person4AnimationContracts.cpp`.
- **Allowed edit files:** exact targets only.
- **Read-only dependency files:** P3 entity/player/enemy/item files; P4 Block files as consumers.
- **Do not implement:** P3 sprite/texture initialization, action state selection, asset coordinates, or a generalized animation graph.
- **Dependencies:** none for source start; P3 consumer integration for final completion.
- **Contracts provided:** `CON-P3-P4-ANIMATION` API, input validation, timing, and borrowed lifetime.
- **Contracts consumed:** P3-owned animation keys/frame data and owner lifetime.
- **Group5 reference if useful:** frame timing/atlas coordinates may be reference-only; do not import architecture.
- **Do-not-copy warning:** no Group5 raw texture/sprite owners, chatbot code, or unrelated asset loader.
- **Implementation notes:** prefer a small result/boolean/query over exception-catching-and-logging internally. Preserve references as non-owning. Handle large `dt` without unbounded work; document any cap with a `ponytail:` ceiling comment if needed.
- **SFML 3.1 notes:** use SFML 3 `sf::IntRect`/sprite texture APIs and C++20; tests compile against installed 3.1.
- **Compile checks:** direct syntax check both component files and animation contract test with warnings enabled.
- **Runtime checks:** add/play/switch; duplicate/missing/empty keys; zero/negative interval; exact interval; multi-interval `dt`; texture/sprite still owned by caller.
- **Visual checks:** one P3 player and enemy visibly switch Idle/Move/Jump or equivalent without stale first frames or jitter.
- **Gameplay checks:** animation changes follow P3 action state and never alter physics/gameplay state.
- **Definition of done:** behavior is deterministic/tested, P3 consumes the documented surface, no log spam/ownership regression, and all dimensions are evidenced separately.
- **Suggested commit message:** `person4: stabilize animation playback contract`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-BLOCK-SIZE-001 — 32x32 block visual invariant

- **Task ID:** `P4-BLOCK-SIZE-001`
- **Title:** Synchronize Block visual size with 32x32 physics size
- **Owner:** Person 4
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN` and `IMPLIED_BY_CURRENT_GROUP4_DESIGN`
- **Current status:** READY
- **Purpose:** Ensure each block sprite occupies the P2-supplied world rectangle, normally one 32x32 tile, without changing collision ownership.
- **Exact current behavior:** block frames are usually 16x16; P2 calls virtual `GameObject::setSize`, which updates size/hitbox only; `Block::setSizeBlock` scales only when called after a valid texture rectangle and is currently unused by P2 construction.
- **Missing behavior checklist:** [ ] P2's existing size call reaches P4 visual sync; [ ] sprite scale derives from current nonzero frame rect; [ ] animation frame changes preserve world size; [ ] hitbox remains exactly P2-supplied; [ ] zero/empty rect safe; [ ] multi-tile SolidBlock policy explicit; [ ] no double scale; [ ] all P4 block types checked.
- **Exact target files:** `include/Objects/Blocks/Block.hpp`; `src/Objects/Blocks/Block.cpp`; P4 block files only if their frame initialization order requires a minimal fix; planned block test.
- **Allowed edit files:** P4 block targets only; no P2 call-site edit.
- **Read-only dependency files:** P3 `GameObject.*`; P2 `LevelManager.cpp`, Physics, map format; `Constants.hpp` frozen.
- **Do not implement:** P2 map dimensions/constructor changes, physics resizing, texture coordinates, camera, or map-specific exceptions.
- **Dependencies:** existing P2 32x32 construction call; `CON-P2-P4-BLOCK-VISUAL-SIZE`.
- **Contracts provided:** P4 side of `CON-P2-P4-BLOCK-VISUAL-SIZE`.
- **Contracts consumed:** P2 supplies authoritative world size after construction; P4 animation supplies active frame rect.
- **Group5 reference if useful:** visual tile size may be compared only; Group4 `TILE_SIZE=32` is authoritative.
- **Do-not-copy warning:** no fixed Group5 dimensions beyond Group4 contract, no P2 edits, no raw texture ownership.
- **Implementation notes:** reuse virtual `setSize` rather than adding a second required call if the existing override can cover the seam. Keep one source of truth: physics/world size; visual scale follows it.
- **SFML 3.1 notes:** use `sf::Vector2f` scale/texture-rect APIs as in SFML 3.1; verify negative/zero rect dimensions defensively.
- **Compile checks:** syntax check Block plus every derived P4 block and block contract test.
- **Runtime checks:** 16->32 scale; repeated size call; frame switch; zero rect then initialization; 32x64 fixture; hitbox and rendered bounds equal expected world dimensions.
- **Visual checks:** SolidBlock, Brick, CoinBlock, MushroomBlock appear exactly tile-sized with no gap/half-size/double-size.
- **Gameplay checks:** collision contact matches visible faces in representative block interaction; normal Brick baseline remains green.
- **Definition of done:** P2 needs no special P4 call, visual/physics bounds agree, all P4 block types and regressions pass, and five dimensions have evidence.
- **Suggested commit message:** `person4: keep block sprites aligned with world size`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-QUESTION-BLOCK-001 — CoinBlock and MushroomBlock validation

- **Task ID:** `P4-QUESTION-BLOCK-001`
- **Title:** Complete and validate base CoinBlock and MushroomBlock behavior
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Turn current source behavior into SFML 3.1-tested, once-only/finite payload block contracts without reopening corrected spawn facts.
- **Exact current behavior:** CoinBlock validates positive count, spawns a popped Coin one tile above, decrements/counts down, bounces, then renders Empty; MushroomBlock spawns a popped Mushroom one tile above once, bounces, and renders Empty. `maxBounceHeight` is unused; original Y is lazily captured; no valid end-to-end test exists.
- **Missing behavior checklist:** [ ] SFML 3.1 test modernization; [ ] original-position capture robust to update/set order; [ ] finite bounce returns exactly to origin; [ ] repeat hit blocked during bounce; [ ] empty state never respawns; [ ] multi-coin count exactly N; [ ] null callback safe; [ ] P2 immediate adoption verified; [ ] P3 popped payload state verified; [ ] active/empty animation and 32x32 visual proof.
- **Exact target files:** CoinBlock and MushroomBlock header/source; `tests/TestCoinBlock.cpp`; `tests/TestMushroomBlock.cpp` only as needed; planned `tests/Person4BlockContracts.cpp`.
- **Allowed edit files:** exact P4 block/test targets; preserve baseline facts.
- **Read-only dependency files:** P2 LevelManager/Physics; P3 Coin/Mushroom; P4 Block/Animation under their own tasks.
- **Do not implement:** payload collection/effects, variant 1-Up/Star behavior, actor eligibility policy, P2 callback adoption, or P3 item physics.
- **Dependencies:** frozen block baseline; block-size/animation for visual finish; existing P2/P3 APIs.
- **Contracts provided:** base question-block portions of `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-LIFECYCLE`, `CON-P2-P4-SPAWN-HANDOFF`.
- **Contracts consumed:** P2 bottom-collision/adoption; P3 popped Coin/Mushroom constructors.
- **Group5 reference if useful:** bounce feel/frames only, not architecture or ownership.
- **Do-not-copy warning:** do not copy Group5 raw payload ownership, map parsing, hard-coded paths, or chatbot code.
- **Implementation notes:** first preserve `Coin(..., true)` and `Mushroom(..., true)` at Y-32. Prefer one small shared behavior only if existing code can reuse it without a new abstraction; otherwise keep the minimal local fixes.
- **SFML 3.1 notes:** update legacy TestCoinBlock to `sf::VideoMode({})`, optional event polling, event type queries, and SFML 3 rectangle/view APIs.
- **Compile checks:** syntax check both block translations and all touched tests against SFML 3.1.
- **Runtime checks:** N=1 and N>1 CoinBlock; Mushroom once; repeat/simultaneous calls; null callback; bounce completion across varied `dt`; P2 adoption/removal.
- **Visual checks:** active animation, bounce, Empty frame, popped item origin, exact 32x32 sprite/world alignment.
- **Gameplay checks:** one base coin and mushroom interaction in vertical slice; collection outcome belongs to P2/P3/P1/P4 HUD seam.
- **Definition of done:** modern tests run, spawn corrections remain intact, lifecycle/handoff and visual/gameplay checks pass, and all dimensions cite evidence.
- **Suggested commit message:** `person4: validate coin and mushroom block behavior`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-PAYLOAD-BLOCK-001 — Special Brick and block variants

- **Task ID:** `P4-PAYLOAD-BLOCK-001`
- **Title:** Implement special Brick payloads and mapped block variants
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** BLOCKED
- **Purpose:** Give P2's variant metadata a P4 behavior surface for coin Brick and Mushroom/1-Up/Star blocks while preserving normal Brick.
- **Exact current behavior:** P2 parses variant values but drops them during construction; Brick only performs the frozen normal break; MushroomBlock always spawns Mushroom; P4 has no variant/payload API.
- **Missing behavior checklist:** [ ] closed actor/power eligibility policy; [ ] explicit variant type/value; [ ] normal Brick unchanged; [ ] coin Brick payload behavior; [ ] Mushroom/1-Up/Star mapping; [ ] unknown variant safe failure/diagnostic; [ ] exactly-once spawn; [ ] active/empty/inactive lifecycle; [ ] P2 immediate adoption; [ ] P3 payload behavior; [ ] variant table tests.
- **Exact target files:** Brick and MushroomBlock header/source; CoinBlock only if shared variant surface truly needs it; planned P4 block test.
- **Allowed edit files:** exact P4 targets only.
- **Read-only dependency files:** P2 MapManager/LevelManager/Physics; P3 item types/factory; frozen constants and fragment baseline.
- **Do not implement:** variant pixel decoding/forwarding, collision actor detection, item classes/effects, normal Brick rewrite, map edits, or P2 runtime ownership.
- **Dependencies:** CLOSED `DEC-BLOCK-ACTOR-ELIGIBILITY`; `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-VARIANTS`, `CON-P2-P4-SPAWN-HANDOFF`; P2 forwarding task; P3 payload APIs.
- **Contracts provided:** P4 behavior/lifecycle side of block collision, variants, and spawn handoff contracts.
- **Contracts consumed:** P2 provides actor context/variant and adopts spawn; P3 provides payload constructors/behavior.
- **Group5 reference if useful:** special Brick outcome may be behavior reference only.
- **Do-not-copy warning:** no Group5 map decoder, raw owning callbacks, player-class logic, or chatbot feature.
- **Implementation notes:** choose behavior from a small enum/value supplied by P2; never inspect pixels. Spawn via the existing callback and retain no pointer. Keep the normal constructor/default behavior byte-for-behavior equivalent where practical.
- **SFML 3.1 notes:** variant rendering uses current rectangle/sprite APIs and P4 AnimationComponent.
- **Compile checks:** syntax check all touched blocks plus P2/P3 public contract headers and variant test.
- **Runtime checks:** every recognized variant, unknown variant, eligible/ineligible actor cases from decision, repeated hit, null callback, P2 adoption and cleanup; rerun all frozen baselines.
- **Visual checks:** variant active/empty states and emerged payload are distinguishable and tile-aligned.
- **Gameplay checks:** each variant produces the intended P3 effect once and updates event/HUD flow once.
- **Definition of done:** decision is CLOSED, P2 forwards all mapped variants, P3 payloads integrate, normal Brick regressions pass, and every dimension has evidence.
- **Suggested commit message:** `person4: add variant-aware payload block behavior`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-CLOUD-001 — CloudBlock behavior

- **Task ID:** `P4-CLOUD-001`
- **Title:** Implement the approved CloudBlock semantics
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN` and policy `NEEDS_HUMAN_DECISION`
- **Current status:** BLOCKED
- **Purpose:** Implement P4's required CloudBlock without colliding with P2's separately required MovingBlock or current marker mapping.
- **Exact current behavior:** CloudBlock is an empty non-GameObject class. P2 currently maps `CloudPlatform` markers to P2 `MovingBlock`; no P4 CloudBlock is constructed or collidable.
- **Missing behavior checklist:** [ ] CLOSED static/moving/combined semantics; [ ] derive from approved P4 block base if required; [ ] texture/frame/size; [ ] one-way or solid collision rule; [ ] lifecycle; [ ] construction surface; [ ] P2 mapping integration by P2; [ ] player/enemy behavior tests; [ ] no duplicate platform for one marker.
- **Exact target files:** `include/Objects/Blocks/CloudBlock.hpp`; `src/Objects/Blocks/CloudBlock.cpp`; planned P4 block test.
- **Allowed edit files:** exact P4 targets only.
- **Read-only dependency files:** P2 MovingBlock/MapManager/LevelManager/Physics; P3 player/enemies; P4 Block/Animation.
- **Do not implement:** MovingBlock, map palette/decoding, physics engine, player collision logic, or silent semantics.
- **Dependencies:** OPEN `DEC-CLOUD-SEMANTICS`; block-size/animation contracts; P2 mapping/collision provider after decision.
- **Contracts provided:** approved CloudBlock side of `CON-P2-P4-BLOCK-COLLISION`, `CON-P2-P4-BLOCK-LIFECYCLE`, and visual-size contract.
- **Contracts consumed:** P2 construction/collision/motion boundary; P3 actor bounds.
- **Group5 reference if useful:** visual/collision feel may be reference-only; it cannot resolve the human decision.
- **Do-not-copy warning:** no Group5 class hierarchy/raw ownership/map logic or chatbot code.
- **Implementation notes:** keep the class minimal once semantics are decided. If the marker remains exclusively MovingBlock, record CloudBlock's distinct construction source; do not create two objects for one pixel.
- **SFML 3.1 notes:** use P4 block/animation SFML 3 surfaces.
- **Compile checks:** syntax check CloudBlock with Block/Animation and P2 public headers.
- **Runtime checks:** construction, size, collision sides/one-way rule, lifecycle, static/moving combination selected by decision, no duplicate marker object.
- **Visual checks:** cloud tile(s) align with collision and move only if the decision says so.
- **Gameplay checks:** player can traverse/stand/fall through exactly as approved; enemy behavior matches the contract.
- **Definition of done:** decision recorded, ownership is non-duplicated, P2 integration and all dimensions pass with evidence.
- **Suggested commit message:** `person4: implement approved cloud block semantics`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-AUDIO-001 — AudioSystem

- **Task ID:** `P4-AUDIO-001`
- **Title:** Implement AudioSystem music, SFX, and volume surface
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Provide one controlled SFML 3.1 audio service for P1 state/event cues without embedding gameplay policy.
- **Exact current behavior:** AudioSystem is empty. Audio assets exist, but no buffers/music, volume, cue identifiers, load errors, or playback calls exist.
- **Missing behavior checklist:** [ ] one application-visible service/singleton policy required by PLAN; [ ] named SFX catalog; [ ] streamed music; [ ] buffer lifetime; [ ] play/stop/switch; [ ] music/SFX volume and clamp; [ ] mute; [ ] missing/unsupported asset result; [ ] duplicate music suppression; [ ] event hookup; [ ] cleanup.
- **Exact target files:** `include/Audio/AudioSystem.hpp`; `src/Audio/AudioSystem.cpp`; planned data/audio portion of P4 tests. Asset paths integrate later.
- **Allowed edit files:** exact source targets; existing audio assets are read-only unless an evidence-backed asset correction is separately recorded.
- **Read-only dependency files:** P1 MyApp/Mediator/states; P4 UserData/GUI; `CMakeLists.txt` until package task.
- **Do not implement:** gameplay event detection, menu state logic, persistence, asset-root decision, audio asset replacement, or chatbot voice/chat features.
- **Dependencies:** source core can start; final integration consumes `CON-P1-P4-AUDIO-EVENTS` and CLOSED `DEC-ASSET-ROOT` through package configuration.
- **Contracts provided:** P4 consumer/API side of `CON-P1-P4-AUDIO-EVENTS`.
- **Contracts consumed:** P1 typed cue/music requests and settings volume; approved runtime asset resolver/root.
- **Group5 reference if useful:** cue timing/catalog may be reference-only.
- **Do-not-copy warning:** do not import LLM/chat audio, raw global resource ownership, or Group5 paths.
- **Implementation notes:** keep sound buffers owned for the lifetime of active sounds. Avoid a new dependency. Expose failure rather than printing and pretending playback succeeded.
- **SFML 3.1 notes:** verify SFML 3 Audio constructors/lifetime APIs; do not use SFML 2 signatures from memory.
- **Compile checks:** direct C++20/SFML 3.1 syntax/link check for AudioSystem with SFML Audio/System.
- **Runtime checks:** load/play/stop music and overlapping cues; volume 0/min/max/out-of-range; repeat switching; missing file; teardown.
- **Visual checks:** not applicable; settings UI displays the effective volume under GUI integration.
- **Gameplay checks:** jump/coin/block/death/completion cues occur once at P1 event boundaries; music transitions with states.
- **Definition of done:** service lifetime and errors are safe, P1 event/settings integration works from packaged root, and all applicable dimensions are evidenced.
- **Suggested commit message:** `person4: add SFML 3 audio service and cue controls`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ] (n/a for audio itself); GAMEPLAY_VERIFIED [ ].

### P4-GUI-001 — Reusable GUI controls

- **Task ID:** `P4-GUI-001`
- **Title:** Implement reusable menu GUI controls
- **Owner:** Person 4
- **Priority:** P0
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** READY
- **Purpose:** Supply P1 menu states with renderable, keyboard-accessible controls that report intent without owning navigation.
- **Exact current behavior:** GUI is an empty class. P1 menu shells are also empty; fonts/textures exist but no loading, focus, layout, text input, or control-event surface exists.
- **Missing behavior checklist:** [ ] minimal button/selectable control; [ ] focus/hover/pressed/disabled states; [ ] keyboard navigation and activation; [ ] text entry support needed by EnterName; [ ] bounds/hit test; [ ] resize/layout behavior; [ ] font/texture load failure; [ ] accessible focus/contrast/readability; [ ] action result returned to P1; [ ] no state transition inside GUI.
- **Exact target files:** `include/UI/GUI.hpp`; `src/UI/GUI.cpp`; planned GUI check under P4 tests.
- **Allowed edit files:** exact targets only; asset consumption is read-only until packaging.
- **Read-only dependency files:** all P1 states/MyApp; P4 Audio/UserData; font/texture assets.
- **Do not implement:** P1 menu choices/transitions, Leaderboard behavior, HUD, UserData, asset-root fallback, or a broad UI framework.
- **Dependencies:** P1 defines command/input needs through `CON-P1-P4-GUI`; implementation can start with existing PLAN menu scope.
- **Contracts provided:** P4 control/focus/render/action side of `CON-P1-P4-GUI`.
- **Contracts consumed:** P1 supplies labels/actions/focus flow; approved asset root later.
- **Group5 reference if useful:** visual layout may be reference-only.
- **Do-not-copy warning:** no Group5 raw widget pointers, state navigation, hard-coded paths, or chatbot menu.
- **Implementation notes:** one reusable control set is enough. Return action IDs/events; P1 decides transitions. Make keyboard operation first-class, not a mouse-only afterthought.
- **SFML 3.1 notes:** use SFML 3 event variants, text/font constructors, and rectangle/vector APIs; validate actual installed signatures.
- **Compile checks:** direct syntax check GUI and a small input/action test against SFML 3.1.
- **Runtime checks:** focus traversal, activate, disabled control, pointer hit, Unicode text entry/backspace/confirm/cancel, resize, missing font.
- **Visual checks:** normal/focused/pressed/disabled states distinct; text not clipped; contrast/readability and 800x600 plus resize checked.
- **Gameplay checks:** P1 Main/Name/Settings/Selection/Pause/Death/Win states consume controls without duplicate actions.
- **Definition of done:** P1 can implement every assigned menu without duplicating widgets, accessibility checks pass, and all dimensions are tracked.
- **Suggested commit message:** `person4: add reusable accessible menu controls`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-HUD-001 — Gameplay HUD

- **Task ID:** `P4-HUD-001`
- **Title:** Implement LevelGUI score, coins, lives, and level HUD
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** BLOCKED
- **Purpose:** Render the authoritative session snapshot in screen space and update exactly once per P1 event/outcome.
- **Exact current behavior:** LevelGUI is empty; no HUD exists. UserData has in-memory values but is disconnected. No score/coin/lives events reach P4.
- **Missing behavior checklist:** [ ] immutable/snapshot input; [ ] player/profile label as approved; [ ] score; [ ] coins; [ ] lives; [ ] world-level identity; [ ] 1P/2P presentation under decision; [ ] screen-space view; [ ] resize; [ ] exactly-once refresh; [ ] safe missing font/texture; [ ] readable contrast.
- **Exact target files:** `include/UI/LevelGUI.hpp`; `src/UI/LevelGUI.cpp`; planned P4 HUD test.
- **Allowed edit files:** exact targets only; UserData changes only under persistence card.
- **Read-only dependency files:** P1 GameState/Mediator; P3 players/items; P2 LevelManager; P4 GUI/UserData.
- **Do not implement:** score calculation, coin collection, death/lives policy, camera, GameState render order, or persistence.
- **Dependencies:** `CON-P1-P4-SCORE-COINS-LIVES`, `CON-P1-P4-HUD-DATA`; P1 mediator/GameState; CLOSED `DEC-MULTIPLAYER-DEATH` for lives presentation.
- **Contracts provided:** P4 rendering/consumer side of HUD and score/coins/lives contracts.
- **Contracts consumed:** P1 supplies the stable validated value snapshot and screen-view render timing; HUD does not depend on durable UserData persistence.
- **Group5 reference if useful:** HUD composition may be visual reference only.
- **Do-not-copy warning:** no direct polling of Group5/player raw pointers, fixed camera assumptions, or chat UI.
- **Implementation notes:** render from a value snapshot so the HUD does not own or retain gameplay objects. P1 restores the screen-space view before calling render.
- **SFML 3.1 notes:** use current Text/Font/View APIs and avoid SFML 2 constructors.
- **Compile checks:** syntax check LevelGUI with UserData/value model and SFML 3.1.
- **Runtime checks:** zero/max/large values, coin rollover result supplied by UserData, lives zero, 1P/2P decision cases, resize, missing resource.
- **Visual checks:** HUD stays fixed while camera moves; values fit; focus is not relevant; contrast/readability verified over bright/dark scenes.
- **Gameplay checks:** coin/item/enemy/death/completion update displayed values once and at the expected frame boundary.
- **Definition of done:** canonical data only, no gameplay ownership, all decided modes display correctly, and five dimensions cite evidence.
- **Suggested commit message:** `person4: add screen-space gameplay HUD`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-PERSISTENCE-001 — UserData save/load and progression

- **Task ID:** `P4-PERSISTENCE-001`
- **Title:** Implement validated UserData persistence and progression
- **Owner:** Person 4
- **Priority:** P1
- **Requirement class:** `REQUIRED_BY_PLAN` and policy `NEEDS_HUMAN_DECISION`
- **Current status:** BLOCKED
- **Purpose:** Persist profiles/session/progression without losing data or inventing behavior outside approved death/save rules.
- **Exact current behavior:** UserData holds and mutates values in memory, clamps lives at zero, converts each 100 coins to a life, and records monotonic unlock coordinates. No serialization or validation exists; `SaveData/` contains only `.gitkeep`.
- **Missing behavior checklist:** [ ] **unblocked first-slice provider checklist:** validate exactly W1_LV1 through W3_LV3, safe default/current/unlocked snapshots, eight successors plus terminal W3_LV3, and duplicate-completion idempotence; [ ] `DEC-PERSISTENCE` `RESOLVED` for the durable portion; [ ] validate names and numeric ranges; [ ] character/player representation; [ ] serialize/load; [ ] missing-file defaults; [ ] corrupt/unknown-version failure; [ ] loss-resistant temp-write/replace; [ ] preserve old valid data on failure; [ ] session reset per respawn decision; [ ] surfaced error; [ ] focused round-trip checks.
- **Exact target files:** `include/Core/UserData.hpp`; `src/Core/UserData.cpp`; planned `tests/Person4DataContracts.cpp`; approved schema/template under `SaveData/` only after decision.
- **Allowed edit files:** exact targets; no generated personal save committed.
- **Read-only dependency files:** P1 states/Mediator; P3 players; P2 stages; P4 HUD/Leaderboard/GUI.
- **Do not implement:** P1 semantic event definitions/state flow, multiplayer death policy, player reset, leaderboard UI, map progression inference, external database, or a new dependency. P4 may map the stable P1 events to save timing only after `DEC-PERSISTENCE` approves that mapping.
- **Dependencies:** the in-memory nine-stage provider checklist may proceed immediately under the workflow's unblocked-child rule and must land before `P1-SELECT-001`/`P1-WIN-001` integration. The full durable card remains blocked by OPEN `DEC-PERSISTENCE`, `DEC-RESPAWN`, and relevant `DEC-MULTIPLAYER-DEATH`; it uses `CON-P1-P4-PERSISTENCE`, `CON-P1-P4-PROGRESSION`, and `CON-P1-P4-SCORE-COINS-LIVES`.
- **Contracts provided:** validated data/schema/I/O side of persistence, progression, and score/coins/lives contracts.
- **Contracts consumed:** P1 supplies stable semantic session events and explicit load/save requests where UI needs them; P4 applies the approved event-to-save timing to its own UserData/profile state.
- **Group5 reference if useful:** fields/CSV technique may be reference-only; Group4 decisions/schema govern.
- **Do-not-copy warning:** no Group5 raw player serialization, path assumptions, chatbot history, or silent schema.
- **Implementation notes:** use standard library I/O and the smallest versioned format that meets the decision. Validate before mutating live data; write a sibling temporary file then replace only after complete successful output, within safe platform constraints.
- **SFML 3.1 notes:** data layer should be SFML-independent.
- **Compile checks:** C++20 syntax check UserData/data tests without needing a window.
- **Runtime checks:** default, round trip, Unicode/name policy, min/max values, 99/100/199 coins, lives zero, all nine coordinates, corrupt/truncated/unknown version, failed write preserving prior data.
- **Visual checks:** P1/P4 UI shows loaded/error/default state accurately; no visual requirement inside serializer.
- **Gameplay checks:** approved death/restart/save/complete/relaunch flows preserve/reset exactly decided fields and unlock no out-of-range stage.
- **Definition of done:** decisions are `RESOLVED`, failure is non-destructive, nine-level progression and round trips pass, consumers integrate, and dimensions have evidence. Completing only the unblocked in-memory provider checklist satisfies the P0 progression contract but does not make this P1 durable-persistence card `DONE`.
- **Suggested commit message:** `person4: add validated profile persistence and progression`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ] (n/a internally); GAMEPLAY_VERIFIED [ ].

### P4-LEADERBOARD-001 — Leaderboard CSV and state

- **Task ID:** `P4-LEADERBOARD-001`
- **Title:** Implement leaderboard data and LeaderboardState
- **Owner:** Person 4
- **Priority:** P2
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** BLOCKED
- **Purpose:** Store, rank, and display completed scores through the P4-owned state while P1 only routes to/from it.
- **Exact current behavior:** LeaderboardState is empty; expected `LeaderBoard.csv` does not exist; no parser, ranking, write, limit, or GUI integration exists.
- **Missing behavior checklist:** [ ] schema consistent with persistence decision; [ ] safe missing-file behavior; [ ] quoted/delimiter-safe name handling; [ ] numeric validation; [ ] deterministic score order/tie-break; [ ] entry limit; [ ] loss-resistant append/rewrite; [ ] P4 GUI display; [ ] P1 back/navigation contract; [ ] corrupt-row policy; [ ] packaged writable location.
- **Exact target files:** LeaderboardState header/source; `tests/Person4DataContracts.cpp`; approved `LeaderBoard.csv` template/location after decision; GUI consumed read-only unless a GUI-task change is separately tracked.
- **Allowed edit files:** exact P4 targets only.
- **Read-only dependency files:** P1 State/MainMenu/Win flow; P4 UserData/GUI/package surfaces outside their own cards.
- **Do not implement:** P1 navigation, score calculation, profile persistence policy, online leaderboard, database, map logic, or edit other P1 states.
- **Dependencies:** `P4-PERSISTENCE-001`, `P4-GUI-001`, P1 State lifecycle/routing; CLOSED `DEC-PERSISTENCE`; progression/completion event integration.
- **Contracts provided:** leaderboard read/write/display surface subordinate to `CON-P1-P4-PERSISTENCE` and `CON-P1-P4-PROGRESSION`.
- **Contracts consumed:** validated name/score and approved submission time from P1/P4 persistence.
- **Group5 reference if useful:** CSV parsing/ranking behavior may be reference-only.
- **Do-not-copy warning:** no Group5 path/raw state ownership/chat data; do not import malformed-row assumptions.
- **Implementation notes:** reuse persistence parsing/writing helpers only if they already exist after that task; do not invent a generic storage layer. Stable sorting must have a documented tie-break.
- **SFML 3.1 notes:** LeaderboardState rendering/input uses P4 GUI and SFML 3 event APIs through the P1 State contract.
- **Compile checks:** syntax check state/data code with P1/P4 public headers; CSV tests are headless.
- **Runtime checks:** missing/empty file, one/many entries, ties, commas/quotes/name policy, corrupt rows, max list, write failure preserving old data.
- **Visual checks:** readable ranked list, long names constrained, empty/error states visible, keyboard Back works.
- **Gameplay checks:** approved final/level completion submits exactly once and reopening the app displays the entry.
- **Definition of done:** schema/timing are approved, data failure is safe, P1 routing and P4 GUI/package integration pass, and all dimensions are evidenced.
- **Suggested commit message:** `person4: add persistent leaderboard state`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

### P4-PACKAGE-001 — SFML 3.1 build, assets, and release package

- **Task ID:** `P4-PACKAGE-001`
- **Title:** Produce reproducible C++20/SFML 3.1 build and runtime package
- **Owner:** Person 4
- **Priority:** P2
- **Requirement class:** `REQUIRED_BY_PLAN`
- **Current status:** BLOCKED
- **Purpose:** Align configured dependency version and place executable, libraries, assets, and writable data so production works outside the repository CWD.
- **Exact current behavior:** CMake uses C++20 but pins SFML 3.0.0; sources are globbed; Windows DLL copy assumes an SFML layout; no project asset copy/install rule exists. Full build currently fails in FreeType `windres` under a path containing spaces. Runtime code assumes inconsistent relative paths.
- **Missing behavior checklist:** [ ] CLOSED asset-root convention; [ ] SFML 3.1 configuration; [ ] reproducible configure/build outside space-sensitive failure or documented fixed invocation; [ ] production `SuperMario` link; [ ] copy/install required SFML runtime libraries; [ ] copy assets once; [ ] writable save/leaderboard location; [ ] missing-asset diagnostics; [ ] clean-package launch from package directory; [ ] all nine named maps present by file identity only; [ ] license/readme if required; [ ] release smoke/size inventory.
- **Exact target files:** `CMakeLists.txt`; future minimal install/package rules; package layout; approved data templates. Production path consumers change only through their owning cards/cross-owner requests.
- **Allowed edit files:** P4 build/package targets; coordinate exact P2/P1/P3 path-consumer requests instead of editing them.
- **Read-only dependency files:** all P1/P2/P3 production source; map image content; P4 source outside separately active cards.
- **Do not implement:** gameplay, path fallbacks per subsystem, map edits/audit, dependency downgrade, network installer, chatbot/LLM assets, or deletion of user data.
- **Dependencies:** CLOSED `DEC-ASSET-ROOT`; production P1 entry; P2 load readiness; P3/P4 visuals; audio/GUI/persistence/leaderboard; all P0/P1 runtime blockers.
- **Contracts provided:** approved read-only runtime asset environment plus the separately `DEC-PERSISTENCE`-approved writable-data location and release evidence.
- **Contracts consumed:** `CON-P1-P4-PERSISTENCE`, `CON-P1-P4-AUDIO-EVENTS`, `CON-P2-P4-BLOCK-VISUAL-SIZE` asset needs, plus P2 load contract; asset-root decision.
- **Group5 reference if useful:** packaging file inventory only; no architecture or special feature.
- **Do-not-copy warning:** no Group5 raw paths, LLM/Ollama binaries/models, saves, or dependency versions.
- **Implementation notes:** one asset-root rule, resolved once, is preferable to per-class `../` retries. Keep writable user data separate from read-only packaged assets. Do not claim source failure for the known external `windres` path issue.
- **SFML 3.1 notes:** configure/link the actual 3.1 target, run direct syntax plus full link, and verify Windows runtime DLLs/plugins needed by Audio/Graphics.
- **Compile checks:** clean configure; full warning-enabled build; direct 3.1 syntax baseline; package target; run in a path with and without spaces if supported.
- **Runtime checks:** launch from package directory and an unrelated CWD; load menu/font/audio/blocks/one level; save/reload; missing asset gives actionable error; no repository dependency.
- **Visual checks:** packaged menus/HUD/entities/blocks render; no half-size blocks or missing fonts/textures.
- **Gameplay checks:** first vertical slice and all nine separate `LV-*` validations run from the package; this verifies file deployment, never map-pixel correctness.
- **Definition of done:** clean SFML 3.1 build/package launches independently, asset/save roots obey decisions, required checks pass, and all dimensions cite evidence.
- **Suggested commit message:** `person4: package the SFML 3.1 production build`
- **Completion dimensions:** SOURCE_DONE [ ]; INTEGRATION_DONE [ ]; RUNTIME_TESTED [ ]; VISUALLY_VERIFIED [ ]; GAMEPLAY_VERIFIED [ ].

## 9. Tests required per task

Use the smallest runnable checks; do not add a framework. Every result recorded in the master board includes command, commit, platform, and outcome.

| Card | Minimum runnable check | Required integration/manual proof | Completion dimensions it can close |
|---|---|---|---|
| `BASE-P4-BRICK-001` | preserve P2 27-check normal Brick regression | optional later visual proof does not reopen baseline | already SOURCE/INTEGRATION/RUNTIME |
| `BASE-P4-FRAGMENT-001` | preserve fragment spawn/adoption/expiry/removal regression | optional visible fragment proof downstream | already SOURCE/INTEGRATION/RUNTIME |
| `BASE-P4-BLOCK-INTEGRATION-001` | compile/source assertions for constants/lifecycle/popped item/offset; preserve runtime suite | base payload integration under question task | frozen scope only |
| `P4-ANIMATION-001` | registration/failure/switch/timing/large-dt assertions | visible P3 player + enemy action changes | all five after P3 integration |
| `P4-BLOCK-SIZE-001` | rendered bounds vs hitbox for 16->32, repeated/frame/multi-tile cases | representative stage block alignment | all five |
| `P4-QUESTION-BLOCK-001` | modern SFML 3 tests for finite bounce/count/once-only/handoff | visible Coin/Mushroom interaction and collection seam | all five |
| `P4-PAYLOAD-BLOCK-001` | exhaustive variant x eligible-actor table and ownership assertions | visible/gameplay effect once for every required variant | all five |
| `P4-CLOUD-001` | decided construction/collision/lifecycle table | visible traversal without duplicate MovingBlock | all five |
| `P4-AUDIO-001` | resource lifetime, volume, missing-file, play/switch/stop check | audible cue/music/settings verification from packaged root | SOURCE/INTEGRATION/RUNTIME/GAMEPLAY; visual n/a |
| `P4-GUI-001` | input-to-action, focus, text, resize, missing-font checks | keyboard/mouse/readability check across P1 menus | all five |
| `P4-HUD-001` | snapshot/value/resize/view assertions | camera-moving HUD plus event updates | all five |
| `P4-PERSISTENCE-001` | round-trip, bounds, corrupt/truncate/version, failed-write preservation | relaunch death/win/progression flows | all five; internal visual may be n/a |
| `P4-LEADERBOARD-001` | CSV quote/tie/sort/corrupt/missing/write-failure checks | display/navigation and exactly-once submission | all five |
| `P4-PACKAGE-001` | clean configure/build/package plus launch smoke | first slice and nine validation entries from package | all five |

Every touched translation unit receives a direct `g++ -std=c++20 -fsyntax-only` check using Group4 include and installed SFML 3.1 include paths. Full build/package evidence must use configured SFML 3.1, not the current 3.0 pin. Modernizing `TestCoinBlock.cpp` is required; its current syntax failure is not a production-source regression.

## 10. Integration checks

1. **Frozen block regression:** rerun P2's 27-check suite after every Block/Animation change. Normal Brick still becomes inactive once, creates four P4 fragments, and P2 adopts/removes them; shared constants/lifecycle stay unchanged.
2. **Visual-size seam:** P2's existing world-size call makes every P4 block sprite match the collision rectangle, normally 32x32, without a P2-specific helper call.
3. **Animation seam:** P3 supplies keys/frames and owns sprite/texture; P4 component switches/times frames deterministically with no ownership or missing-key spam.
4. **Payload seam:** after decisions/contracts, P2 supplies actor context + variant, P4 selects/triggers behavior once, P3 creates/implements payload, and P2 adopts it immediately. P4 never parses pixels or retains the raw callback pointer.
5. **Cloud seam:** exactly one approved object results from a cloud marker; P4 CloudBlock and P2 MovingBlock roles do not overlap silently.
6. **P1/P4 event seam:** one score/coin/lives/death/completion event changes UserData/HUD/audio/persistence no more than once and never causes a reentrant state teardown.
7. **Screen-space seam:** P1 applies world camera then restores HUD/menu view; GUI/HUD remain stable during movement/resize and support keyboard use.
8. **Persistence seam:** approved death/restart/win/final transitions save exactly when decided; failed/corrupt I/O preserves valid prior data and presents an actionable UI state.
9. **First integrated vertical slice:** production entry -> selected player -> loaded level -> visible animated player/world -> correctly sized normal Block interaction -> enemy -> item -> HUD/audio -> pit/death -> approved restart -> WinFlag -> progression. Preserve baselines; special variants are not required to prove normal Brick.
10. **Nine-level/release seam:** package contains the nine named map files and separate validation entries `LV-W1-LV1` through `LV-W3-LV3` pass from the package. P4 verifies deployment and UI/audio/data behavior, not map pixels.
11. **Failure seam:** missing texture/font/audio, bad save/CSV, unknown block variant, failed level load reported by P2, and unrelated working directory all fail safely and visibly.

Merge hotspots: `Block.*`, Brick/MushroomBlock files, `AnimationComponent.*`, `UserData.*`, `GameState.*` (P1-owned), `LevelManager.*`/`MapManager.*` (P2-owned), and `CMakeLists.txt`. Assign one active owner per hotspot. P4 requests P1/P2/P3 seam edits through `04_USAGE_AND_PROGRESS_WORKFLOW.md`; integration responsibility never authorizes a direct cross-owner edit.

## 11. Final owner-specific definition of done

Person 4 is complete only when all of the following are true:

- All 11 remaining P4 cards are DONE, and the three frozen baseline cards remain DONE with their regression evidence. Required work is not hidden as DEFERRED or EXCLUDED.
- Every P4-owned production file in Section 1 remains accounted for. MovingBlock stays P2-owned; P3 items stay P3-owned; all non-Leaderboard states stay P1-owned.
- Normal Brick, four real BrickFragments, corrected constants/public lifecycle, popped Coin state, Mushroom one-tile offset, P2 immediate unique ownership, inactive cleanup, and same-frame exclusion are preserved.
- Animation is deterministic and consumed visibly by P3; every P4 block visual matches its P2 world/physics size, normally 32x32.
- Base question blocks, approved special/payload variants, and approved CloudBlock semantics pass compile/runtime/visual/gameplay checks without P4 map parsing or P2/P3 implementation.
- Audio, GUI, HUD, UserData persistence/progression, Leaderboard, and packaging integrate through the canonical P1/P4 contracts and recorded human decisions.
- `DEC-BLOCK-ACTOR-ELIGIBILITY`, `DEC-CLOUD-SEMANTICS`, `DEC-ASSET-ROOT`, and `DEC-PERSISTENCE` are CLOSED before dependent acceptance; `DEC-MULTIPLAYER-DEATH` and `DEC-RESPAWN` are also CLOSED where lives/session reset are consumed. No recommended default was silently implemented.
- C++20/SFML 3.1 direct checks pass; a clean configured 3.1 build and release package run outside the repository working directory with coherent read-only assets and safe writable data.
- The first integrated vertical slice passes, and all nine separate `LV-*` entries pass their P4 visual/audio/HUD/data/package portions without engine duplication or map-content edits.
- Every task independently records SOURCE_DONE, INTEGRATION_DONE, RUNTIME_TESTED, VISUALLY_VERIFIED, and GAMEPLAY_VERIFIED with linked evidence; DONE never means source-only.
- No P4 commit includes Group5 chatbot/LLM code, Group5 raw-pointer ownership, hard-coded alternate roots, map-image edits/audits, direct cross-owner implementation, new unnecessary dependencies, or unrequested abstractions.

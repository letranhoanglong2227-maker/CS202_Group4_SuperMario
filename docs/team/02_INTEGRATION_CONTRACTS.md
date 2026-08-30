# Group4 Super Mario — Integration Contracts

These 25 cards are the canonical cross-owner boundaries. They define data, ownership, call timing, and failure behavior; they do not silently resolve the eight human decisions. Provider code lands before consumer integration whenever possible.

Contract status vocabulary:

- `VERIFIED_BASELINE` — current behavior is evidenced and must be preserved.
- `DEFINED_PENDING_IMPLEMENTATION` — interface behavior is sufficiently defined for task planning but source is unfinished.
- `PARTIAL` — part of the current interface exists; named gaps remain.
- `BLOCKED_DECISION` — implementation cannot be finalized until the named `DEC-*` card is approved.

## P1 ↔ P2 contracts

### CON-P1-P2-ACTIVE-LEVEL — active level ownership

- **Provider / implementation owner:** P2 provides `ConfiguredLevel` / `LevelManager`; P1 provides `GameState` ownership shell.
- **Consumer:** P1 calls the active level; P2 consumes stable injected player views and callbacks.
- **Runtime owner:** P1 `GameState` owns exactly one active level object for the state's lifetime. P2 remains implementation owner of the level classes and owner of their internal map/runtime objects.
- **Lifetime / ownership:** The active level is destroyed only after its update returns. P2 internal runtime objects remain `unique_ptr`-owned. No contract transfers players to P2.
- **Input:** selected `worldId`, `levelId`, borrowed player view, callbacks.
- **Output:** a ready-or-failed active level capable of one update/render per frame.
- **Call direction:** P1 constructs/configures → P2 level; P1 calls update/render; P2 calls registered events back to P1.
- **Timing:** inject players and callbacks before load/update. Queue state replacement and level destruction until after `LevelManager::update` returns.
- **Current interface / missing behavior:** P1 `GameState` owns injected player/level objects, rebinds the borrowed player view, validates `isLoaded()` plus world extent, invokes update/render once, and detaches callbacks/views before teardown. The production selection-to-stage construction route and packaged P2 load provider remain missing.
- **Failure behavior:** Failed load produces a non-running level state plus diagnostic; P1 shows a safe error/returns to selection. Never update an empty failed world as though it loaded.
- **Status:** `PARTIAL`.
- **Dependent tasks:** `P1-SELECT-001`, `P1-GAME-001`, `P2-LOAD-001`, `P2-NINE-LEVEL-001`.

### CON-P1-P2-LEVEL-LOAD — load request, readiness, and error

- **Provider / implementation owner:** P2.
- **Consumer:** P1 `GameState` and stage-selection flow.
- **Runtime owner:** Load result is value/status data held by the active level; map objects remain P2-owned.
- **Lifetime / ownership:** Result remains queryable until the next load/destruction; error text must not dangle.
- **Input:** canonical stage identity and borrowed players; path construction occurs behind the P2/P4-approved asset-root policy.
- **Output:** explicit success/failure and diagnostic; on success, valid world extent and spawn/runtime registry.
- **Call direction:** P1 requests → P2 loads → P1 queries/receives result.
- **Timing:** once during state activation/reload, before the first gameplay update or camera query.
- **Current interface / missing behavior:** `LevelManager::load` returns `bool`, `isLoaded()` and MapManager's retained error are queryable, and P1 suppresses runtime update/render on failure. `ConfiguredLevel` still builds a relative `assets/textures/...` path with a one-level fallback; the shared executable-adjacent resolver and production error screen/return route remain missing.
- **Failure behavior:** Clear partial runtime, retain diagnostic, report false, do not call gameplay callbacks, and let P1 transition safely.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`; `DEC-ASSET-ROOT` is resolved.
- **Dependent tasks:** `P2-LOAD-001`, `P1-GAME-001`, `P2-NINE-LEVEL-001`, `P4-PACKAGE-001`.

### CON-P1-P2-WORLD-EXTENT — map-derived world rectangle

- **Provider / implementation owner:** P2 MapManager/LevelManager.
- **Consumer:** P1 camera; P2 bounds, pit, cleanup, and projectile logic.
- **Runtime owner:** immutable/value extent associated with the successfully loaded map.
- **Lifetime / ownership:** valid from successful load until clear/reload/destruction.
- **Input:** parsed dynamic image width, 15 logical gameplay rows, `MapFormat::TILE_SIZE` (`CELL_SIZE=64` in the adopted sketches).
- **Output:** pixel-space world rectangle (or width/height values) with origin and explicit validity.
- **Call direction:** consumers query P2 after successful load.
- **Timing:** compute once per load; never derive from window size or a hard-coded level width.
- **Current interface / missing behavior:** P2 exposes an optional map-derived pixel rectangle from loaded state; P1 camera and P2 bounds/pit/cleanup consume it. Visual acceptance remains.
- **Failure behavior:** invalid/unloaded query returns no extent or an explicit invalid state, never zero masquerading as a valid tiny world.
- **Status:** `IMPLEMENTED_PENDING_VISUAL_ACCEPTANCE`.
- **Dependent tasks:** `P2-EXTENT-001`, `P2-BOUNDS-001`, `P2-PIT-001`, `P2-CLEANUP-001`, `P1-CAMERA-001`.

### CON-P1-P2-CAMERA — follow and clamp responsibility

- **Provider / implementation owner:** P2 provides extent; P1 implements camera policy.
- **Consumer:** P1 `GameState` render path.
- **Runtime owner:** P1 owns the `sf::View`; P2 owns no camera state.
- **Lifetime / ownership:** view lives with `GameState`; world extent is copied/queried value data.
- **Input:** active target position, viewport size, valid world rectangle.
- **Output:** clamped gameplay view plus restoration of the default/UI view.
- **Call direction:** P1 queries P2 extent, computes view, sets window view, calls P2 render, then restores UI view.
- **Timing:** after player/world update and before world render each frame.
- **Current interface / missing behavior:** P1 queries P2's optional pixel rectangle, follows the owned 1P hitbox center, applies the clamped world view for P2 render, and restores the exact prior view. P4 HUD overlay and gameplay visual acceptance remain.
- **Failure behavior:** on failed load use safe default view and no gameplay render; if world is smaller than viewport, center it without negative/out-of-world drift.
- **Status:** `IMPLEMENTED_PENDING_VISUAL_ACCEPTANCE`.
- **Dependent tasks:** `P2-EXTENT-001`, `P1-CAMERA-001`, `P1-GAME-001`.

### CON-P1-P2-DEATH — exactly-once player death event

- **Provider / implementation owner:** P2 detects world/hazard/contact death; P1 consumes and transitions.
- **Consumer:** P1 `GameState` / mediator; P4 lives data and P3 reset are downstream consumers through P1.
- **Runtime owner:** P1 owns sequencing and transitions; P4 UserData owns lives values; P1 owns the selected player under resolved `DEC-PLAYER-OWNER`; P2 owns detection runtime and only borrows the affected player.
- **Lifetime / ownership:** callback receives an affected player reference/identity valid during the call only; it must not be retained.
- **Input:** a transition from alive/eligible to dead/fallen, with the single active player's identity and cause. For a pit, P2 triggers when the active player's world-position Y is greater than `worldExtent.bottom + TILE_SIZE` (one configured map cell below the logical 15-row world).
- **Output:** one queued death event per affected player occurrence.
- **Call direction:** P2 callback → P1 queues session mutation and transition.
- **Timing:** after movement/contact detection and before cleanup. P2 filters that player from later same-frame interactions and emits at most once for the death occurrence; P1 applies destructive state changes only after `LevelManager::update` returns. P2-owned object cleanup uses its separate off-world policy, not this player-death threshold contract.
- **Current interface / missing behavior:** P2 pit, Lava, and harmful-contact paths emit the affected player once; P1 queues and flushes that typed event after level update and clears the callback on teardown. P4 lives storage plus P1 reset/DeathMenu/Game Over transitions remain.
- **Failure behavior:** duplicate contacts in one frame do not decrement lives repeatedly; a callback must not fire when a powered player merely downgrades and remains alive. A fatal event spends exactly one 1P session life; a nonfatal power downgrade emits no death event.
- **Status:** `IMPLEMENTED_PENDING_DOWNSTREAM_ACCEPTANCE`; 2P is out of scope for this release.
- **Dependent tasks:** `P2-PIT-001`, `P2-LAVA-001`, `P2-CONTACT-ENEMY-001`, `P1-DEATH-001`, `P3-PLAYER-RESET-001`, `P4-PERSISTENCE-001`.

### CON-P1-P2-COMPLETION — exactly-once level completion

- **Provider / implementation owner:** P2 WinFlag/runtime.
- **Consumer:** P1 `GameState`, win flow, and progression mediator.
- **Runtime owner:** P2 owns the active WinFlag; P1 owns transition sequencing; P4 UserData owns stored progression values.
- **Lifetime / ownership:** callback captures no owning raw pointer and is removed with the active level/state.
- **Input:** first valid player activation of the correctly anchored flag.
- **Output:** one completion event for the current stage identity.
- **Call direction:** P2 callback → P1 queues win/progression transition.
- **Timing:** during level update; state replacement occurs only after update returns. A valid contact starts the finite flag animation, and completion is emitted once after the animation finishes; a bounded animation failure path must not stall progression forever.
- **Current interface / missing behavior:** P2's one-shot completion callback is bound by P1 and queues the validated current stage after update. P4 progression plus P1 WinMenu/next/final transitions remain.
- **Failure behavior:** repeated overlap cannot duplicate score/unlock/save; invalid/failed stage cannot complete.
- **Status:** `IMPLEMENTED_PENDING_DOWNSTREAM_ACCEPTANCE`.
- **Dependent tasks:** `P2-WINFLAG-001`, `P1-GAME-001`, `P1-WIN-001`, `P4-PERSISTENCE-001`.

## P1 ↔ P3 contracts

### CON-P1-P3-PLAYER-OWNERSHIP — selected-player owner and borrowed views

- **Provider / implementation owner:** P3 provides concrete player/factory types; P1/GameState creates and holds the production owner approved by `DEC-PLAYER-OWNER`.
- **Consumer:** P1 states and P2 LevelManager borrowed-player API.
- **Runtime owner:** P1 `GameState`/session owns one `std::unique_ptr<PlayerManager>` for the current 1P session.
- **Lifetime / ownership:** player addresses must stay stable while P2 holds raw non-owning views; P2 clears/rebinds views before the approved session owner destroys/replaces owners.
- **Input:** phase 1 is 1P selection, player type, and the approved 1P key mapping. Map spawn data is not a P3 factory input.
- **Output:** stable owner collection plus temporary `std::vector<PlayerManager*>` view.
- **Call direction:** the approved session owner requests P3 factory creation and passes borrowed views to P2; after a successful load, P2 applies actor-layer spawn positions to those borrowed players.
- **Timing:** construct stable player owners before active-level load; apply spawn positions during successful P2 load/reload; rebind views on replacement; destroy only after active-level update/destruction is safe.
- **Current interface / missing behavior:** P3 factory can return a player `unique_ptr`; injected P1 `GameState` is the tested runtime owner and clears P2's borrowed view before level-then-player destruction. The production selection state still does not construct/transfer that owner.
- **Failure behavior:** unknown player type fails explicitly; no null entry is passed to P2; the single-player construction rolls back safely.
- **Status:** `PARTIAL`.
- **Dependent tasks:** `P1-SELECT-001`, `P1-GAME-001`, `P3-FACTORY-001`, `P3-PLAYER-RESET-001`.

### CON-P1-P3-PLAYER-RESET — reset/reconstruct after death or restart

- **Provider / implementation owner:** P3 supplies reset semantics/API; P1 chooses transition timing, while the object owner remains whichever owner `DEC-PLAYER-OWNER` approves.
- **Consumer:** P1 death/restart flow; P2 receives the rebound player view.
- **Runtime owner:** unchanged from `CON-P1-P3-PLAYER-OWNERSHIP`.
- **Lifetime / ownership:** reset must not invalidate a borrowed address unless P1 first coordinates detachment from P2 and the approved session owner then replaces/rebinds it.
- **Input:** affected player identity, approved reset mode, map spawn position, persisted session fields.
- **Output:** valid player state with defined health/power/motion/invulnerability/action and position.
- **Call direction:** P1 coordinates the transition; the approved session owner invokes P3 API/reconstruction, then P1 reinjects the resulting borrowed view into P2.
- **Timing:** outside active level traversal; after lives policy is applied and before resumed update.
- **Current interface / missing behavior:** no explicit reset contract; current player methods cover only parts of state.
- **Failure behavior:** failed reset/reload goes to a safe menu/error path; never resume with stale pointers or partially reset movement/buffs. Fatal reset returns the player to the stage start; nonfatal power loss resets only power state and preserves position.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`; 2P reset behavior is out of scope for this release.
- **Dependent tasks:** `P3-PLAYER-RESET-001`, `P1-DEATH-001`, `P1-PAUSE-001`, `P2-PIT-001`.

## P2 ↔ P3 contracts

### CON-P2-P3-ENTITY-CONSTRUCTION — map spawn to owned enemy/item

- **Provider / implementation owner:** P3 `EntityFactory` provides concrete enemy/item construction; P2 provides parsed spawn data and runtime registration.
- **Consumer:** P2 `LevelManager::constructSpawn` and load transaction.
- **Runtime owner:** every successfully constructed map enemy/item transfers immediately as one `std::unique_ptr<GameObject>` to P2. Selected players are explicitly excluded and follow `CON-P1-P3-PLAYER-OWNERSHIP`.
- **Lifetime / ownership:** P3 retains no owner or borrowed pointer after return; P2 registers typed non-owning views only after accepting the unique owner.
- **Input:** validated map object type/name and world position from P2 `MapSpawnInfo`; variant data remains under the P2↔P4 variant contract. The factory does not read map pixels or paths.
- **Output:** a non-null, render-initialized `unique_ptr` of the requested type, or an explicit construction failure.
- **Call direction:** P2 parser/load → P3 factory → move-only return to P2 owner/registry.
- **Timing:** during the load transaction before the first update/render; set spawn position before typed-view registration.
- **Current interface / missing behavior:** unique_ptr enemy/item factories exist and P2 calls them, but invalid-type/failure propagation and render-initialization policy are not explicit.
- **Failure behavior:** unknown type or null construction is diagnosed and makes the affected load fail safely; never silently omit a required object or register a null/raw owner.
- **Status:** `PARTIAL`.
- **Dependent tasks:** `P3-FACTORY-001`, `P2-LOAD-001`, `P2-NINE-LEVEL-001`, all P3 visual initialization tasks.

### CON-P2-P3-PLAYER-ENEMY — contact classification and semantic result

- **Provider / implementation owner:** P3 provides enemy-specific semantic methods/state; P2 detects and orchestrates contacts.
- **Consumer:** P2 runtime; P1/P4 receive downstream score/death outcomes.
- **Runtime owner:** the owner approved by `DEC-PLAYER-OWNER` owns players (recommended P1); P2 owns enemies through `unique_ptr`; neither contact API transfers ownership.
- **Lifetime / ownership:** raw references are valid only during the collision call; inactive/dead enemies are removed in P2 cleanup after traversal.
- **Input:** player/enemy references, overlap/previous bounds or resolved contact side, relative motion.
- **Output:** deterministic result such as stomped, harmful, shell-kicked, enemy-defeated, player-damaged, bounce, score value.
- **Call direction:** P2 classifies → invokes P3 behavior → applies physical response → emits P1 event.
- **Timing:** after movement/block resolution, before inactive cleanup; at most one semantic resolution per contact pair/frame.
- **Current interface / missing behavior:** P2 consumes P3's common typed result after relative-motion AABB classification; P1 binds its score/death callbacks into the post-update mediator. P4 data consumers and visual/gameplay acceptance remain.
- **Failure behavior:** ambiguous contact uses a documented harmful/default result, never both stomp and damage; invulnerability and dead/inactive filters apply first.
- **Status:** `IMPLEMENTED_PENDING_DOWNSTREAM_ACCEPTANCE`.
- **Dependent tasks:** `P2-CONTACT-ENEMY-001`, `P3-GOOMBA-001`, `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001`, `P1-EVENT-001`.

### CON-P2-P3-PLAYER-ITEM — world motion and exactly-once collection

- **Provider / implementation owner:** P3 provides item behavior/effect; P2 provides movement/contact orchestration and runtime ownership.
- **Consumer:** P2 runtime; P1/P4 consume semantic outcome.
- **Runtime owner:** P2 owns each map/spawned item `unique_ptr`; the owner approved by `DEC-PLAYER-OWNER` owns the player; P3 methods retain neither pointer.
- **Lifetime / ownership:** item lives until P3 marks it inactive and P2 cleanup removes the owner/view.
- **Input:** active item, eligible player, resolved overlap, and block/world contacts for moving items. Before a Mushroom/fire growth, P2 evaluates the proposed 64×128 player bounds anchored at the current feet against active solid Blocks and supplies a clearance result.
- **Output:** exactly one item effect/outcome, inactive transition, and optional score/coin/power event. A denied growth clearance returns `not consumed/not applied`: player size/power and item active state remain unchanged so no player is enlarged into a solid Block.
- **Call direction:** P2 moves/resolves item → detects player overlap → invokes P3 collection → queues event/cleanup.
- **Timing:** after movement/block resolution and before cleanup; newly spawned payload joins via pending queue and is not double-updated in its birth traversal.
- **Current interface / missing behavior:** P2 resolves item/Block motion, safe-growth clearance, typed collection, form application, delta callbacks and deferred cleanup; P1 binds score/coin/lives deltas into the post-update mediator. P4 data/HUD consumers and visual/gameplay acceptance remain.
- **Failure behavior:** null/inactive/collected item is ignored; an effect failure cannot remove the item silently without a recorded outcome.
- **Status:** `IMPLEMENTED_PENDING_DOWNSTREAM_ACCEPTANCE`.
- **Dependent tasks:** `P2-CONTACT-ITEM-001`, `P3-PLAYER-STATE-001`, `P3-ITEM-001`, `P3-FIRE-001`, `P4-QUESTION-BLOCK-001`, `P4-PAYLOAD-BLOCK-001`.

### CON-P2-P3-ENEMY-REMOVAL — dead and off-world owned entity cleanup

- **Provider / implementation owner:** P3 exposes dead/inactive semantic state; P2 defines runtime participation and removal thresholds.
- **Consumer:** P2 owner/view cleanup.
- **Runtime owner:** P2 sole `unique_ptr` owner.
- **Lifetime / ownership:** typed raw views are rebuilt/filtered after deletion; no subsystem retains an enemy pointer across cleanup.
- **Input:** P3 dead/inactive state or P2 map-derived off-world test.
- **Output:** exclusion from later same-frame interactions followed by owner deletion at the cleanup barrier.
- **Call direction:** P3 state/P2 threshold → P2 filters participation → P2 cleanup.
- **Timing:** deactivate immediately; delete after active traversal; rebuild views before the next frame.
- **Current interface / missing behavior:** dead/inactive and map-derived off-world enemy/item/Fireball removal plus typed view rebuild pass; long-session gameplay acceptance remains.
- **Failure behavior:** duplicate removal is harmless; cleanup never dereferences a removed object or erases while iterating active owners.
- **Status:** `IMPLEMENTED_PENDING_GAMEPLAY_ACCEPTANCE` with verified ownership foundation.
- **Dependent tasks:** `P2-CLEANUP-001` and all P3 enemy/item tasks; preserves `BASE-P2-OWNERSHIP-001` and `BASE-P2-LIFECYCLE-001`. P2-owned Cannon/Bullet/Rocket culling is an internal P2 cleanup use, not a P3 removal dependency.

### CON-P2-P3-PROJECTILE-SPAWN — boss/fire attack request and runtime adoption

- **Provider / implementation owner:** P3 owns boss/FireBuff attack semantics and any P3-specific attack request/type; P2 owns Cannon/Bullet/Rocket implementation plus world-safe queueing, construction/adoption, collision classification, and cleanup. P3 owns player/enemy target-state reaction methods; P1 owns session-level score/death sequencing.
- **Consumer:** P2 `LevelManager`/projectile runtime consumes attack requests; P3 player/enemy targets consume classified hits; P1 consumes the resulting score/death outcome where applicable.
- **Runtime owner:** after construction, P2 is the sole `unique_ptr` runtime owner. P3 retains no owner and no projectile/target raw pointer.
- **Lifetime / ownership:** the request is value or move-only data valid through the handoff; any target is represented by stable identity/position resolution, never a retained player pointer. Pending objects join P2 typed views only after active traversal.
- **Input:** approved attack kind, origin, direction/velocity or stable target identity, source identity, gameplay parameters, and on contact a call-local target identity/contact classification; no state/menu/persistence pointer.
- **Output:** one queued P2-owned runtime projectile/attack object or an explicit rejected request; on collision, one typed P3 target-state result and at most one P1-facing score/death outcome.
- **Call direction:** P3 update/input → P2 spawn callback/queue → P2 constructs or immediately adopts one unique owner → P2 classifies projectile contact → P3 target-state method returns a semantic result → P2 deactivates/queues cleanup and forwards any P1 outcome.
- **Timing:** request at most once per cooldown/action edge; flush after current traversal so the birth frame is not double-updated; collide starting at the agreed next participation point; accept at most one semantic hit per projectile/target pair per frame, mark a consumed projectile inactive before later pair traversal, and delete only at the owner cleanup barrier.
- **Current interface / missing behavior:** P2 validates typed requests, consumes Boss requests through base `Enemy`, and queues damage-carrying Bullet/Fireball owners without a birth-frame update; P1 fire-input binding and production visual/gameplay acceptance remain.
- **Failure behavior:** invalid kind/parameters or failed construction is diagnosed and creates no partial/raw owner; a rejected request does not consume cooldown unless the P3 task explicitly tests that policy. Duplicate pairs/events are suppressed, an already-inactive projectile cannot award score or death twice, and a failed target reaction produces no partial session mutation.
- **Status:** `IMPLEMENTED_PENDING_DOWNSTREAM_ACCEPTANCE`.
- **Dependent tasks:** `P2-PROJECTILE-001`, `P3-BOSS-001`, `P3-FIRE-001`; preserves `BASE-P2-OWNERSHIP-001`, `BASE-P2-LIFECYCLE-001`, and `BASE-P2-ROCKET-001`.

## P2 ↔ P4 contracts

### CON-P2-P4-BLOCK-COLLISION — collision context and activation eligibility

- **Provider / implementation owner:** P2 provides collision side/actor context; P4 implements block reaction.
- **Consumer:** P2 PhysicsEngine and P4 Block subclasses; P3 player power state is read-only input where approved.
- **Runtime owner:** P2 owns Block `unique_ptr`; P4 owns class implementation only.
- **Lifetime / ownership:** call-local actor/block references; spawned objects use the separate handoff contract.
- **Input:** collision side, actor category/identity, relevant power state, relative motion.
- **Output:** physical response plus semantic block result: no activation, bounce, empty, break, or payload spawn request.
- **Call direction:** P2 resolves contact → P4 reaction → P2 applies lifecycle/spawn result.
- **Timing:** once for a qualifying collision; inactive block is excluded from all later contacts in the same frame.
- **Current interface / missing behavior:** `reactToCollision(int)` and lifecycle work for current normal Brick; actor/power context is absent.
- **Failure behavior:** unknown actor is non-activating but still receives safe solid collision; no unsafe cast or default break.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION` under the approved actor/power eligibility rule; normal Brick baseline stays verified.
- **Dependent tasks:** `P4-PAYLOAD-BLOCK-001`, `P4-CLOUD-001`, `P2-CONTACT-ENEMY-001`, `P2-ENV-001`.

### CON-P2-P4-BLOCK-LIFECYCLE — deactivate now, delete after traversal

- **Provider / implementation owner:** P4 exposes `isExist()` and changes lifecycle; P2 consumes and owns cleanup.
- **Consumer:** P2 PhysicsEngine/LevelManager.
- **Runtime owner:** P2 sole `unique_ptr` owner.
- **Lifetime / ownership:** P4 never deletes its runtime owner; P2 deletes only at the cleanup barrier and rebuilds views.
- **Input:** P4 active/existence flag after a reaction/update.
- **Output:** immediate participation exclusion and deferred owner deletion.
- **Call direction:** P4 marks inactive → P2 filters → P2 removes.
- **Timing:** same-frame filter before later actors; deletion after traversal.
- **Current interface / missing behavior:** implemented and covered by current runtime checks.
- **Failure behavior:** repeated deactivation is idempotent; null and inactive entries are ignored.
- **Status:** `VERIFIED_BASELINE`.
- **Dependent tasks:** all block extension tasks; `BASE-P2-LIFECYCLE-001`, `BASE-P4-BRICK-001`, `BASE-P4-FRAGMENT-001`.

### CON-P2-P4-SPAWN-HANDOFF — immediate adoption of block payloads/fragments

- **Provider / implementation owner:** P4 creates block-result objects; P2 provides adoption callback/queue. P3 implements item payload classes.
- **Consumer:** P2 LevelManager owner system.
- **Runtime owner:** ownership transfers immediately to one P2 `unique_ptr`; provider retains no owning pointer.
- **Lifetime / ownership:** raw callback parameter is transfer-only and must be wrapped exactly once before returning; typed views are registered when the pending queue is flushed.
- **Input:** one fresh heap object per callback with initialized position/state.
- **Output:** pending owned runtime object; no same-traversal double update.
- **Call direction:** P4 callback → P2 immediate adopt → post-traversal register.
- **Timing:** during block collision/update; flush after active traversal.
- **Current interface / missing behavior:** structurally verified for BrickFragments and current question-block payloads; payload behavior/variants and automated question-block checks remain.
- **Failure behavior:** null is rejected; duplicate pointer is never adopted; exception/failure path must not leak.
- **Status:** `PARTIAL` with verified foundation.
- **Dependent tasks:** `P4-QUESTION-BLOCK-001`, `P4-PAYLOAD-BLOCK-001`, `P2-VARIANT-WIRE-001`, `P3-ITEM-001`; preserves baseline tasks.

### CON-P2-P4-BLOCK-VARIANTS — parser metadata to concrete behavior

- **Provider / implementation owner:** P2 preserves parser `MapSpawnInfo` type/variant; P4 defines block variant API/behavior; P3 supplies payload types.
- **Consumer:** P2 `constructSpawn` and P4 constructors/factories.
- **Runtime owner:** constructed object becomes P2-owned.
- **Lifetime / ownership:** variant is copied at construction; it does not reference map-image memory.
- **Input:** map object type plus validated supported variant. CloudPlatform is excluded from the current release.
- **Output:** correct concrete block configuration and payload behavior.
- **Call direction:** P2 parser → P2 construction mapping → P4 behavior object.
- **Timing:** once at load/construction before first update/render.
- **Current interface / missing behavior:** parser recognizes variants but `LevelManager` discards them; CloudPlatform mapping is intentionally deferred and must not be wired in this release.
- **Failure behavior:** unknown variant produces a diagnostic and safe default/rejected load as agreed; never silently emits the wrong reward.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION` for supported payload variants; CloudPlatform remains `DEFERRED` and is not a dependency for this release.
- **Dependent tasks:** `P2-VARIANT-WIRE-001`, `P4-PAYLOAD-BLOCK-001`, `P4-CLOUD-001`, `P3-ITEM-001`.

### CON-P2-P4-BLOCK-VISUAL-SIZE — 64×64 hitbox and sprite alignment

- **Provider / implementation owner:** P4 owns Block visual invariant/API; P2 uses it during construction.
- **Consumer:** all block subclasses and P2 runtime/rendering.
- **Runtime owner:** P2 owns objects; texture/sprite resources remain under P4 class/resource policy.
- **Lifetime / ownership:** scale/size state remains with the Block; texture lifetime outlives sprite/animation references.
- **Input:** logical tile size 64×64 and source atlas frame dimensions, currently commonly 16×16.
- **Output:** one aligned 64×64 collision box and rendered footprint at the same world position.
- **Call direction:** P2 construction invokes P4's single size API; P4 maintains alignment after frame switches.
- **Timing:** immediately after construction and before registration/first render.
- **Current interface / missing behavior:** `setSizeBlock` can scale, but P2 calls generic `setSize`, leaving art at half the physics tile.
- **Failure behavior:** missing/zero texture bounds is reported and uses a visible diagnostic/fallback; never divide by zero or claim a false visual pass.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`.
- **Dependent tasks:** `P4-BLOCK-SIZE-001`, `P2-ENV-001`, `P2-NINE-LEVEL-001`.

## P3 ↔ P4 contract

### CON-P3-P4-ANIMATION — entity-facing animation component usage

- **Provider / implementation owner:** P4 implements `AnimationComponent`; P3 owns entity animation registration and semantic state selection.
- **Consumer:** P3 players, enemies, items, and bosses.
- **Runtime owner:** entity owns its animation component; referenced texture/sprite lifetime must exceed component use.
- **Lifetime / ownership:** no component may retain a reference to a destroyed/local texture or sprite; copying is prohibited if references would alias unsafely.
- **Input:** stable texture/sprite, animation key, SFML 3.1 `IntRect` frames, interval/loop policy, dt.
- **Output:** selected frame applied to sprite and advanced deterministically.
- **Call direction:** P3 creates/registers/chooses semantic keys; P4 component advances/applies frames.
- **Timing:** register before first update; select on semantic transition; update once per entity frame; render through entity.
- **Current interface / missing behavior:** component exists and compiles; P3 never constructs/registers it; empty/missing-key behavior needs an explicit safe rule and checks.
- **Failure behavior:** missing key/frame leaves a safe current frame and reports/asserts in a testable way; no out-of-range access.
- **Status:** `PARTIAL`.
- **Dependent tasks:** `P4-ANIMATION-001`, all P3 visual tasks.

## P1 ↔ P4 contracts

### CON-P1-P4-GUI — reusable controls and state-navigation intent

- **Provider / implementation owner:** P4 implements reusable GUI controls, rendering, focus, hit testing, and resource-failure behavior; P1 defines each state's allowed action vocabulary and owns navigation/transition logic.
- **Consumer:** P1 Main/Name/Settings, character/world/level selection, Death, Win, and later Pause state implementations.
- **Runtime owner:** the active P1 state owns its GUI control/container instances for that state's lifetime; P4 owns the class implementation and shared UI resource service, never the P1 state stack.
- **Lifetime / ownership:** controls and borrowed font/texture resources outlive event/update/render calls; returned actions are values, not retained state pointers or callbacks that can outlive the state.
- **Input:** SFML 3.1 event values, state-defined labels/action IDs, enabled/disabled choices, focus order, text-entry constraints, layout rectangle, and elapsed time where needed.
- **Output:** rendered control state plus zero or one value action/text-edit result for P1 to validate and route.
- **Call direction:** P1 configures/owns controls and forwards input → P4 GUI updates focus/text/hit state and returns an action → P1 queues any state transition after the current handler/update returns.
- **Timing:** consume each input event once; emit at most one activation per key/button edge; resize/layout before render; never replace a P1 state from inside a P4 control callback.
- **Current interface / missing behavior:** `GUI` is empty and P1 menu states are shells; no stable action, focus, text-entry, or failure surface exists.
- **Failure behavior:** invalid action/configuration is rejected without transition; missing font/texture reports a stable error and uses a readable fallback or lets P1 show a safe error state; focus loss never activates a stale control.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`.
- **Dependent tasks:** `P4-GUI-001`, `P1-MENU-001`, `P1-SELECT-001`, `P1-DEATH-001`, `P1-WIN-001`, and later `P1-PAUSE-001`/`P4-LEADERBOARD-001` integration.

### CON-P1-P4-SCORE-COINS-LIVES — authoritative session counters

- **Provider / implementation owner:** P1 mediator/session applies gameplay outcomes; P4 `UserData` stores and validates counters.
- **Consumer:** P1 transitions and P4 HUD/persistence; P2/P3 produce semantic outcomes but do not mutate storage directly.
- **Runtime owner:** P4 UserData owns values; P1 owns sequencing/dispatch.
- **Lifetime / ownership:** value updates outlive individual level objects as approved by persistence policy.
- **Input:** typed deltas/events with player/session identity and reason; never raw gameplay-object ownership.
- **Output:** validated new score/coins/lives snapshot and optional derived extra-life event.
- **Call direction:** P2/P3 outcome → P1 mediator → P4 UserData → P1/P4 observers.
- **Timing:** once per semantic interaction, after collision resolution and before HUD snapshot for the next render.
- **Current interface / missing behavior:** in-memory counters exist; mediator and gameplay propagation are absent.
- **Failure behavior:** reject overflow/invalid negative state; duplicate event IDs or one-shot guards prevent repeated award/decrement.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION` for the single-player lives semantics.
- **Dependent tasks:** `P1-EVENT-001`, `P1-DEATH-001`, `P2-CONTACT-ITEM-001`, `P2-CONTACT-ENEMY-001`, `P3-ITEM-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`.

### CON-P1-P4-HUD-DATA — immutable per-frame presentation snapshot

- **Provider / implementation owner:** P1 assembles gameplay/session snapshot; P4 renders `LevelGUI`.
- **Consumer:** P4 HUD only.
- **Runtime owner:** P1/P4 retain their own value snapshot; HUD owns no gameplay objects.
- **Lifetime / ownership:** snapshot is copied or remains valid for the render call only; no stored player/level raw pointers.
- **Input:** score, coins, lives/health according to decision, stage ID, time, and optional player-specific fields.
- **Output:** screen-space HUD rendering and no gameplay mutation.
- **Call direction:** P1 update → P4 HUD update(snapshot) → P1 render calls HUD under UI view.
- **Timing:** update after gameplay/events; render after restoring screen-space view.
- **Current interface / missing behavior:** `LevelGUI` is empty; no snapshot/data bridge exists.
- **Failure behavior:** missing optional data displays a stable placeholder; invalid stage/counter does not crash or write session data.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`.
- **Dependent tasks:** `P1-GAME-001`, `P1-EVENT-001`, `P4-HUD-001`.

### CON-P1-P4-AUDIO-EVENTS — semantic cue and music requests

- **Provider / implementation owner:** P1 mediator/state publishes semantic requests; P4 AudioSystem owns playback/resources.
- **Consumer:** P4 AudioSystem.
- **Runtime owner:** P4 owns sound buffers/music/instances; publishers own no audio handles.
- **Lifetime / ownership:** requests are values; resources outlive active playback.
- **Input:** typed cue/music ID, volume/category, optional stop/fade instruction; no raw entity pointer.
- **Output:** best-effort playback state.
- **Call direction:** P1/P2/P3 semantic outcome → P1 event bridge → P4 AudioSystem.
- **Timing:** event once per outcome; music changes on state boundary, not every frame.
- **Current interface / missing behavior:** AudioSystem and mediator are empty.
- **Failure behavior:** missing audio file logs/records failure and continues silently; gameplay never blocks or crashes on audio.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION`.
- **Dependent tasks:** `P1-EVENT-001`, `P1-MENU-001`, `P4-AUDIO-001`, later integration scenarios.

### CON-P1-P4-PERSISTENCE — save/load request and durable result

- **Provider / implementation owner:** P4 implements serialization/storage and maps P1 semantic events to the human-approved save timing; P1 publishes those events and reacts to explicit load/save results.
- **Consumer:** P1 menu/death/win/session flow and P4 leaderboard/progression.
- **Runtime owner:** P4 UserData owns in-memory model; durable file belongs to the application profile, not a level object.
- **Lifetime / ownership:** serialized value data contains no pointers; schema has a version.
- **Input:** P4-owned validated UserData/profile state plus a P1 semantic session event or explicit load/save request; never a gameplay-object pointer.
- **Output:** explicit success/failure plus diagnostic and loaded validated values.
- **Call direction:** P1 semantic event/request → P4 persistence transaction → P1 consumes explicit result where UI/state flow needs it.
- **Timing:** load before selection/progression display; save at approved completion/death/exit points from `DEC-PERSISTENCE`.
- **Current interface / missing behavior:** UserData is in-memory only; `SaveData` contains only `.gitkeep`.
- **Failure behavior:** missing file creates safe defaults; corrupt/unsupported file is preserved or quarantined and reported; write uses recoverable replacement so previous valid data is not lost.
- **Status:** `DEFINED_PENDING_IMPLEMENTATION` under the approved versioned profile and writable-location policy. `DEC-ASSET-ROOT` governs read-only packaged/runtime assets only.
- **Dependent tasks:** `P4-PERSISTENCE-001`, `P4-LEADERBOARD-001`, `P1-EVENT-001`, later `P1-MENU-001`/`P1-SELECT-001` restoration/result handling, and `P4-PACKAGE-001`. P1 death/win merely emit stable semantic outcomes and are not blocked by durable I/O.

### CON-P1-P4-PROGRESSION — exactly nine-stage unlock model

- **Provider / implementation owner:** P4 stores validated progression; P1 owns selection and transition rules.
- **Consumer:** P1 world/level menus and win flow.
- **Runtime owner:** P4 UserData owns current/unlocked values; P1 owns current state transition.
- **Lifetime / ownership:** in-memory progression values live in P4 UserData for the session; durable survival across launches is separately governed by `CON-P1-P4-PERSISTENCE`. Values are IDs, never State pointers.
- **Input:** current stage ID and exactly-once completion outcome.
- **Output:** unlocked set/current selection constrained to W1_LV1 through W3_LV3; final completion indication at W3_LV3.
- **Call direction:** P1 completion → P4 update/validate → P1 menus read snapshot.
- **Timing:** update in memory after the completion callback and before WinMenu/next selection; persist only at the separately approved durable-save point.
- **Current interface / missing behavior:** UserData has current/unlocked fields but no integrated nine-stage rule or state flow.
- **Failure behavior:** invalid IDs are rejected/clamped to safe default; completion cannot unlock outside the nine-stage table or unlock twice with duplicate rewards.
- **Status:** `PARTIAL`: the in-memory nine-stage provider/consumer contract is defined and is not blocked by `DEC-PERSISTENCE`; implementation and its focused nine-stage/idempotence check must land before P1 selection/win integration. Durable restore/save remains `BLOCKED_DECISION` only in `CON-P1-P4-PERSISTENCE`.
- **Dependent tasks:** `P1-SELECT-001`, `P1-WIN-001`, `P4-PERSISTENCE-001`, all `LV-*` entries.

## Contract acceptance rule

A contract is integration-complete only when:

1. provider and consumer headers agree without cross-owner private assumptions;
2. ownership/lifetime behavior is executable under success and failure paths;
3. call order is asserted in the smallest runnable integration check;
4. SFML 3.1 compile checks pass for both sides;
5. duplicate-event, failed-load, and cleanup failure behavior is covered where applicable;
6. both owner task cards record `INTEGRATION_DONE=YES` and the relevant runtime/visual/gameplay gates.

Do not edit a provider's file merely because a consumer needs more data. File a cross-owner request using `04_USAGE_AND_PROGRESS_WORKFLOW.md` and attach the smallest failing contract check.

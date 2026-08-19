# Human Decision Register

This register contains exactly the eight product/ownership choices that source evidence and `PLAN.md` cannot settle. Every decision remains `OPEN`. A recommended default is a proposal that minimizes architectural risk; it is not authorization to implement it.

## Decision workflow

1. The named owners prepare only the evidence requested by the card. They may add a short prototype result or contract sketch, but they must not merge an assumption as though it were approval.
2. The team lead/product approver selects an option or writes a precise approved variant. The approver fills the card's final-approval field with name, approved option, ISO date, and rationale or meeting link.
3. The planning coordinator changes that card from `OPEN` only after explicit approval, copies the chosen rule into every affected contract, and updates each affected task on `01_MASTER_TASK_BOARD.md` from `BLOCKED` to `READY` only when all of its other blockers are also clear.
4. The relevant owner adds the decision ID to the implementation commit and tests the selected 1P/2P, lifetime, failure, and persistence behavior. A commit, branch, code comment, existing implementation, silence, or an AI/human working assumption is **not** approval.
5. If later evidence requires reversal, reopen the same stable decision ID, record the superseding approval, and re-block affected tasks/contracts. Do not create a duplicate decision card.

Approval authority should be named by the team before review. P1 owns runtime/state-policy integration, P2 owns world detection/runtime mechanics, P3 owns player/entity behavior APIs, and P4 owns presentation/data/release implementation; none may decide a cross-owner product rule alone.

## Decision cards

### `DEC-MULTIPLAYER-DEATH`

- **Decision ID:** `DEC-MULTIPLAYER-DEATH`
- **Status:** `OPEN`
- **Question:** In 1P and 2P play, are lives shared or per-player, how are simultaneous deaths counted, and when does Game Over occur?
- **Why it matters:** P2 emits an affected-player death identity, but P1 must translate it into one deterministic session transaction and P4 must store/display the same life model. Without a decision, two callbacks in one frame can double-charge lives or trigger conflicting state transitions.
- **Affected task IDs:** `P1-EVENT-001`, `P1-DEATH-001`, `P1-GAME-001`, `P2-PIT-001`, `P2-CONTACT-ENEMY-001`, `P2-LAVA-001`, `P2-PROJECTILE-001`, `P3-PLAYER-RESET-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`.
- **Owner(s):** Human product/team lead approves; P1 proposes the session transaction; P2 confirms event timing/identity; P3 confirms reset feasibility; P4 confirms data/HUD schema.
- **Options:**

  1. **Shared session lives (recommended default):** one scalar pool. In 1P, an accepted death transaction spends one life. In 2P, any player death starts one team-death transaction; simultaneous same-frame deaths are coalesced and spend one shared life. Game Over occurs when the shared pool reaches zero. Placement/reset is governed separately by `DEC-RESPAWN`.
  2. **Separate lives per player:** each player has an independent counter; a death spends only that player's life. Game Over occurs only when no active player has a life remaining; the zero-life player's spectate/removal behavior must also be specified.
  3. **Team-wipe lives:** individual deaths do not spend a life while another player remains active; one life is spent only when all active players are dead/fallen. In 1P this reduces to normal one-death accounting.

- **Consequence of each option:**

  1. Reuses current scalar `UserData` lives and produces the smallest safe event/persistence/HUD change, but any-player failure affects the team and requires a same-frame coalescing rule.
  2. Gives independent 2P consequences, but expands UserData, HUD, save migration, selection identity, reconnect/restart rules, and testing. It cannot be represented by the current single lives value without schema work.
  3. Keeps co-op flowing longest, but requires a downed-player state and reliable all-dead detection; it is the largest gameplay-policy addition and can leave one player unable to participate for long periods.

- **Recommended default:** Option 1, shared session lives with one coalesced team-death transaction. It matches the current scalar data model and gives P1 one deferred transition after P2 traversal. This recommendation does not decide the card.
- **Must decide before:** `P1-DEATH-001`, `P2-PIT-001`, and `P3-PLAYER-RESET-001` enter implementation; before P4 freezes HUD/persistence fields.
- **Evidence needed:** current `UserData` life shape; P2 callback timing for one/two players; a truth table for 1P death, one 2P death, simultaneous 2P death, and zero-life state; UX review of team impact.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-PLAYER-OWNER`

- **Decision ID:** `DEC-PLAYER-OWNER`
- **Status:** `OPEN`
- **Question:** Which production object owns selected Mario/Luigi instances, and which systems may only borrow them?
- **Why it matters:** Current P2 runtime deliberately stores non-owning `PlayerManager*` views, while no production GameState/session exists. Selecting the wrong owner can produce dangling players across load/state transitions or regress the established `unique_ptr` architecture.
- **Affected task IDs:** `P1-SELECT-001`, `P1-GAME-001`, `P1-DEATH-001`, `P2-LOAD-001`, `P2-PIT-001`, `P2-CONTACT-ENEMY-001`, `P2-CONTACT-ITEM-001`, `P2-PROJECTILE-001`, `P3-PLAYER-RESET-001`, `P3-FACTORY-001`.
- **Owner(s):** Human/team lead approves; P1 owns session/state design; P3 owns player construction/API; P2 verifies borrowing/lifetime constraints.
- **Options:**

  1. **P1 session/GameState unique ownership (recommended default):** P3 factory returns `std::unique_ptr<PlayerManager>`; P1 stores the selected 1–2 owners for the active session/state; P2 receives non-owning views before load/update and never deletes them.
  2. **P2 LevelManager ownership:** transfer players into the same P2 owner collection as map/runtime objects; P1 keeps handles only.
  3. **Long-lived global/UserData ownership:** retain live player objects outside GameState and let states/P2 borrow them across levels.

- **Consequence of each option:**

  1. Matches current P2 APIs, separates implementation/runtime ownership, supports deferred state replacement, and keeps player lifetime visible. P1 must reinject views after reconstruction/reload.
  2. Conflicts with the current audited player exception and couples menu/session reset policy to P2 map lifetime; it risks deleting selected players during level clear/reload and requires broad contract changes.
  3. Lets objects survive state changes, but mixes durable data with live SFML/gameplay resources, complicates reset and resource lifetime, and creates global-lifetime risks.

- **Recommended default:** Option 1: P1 owns a `std::vector<std::unique_ptr<PlayerManager>>` (or the smallest equivalent session owner), P3 constructs, and P2 borrows. No shared ownership is needed.
- **Must decide before:** `P1-SELECT-001`, `P1-GAME-001`, `P3-FACTORY-001`, or production stage-load wiring changes player lifetime.
- **Evidence needed:** P1 state lifetime sketch; current `LevelManager::setPlayers/load` behavior; P3 factory return types; reset/reload call sequence including callbacks and state replacement.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-BLOCK-ACTOR-ELIGIBILITY`

- **Decision ID:** `DEC-BLOCK-ACTOR-ELIGIBILITY`
- **Status:** `OPEN`
- **Question:** Which actor and player power states may activate question/payload blocks or break Bricks, and what collision context must P2 pass to P4?
- **Why it matters:** Current `reactToCollision(int)` carries only a side. Normal Brick currently breaks on a bottom hit regardless of actor context, while planned payload variants require small/big/fire/enemy distinctions. P2 owns collision orchestration; P4 owns Block reactions; P3 owns player/enemy state.
- **Affected task IDs:** `P2-CONTACT-ENEMY-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001`, `P3-PLAYER-STATE-001`, `P3-KOOPA-001`, `P4-PAYLOAD-BLOCK-001`, `P4-QUESTION-BLOCK-001`.
- **Owner(s):** Human/gameplay lead approves; P4 proposes Block rule/API; P2 proposes collision context and timing; P3 supplies stable actor/power-state values.
- **Options:**

  1. **Typed activation context (recommended default):** any active player underside hit activates Coin/Mushroom/payload blocks; only Big/Fire players break normal Bricks; Small players bump without breaking; enemies/bosses/shells do not activate or break by default unless a later explicit rule is approved. Preserve one-shot/four-fragment lifecycle even if eligibility changes.
  2. **Player-only, any power:** every player underside hit activates and breaks eligible Blocks; enemies never do. Add actor identity but not power eligibility.
  3. **Legacy side-only:** any object resolved as a bottom hit can trigger the existing reaction; no actor/power context is added.

- **Consequence of each option:**

  1. Gives explicit, extensible gameplay semantics and separates P2 detection from P4 behavior, but intentionally changes who can break a Brick and requires new small/big/fire tests around the frozen lifecycle baseline.
  2. Is simpler and close to current behavior while preventing enemy activation, but cannot represent Small-player bump-versus-break behavior.
  3. Is the smallest API diff, but cannot enforce the requested eligibility, risks enemies triggering payloads, and leaves variant behavior ambiguous; it is not recommended for final integration.

- **Recommended default:** Option 1 with a small value `BlockHitContext` or equivalent agreed API owned by P4 and populated by P2. Do not reopen normal Brick's one-shot, four-fragment, adoption, or inactive-cleanup guarantees.
- **Must decide before:** `P4-PAYLOAD-BLOCK-001` defines constructors/reactions and before `P2-VARIANT-WIRE-001` or enemy/shell-to-Block integration freezes caller behavior.
- **Evidence needed:** current normal-Brick runtime assertions; P3 power-state enum/API; desired Small/Big/Fire behavior table; whether any mapped level requires shell/enemy activation (without inspecting maps in this planning pass).
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-CLOUD-SEMANTICS`

- **Decision ID:** `DEC-CLOUD-SEMANTICS`
- **Status:** `OPEN`
- **Question:** Does a `CloudPlatform` map marker construct P4 `CloudBlock`, P2 `MovingBlock`, or a formally defined composition/distinction?
- **Why it matters:** Current parser metadata calls the object CloudPlatform, but `LevelManager` always creates P2 `MovingBlock`; P4 `CloudBlock` is separately required and empty. Leaving both meanings implicit causes ownership overlap and inconsistent collision/render behavior.
- **Affected task IDs:** `P2-ENV-001`, `P2-VARIANT-WIRE-001`, `P2-NINE-LEVEL-001`, `P4-CLOUD-001`, `P4-BLOCK-SIZE-001`.
- **Owner(s):** Human/gameplay lead approves; P2 owns parser/construction mapping and MovingBlock; P4 owns CloudBlock behavior/rendering.
- **Options:**

  1. **Distinct meanings (recommended default):** CloudPlatform marker constructs a P4 static one-way `CloudBlock`; P2 `MovingBlock` remains a different component and is created only by a separate explicit marker/configuration once approved.
  2. **Cloud marker means MovingBlock:** preserve current mapping; P4 CloudBlock becomes a visual/behavior helper only if a clear composition contract is added, or is explicitly removed from required scope by a separate plan revision.
  3. **CloudBlock owns appearance; MovingBlock owns motion via composition:** the marker creates one runtime object with a formally defined P4 Block surface and P2 motion component/strategy.

- **Consequence of each option:**

  1. Keeps implementation ownership clear and gives both required classes distinct purposes, but existing user map intent must be confirmed before a new MovingBlock marker is assigned.
  2. Minimizes current parser changes, but fails to explain the separately assigned CloudBlock unless scope is explicitly changed; hidden dual ownership is likely.
  3. Can support moving clouds cleanly, but adds the largest cross-owner API and lifecycle surface and is unnecessary unless moving-cloud gameplay is actually required.

- **Recommended default:** Option 1: Cloud marker → P4 static one-way CloudBlock; MovingBlock uses a distinct explicit construction route. This is a proposed semantic rule, not permission to alter map pixels.
- **Must decide before:** `P4-CLOUD-001` or `P2-VARIANT-WIRE-001` starts, and before `P2-NINE-LEVEL-001` claims wrapper readiness.
- **Evidence needed:** team statement of intended Cloud gameplay; current PLAN wording; any prior map-spec legend supplied by the human (not a new image-content audit); P2/P4 one-way collision feasibility.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-RESPAWN`

- **Decision ID:** `DEC-RESPAWN`
- **Status:** `OPEN`
- **Question:** After a non-Game-Over death, does the game reload the stage, reset only the affected player in place, or return to selection, and which state is retained?
- **Why it matters:** P2 can detect a map-derived fall and name the player, but it does not own lives, spawn placement, player reconstruction, or state transitions. Group4 has map actor-layer spawns but no checkpoint contract. The wrong choice can leave dangling borrowed players or reset only half the session.
- **Affected task IDs:** `P1-GAME-001`, `P1-DEATH-001`, `P1-PAUSE-001`, `P2-PIT-001`, `P2-LOAD-001`, `P3-PLAYER-RESET-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`.
- **Owner(s):** Human/gameplay lead approves; P1 owns transition/reload; P3 owns reset/reconstruction; P2 provides spawn/level data and re-borrows; P4 owns retained durable/session values.
- **Options:**

  1. **Reload current stage from actor-layer starts (recommended default):** after the approved life transaction, P1 safely destroys/reloads the active level, reconstructs/resets all active players at map spawn markers, clears transient motion/power/invulnerability, retains the decremented lives plus session score/coins, then reinjects borrowed views. In 1P this resets one player; in 2P the team restarts together.
  2. **Affected-player immediate respawn:** keep the live level/enemies/items, reset only the dead player at an approved spawn/checkpoint, and let the other 2P player continue.
  3. **Exit to DeathMenu/selection after every death:** end GameState; a later selection creates a new level/player set. No in-stage respawn occurs.

- **Consequence of each option:**

  1. Uses existing map spawn data and gives deterministic clean ownership/state, but restarts level progress for both players and needs an explicit retain/reset field list.
  2. Preserves co-op progress, but requires a safe checkpoint/occupancy/invulnerability policy not currently present and careful 2P camera behavior.
  3. Is simple and resembles the referenced Group5 flow, but makes every lost life a menu interruption and may make a multi-life counter feel redundant.

- **Recommended default:** Option 1, whole-stage reload from existing actor-layer spawn data. Retain session score/coins and the post-decrement life value; reset player health/power/buffs/motion and level-local objects. This must be reconciled with the approved `DEC-MULTIPLAYER-DEATH` option.
- **Must decide before:** P1 codes restart/death transitions, P3 freezes reset semantics, or P2 implements pit latch reset/reload behavior.
- **Evidence needed:** exact spawn API and player-count support; desired retain/reset table for score, coins, lives, power, timer, enemies/items; stage reload cost; 1P and 2P sequence diagrams.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-ASSET-ROOT`

- **Decision ID:** `DEC-ASSET-ROOT`
- **Status:** `OPEN`
- **Question:** What single runtime root resolves level images, textures, fonts, audio, save templates, and packaged assets in both development and release launches?
- **Why it matters:** Stage wrappers currently assume project-root `assets/levels`, existing Block texture code assumes `../assets`, the actual named level images are under `assets/textures`, and CMake/package behavior is not coherent. Working-directory guesses will keep load success environment-dependent.
- **Affected task IDs:** `P1-APP-001`, `P2-LOAD-001`, `P2-NINE-LEVEL-001`, `P3-PLAYER-VISUAL-001`, `P3-GOOMBA-001`, `P3-KOOPA-001`, `P3-AIR-HERISS-001`, `P3-BOSS-001`, `P3-ITEM-001`, `P4-ANIMATION-001`, `P4-AUDIO-001`, `P4-GUI-001`, `P4-PERSISTENCE-001`, `P4-PACKAGE-001`.
- **Owner(s):** Human/team lead approves repository/package layout; P4 owns package/copy policy; P1 supplies executable/application context; P2/P3/P4 consume one resolver.
- **Options:**

  1. **Executable-adjacent packaged `assets/` (recommended default):** one resolver anchors at the executable/package directory; the build/package step copies the approved `assets/` tree beside the executable. Development runs use the same generated layout, not the caller's current directory.
  2. **Repository-root assets:** all loads resolve from a required project-root working directory; launch scripts/IDE configurations must enforce it.
  3. **Configurable external root:** command-line/config/environment supplies an asset directory, with one documented fallback.

- **Consequence of each option:**

  1. Makes development and shipped behavior match and removes `../` guesses, but requires reliable CMake/package copying and a small platform-safe executable-root resolver.
  2. Is initially simple, but installed/zipped builds fail unless the repository layout and working directory travel with the executable; tests and IDE launches remain fragile.
  3. Supports custom installations and tests, but introduces configuration/error UX the project may not need and still requires a deterministic default.

- **Recommended default:** Option 1 with one coherent, reused resolver and one packaged `assets/` tree. Asset category subdirectories remain explicit; this decision does not authorize moving or editing assets until the approved layout is recorded.
- **Must decide before:** `P2-LOAD-001` changes paths, any P3/P4 resource loader is finalized, and `P4-PACKAGE-001` defines copy/install rules.
- **Evidence needed:** expected launch modes (IDE, build directory, zip/release); target OS executable-path support; current asset category inventory by filename/path only; CMake copy feasibility; save-data writable-location needs kept distinct from read-only assets.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-WINFLAG-POLISH`

- **Decision ID:** `DEC-WINFLAG-POLISH`
- **Status:** `OPEN`
- **Question:** After correcting mandatory base-anchor geometry, should completion fire immediately, wait for a flag animation, or use no additional polish?
- **Why it matters:** The white marker's base/anchor meaning is already established and must be corrected regardless of this choice. Only callback timing and optional presentation remain undecided; mixing them would incorrectly block or make anchor correctness optional.
- **Affected task IDs:** `P2-WINFLAG-001`, `P1-GAME-001`, `P1-WIN-001`, `P4-AUDIO-001`.
- **Owner(s):** Human/gameplay lead approves timing; P2 owns WinFlag activation/geometry; P1 owns deferred completion transition; P4 owns optional audio/presentation resources.
- **Options:**

  1. **Immediate logical completion with nonblocking polish (recommended default):** one completion callback fires on valid contact; optional slide/sound may continue only if P1's transition presentation explicitly keeps the scene alive.
  2. **Animation-gated completion:** contact starts a finite flag-slide sequence; P2 emits completion once the sequence ends.
  3. **Immediate minimal flag:** correct base-anchor pole/activation geometry and callback with no new slide animation.

- **Consequence of each option:**

  1. Preserves current one-shot timing and keeps progression robust, but visible animation may be cut short unless P1 deliberately delays the screen transition.
  2. Gives a clearer finish sequence, but adds timer/state/cancellation requirements and can soft-lock completion if animation/resource state fails.
  3. Is the lowest-risk required implementation and can ship without optional polish, but provides the least presentation feedback.

- **Recommended default:** Option 1. Logical progression should not depend on visual resource success. The core fix always treats the marker as a base/activation anchor and extends geometry upward; no option may retain the current top-anchor defect.
- **Must decide before:** final visual/gameplay verification of `P2-WINFLAG-001` and before P1/P4 synchronize Win transition/audio timing. It does **not** block the base-anchor source/runtime fix.
- **Evidence needed:** desired finish UX/video sketch; P1 transition timing; availability of flag/audio assets; test proving completion fires exactly once under missing/failed optional resources.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

### `DEC-PERSISTENCE`

- **Decision ID:** `DEC-PERSISTENCE`
- **Status:** `OPEN`
- **Question:** What durable schema, profile model, writable location, and save timing govern UserData, progression, settings, and leaderboard data?
- **Why it matters:** Current UserData is in-memory only; no save/load schema exists. P1 needs deterministic unlock/death/win transactions, P4 needs safe corruption handling and leaderboard rules, and package assets must not be mistaken for writable user data.
- **Affected task IDs:** `P1-EVENT-001`, `P1-MENU-001`, `P1-SELECT-001`, `P4-HUD-001`, `P4-PERSISTENCE-001`, `P4-LEADERBOARD-001`, `P4-PACKAGE-001`. P1 death/win emit stable semantic outcomes regardless; they are not blocked on durable I/O.
- **Owner(s):** Human/product lead approves visible/profile rules; P4 owns schema, safe I/O, migration, event-to-save timing, and leaderboard; P1 owns semantic event sequencing and consumes explicit load/save results; release owner confirms writable location.
- **Options:**

  1. **Versioned shared-profile text format with safe writes (recommended default):** one named profile stores schema version, validated score/coins/lives, current/unlocked position across exactly nine levels, and settings; leaderboard remains a separately versioned CSV/text dataset. Write to a temporary file, flush/close, then replace atomically where supported; retain/recover from last-good data. Save after approved level/death/settings transactions and clean exit, never every frame.
  2. **Single unversioned CSV for everything:** UserData and leaderboard share simple rows overwritten in place.
  3. **Session-only data:** no durable save; start from defaults every launch, with an optional ephemeral leaderboard.

- **Consequence of each option:**

  1. Supports migration, validation, corruption recovery, and future separate-lives fields without a new dependency, but needs explicit parsing, schema tests, writable-path selection, and transaction boundaries.
  2. Is quick to inspect, but ambiguous evolution and in-place writes risk corruption/data loss; mixed profile/leaderboard rows complicate validation.
  3. Avoids I/O failure modes, but does not satisfy PLAN-required save/load, progression, or durable leaderboard work and therefore cannot complete the approved plan without an explicit scope change.

- **Recommended default:** Option 1 using a standard-library, versioned line-oriented format (for example a magic/version header plus validated key/value records), atomic temporary-file replacement, last-good recovery, and a user-writable location separate from packaged read-only assets. Default to one shared profile; if `DEC-MULTIPLAYER-DEATH` approves separate lives, version the schema with explicit per-player values rather than overloading fields.
- **Must decide before:** `P4-PERSISTENCE-001` defines file I/O/event-to-save timing, `P4-LEADERBOARD-001` freezes its schema, and `P4-PACKAGE-001` chooses writable paths. It does not block the in-memory first-slice death/win transitions.
- **Evidence needed:** target OS writable-directory policy; exact required UserData fields/ranges; nine-level unlock table; name/leaderboard limits and tie ordering; missing/corrupt/older-version recovery scenarios; interaction with the approved lives model.
- **Final approval:** Approver: `________` · Approved option/variant: `________` · Date (YYYY-MM-DD): `________` · Rationale/link: `________`

## Register integrity check

- Decision cards present: 8.
- Decision IDs are stable and unique.
- Current status of every card: `OPEN`.
- No recommendation above is approved merely by appearing in this file.
- No ninth decision is implied by the workflow; new ambiguity must first be checked against these eight cards and the integration contracts.

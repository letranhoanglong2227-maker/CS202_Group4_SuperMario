# Person 2 implementation status — updated 2026-08-29

Branch reviewed: `person2/map-integration`
Baseline implementation commit: `419d03d`; bounds/AABB evidence is in the current P2 working tree.

This note records verified implementation evidence. Seven P2 source/runtime cards are complete; six cross-owner/integration cards remain blocked.

## Verified now

- C++20/SFML 3.1 builds succeed for `SuperMario` and `Person2RuntimeContracts`.
- CTest passes `1/1`; after the bounds/AABB/environment work, a fresh direct run reports `64 PASS / 0 FAIL`.
- The original 27 ownership/lifecycle checks remain green.
- All nine `assets/textures/LevelSketch_W*_LV*.png` files parse; the two runtime bands contain zero unknown colors.
- Map-derived dynamic world bounds are available only after successful load.
- Every active player is clamped at the dynamic left/right extent without constraining pit fall; exact-edge, overshoot, narrow-world and two loaded-width checks pass.
- AABB contact-side classification uses previous/current hitbox motion and treats an ambiguous corner as horizontal rather than a false stomp.
- Safe growth remains feet-anchored and rejects a low ceiling in a focused physics check.
- Pit death emits one fatal player callback and skips later same-frame contacts.
- Lava kills Small once and preserves Big/Fire downgrade semantics without a false death callback.
- Pipe and Trampoline collider/launch contracts pass; MovingBlock now consumes the full frame distance through linear endpoints and square-path corners without an artificial pause, while carry remains covered.
- Cannon birth-frame deferral and unique ownership pass; Bullet/Rocket lifetime, target loss, swept Block collision, off-world culling, and once-only player damage have runtime coverage.
- WinFlag uses the map marker as its base, slides for a bounded duration, and queues one completion only after the animation.

The corresponding seven cards are now `DONE` for P2 source/runtime scope. Production consumption and applicable visual/gameplay acceptance remain downstream P1/P3/P4 work.

## Non-blocking build diagnostic

The fresh build emits one warning at `src/Objects/Blocks/BrickFragment.cpp:26`: `BrickFragment::reactToCollision(int)` does not use `collidedSide`. The method must keep the `Block` override signature, but fragments are visual debris and intentionally ignore collisions, so the body is empty. This does not alter runtime behavior, fail the build, or invalidate the green BrickFragment lifecycle contract. `BrickFragment` is P4-owned/frozen baseline scope, so P2 does not change it; P4 can silence the warning by omitting the parameter name in the definition.

## P2 work still open

| Task | Remaining work | Provider/blocker |
|---|---|---|
| `P2-LOAD-001` | Replace CWD fallback with one executable-adjacent asset resolver; expose load diagnostic; make failed levels explicitly inert | P1 executable context, P4 package/copy provider |
| `P2-CLEANUP-001` | Enemy/item off-world cleanup; projectile part is complete | P3 inactive/removal APIs |
| `P2-CONTACT-ENEMY-001` | Consume the tested AABB side classifier for stomp/harm/shell behavior and once-only outcome forwarding | P3 contact result, P1 event sink |
| `P2-CONTACT-ITEM-001` | Use the tested clearance result in one-shot item collection, removal, and outcome forwarding | P3 item result, P1/P4 score/lives surface |
| `P2-VARIANT-WIRE-001` | Preserve terrain/block/payload metadata through P4/P3 construction | P4 block/style API, P3 payload types; Cloud excluded |
| `P2-NINE-LEVEL-001` | Production render/gameplay record for every wrapper | P1 flow, P3 visuals/outcomes, P4 terrain/resources/package |

## Physics decisions from the Group5 comparison

- Group5 applies variable `dt` directly and has no fixed-substep loop. Group4 therefore keeps its current clamped variable step; no unproven substep rewrite is introduced.
- Group5 classifies AABB sides from overlap depth. Group4 retains its safer axis-separated Block resolution and adapts only the useful side-classification idea, adding previous/current bounds to disambiguate corner contacts.
- Group5 also has no swept-AABB solution. Swept collision remains out of scope until a reproducible production tunneling case or projectile-speed requirement establishes it.
- The unused duplicate public `moveX`/`moveY` paths were removed. `step` is the single production Block-motion path; `applyGravity` is now private. `canGrow` stays public for the pending P2→P3 clearance handoff.

## Map-render handoff

### Person 1

- Implement production `MyApp`/State/GameState flow and replace the Brick-test entry point.
- Own the single active player and active level; inject only a borrowed `PlayerManager*` into P2.
- Consume `LevelManager::getWorldBounds()` for a dynamic camera, including narrow `W3_LV3`.
- Consume death/completion only after `LevelManager::update()` returns; state replacement must remain deferred.
- Surface load failure instead of rendering an apparently valid empty level.

### Person 3

- Initialize textures, first frames, and animations for Mario/Luigi, enemies, items, and bosses; logic-only objects are currently invisible.
- Provide typed enemy-contact, item-collection, inactive-removal, player-reset, and projectile-request outcomes.
- Provide a Bowser-defeated outcome for `W3_LV3`.

### Person 4

- Provide the shared executable-adjacent asset resolver and CMake/package asset copy policy.
- Preserve parsed terrain/style keys and render the correct Tileset frame instead of one default `SolidBlock` frame.
- Verify every 16x16 atlas frame scales to the adopted 64x64 world cell without sprite/hitbox drift.
- Load and render the stage background; P1 controls draw order/view, while P4 owns texture/render resources.
- Provide payload/block variant constructors and keep Cloud out of the current release.

## Final-level rule

`W3_LV3` intentionally has no WinFlag. P3 reports Bowser defeated, P2 converts that result into one level-completion notification, and P1 performs the final-game transition. Do not add a flag or allow both paths to fire.

The current release is 1P-only, so missing Player 2 map markers are not a release blocker and map PNGs must not be edited for that reason.

## Reproduce the current evidence

```powershell
cmake --build build --target Person2RuntimeContracts -j 4
ctest --test-dir build --output-on-failure -R Person2RuntimeContracts
.\build\Person2RuntimeContracts.exe
cmake --build build --target SuperMario -j 4
```

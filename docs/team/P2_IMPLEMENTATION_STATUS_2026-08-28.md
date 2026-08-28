# Person 2 implementation status — 2026-08-28

Branch reviewed: `person2/map-integration`
Implementation commit: `419d03d`

This note records verified implementation evidence, not a claim that the complete P2 epic is done.

## Verified now

- C++20/SFML 3.1 builds succeed for `SuperMario` and `Person2RuntimeContracts`.
- CTest passes `1/1`; a fresh direct run reports `57 PASS / 0 FAIL`.
- The original 27 ownership/lifecycle checks remain green.
- All nine `assets/textures/LevelSketch_W*_LV*.png` files parse; the two runtime bands contain zero unknown colors.
- Map-derived dynamic world bounds are available only after successful load.
- Pit death emits one fatal player callback and skips later same-frame contacts.
- Lava kills Small once and preserves Big/Fire downgrade semantics without a false death callback.
- Pipe, Trampoline, and MovingBlock collider/launch/end-point/carry contracts have focused runtime coverage.
- Cannon/Bullet/Rocket ownership, lifetime, target loss, Block collision, off-world culling, and once-only player damage have runtime coverage.
- WinFlag uses the map marker as its base, slides for a bounded duration, and queues one completion only after the animation.

The corresponding cards remain `TESTING` until production integration and the applicable visual/gameplay checks pass.

## P2 work still open

| Task | Remaining work | Provider/blocker |
|---|---|---|
| `P2-BOUNDS-001` | Clamp player at dynamic left/right world edges and cancel outward motion | P2 can start now |
| `P2-LOAD-001` | Replace CWD fallback with one executable-adjacent asset resolver; expose load diagnostic; make failed levels explicitly inert | P1 executable context, P4 package/copy provider |
| `P2-CLEANUP-001` | Enemy/item off-world cleanup; projectile part is complete | P3 inactive/removal APIs |
| `P2-CONTACT-ENEMY-001` | Stomp/side classification and once-only outcome forwarding | P3 contact result, P1 event sink |
| `P2-CONTACT-ITEM-001` | One-shot item collection, removal, and outcome forwarding | P3 item result, P1/P4 score/lives surface |
| `P2-VARIANT-WIRE-001` | Preserve terrain/block/payload metadata through P4/P3 construction | P4 block/style API, P3 payload types; Cloud excluded |
| `P2-NINE-LEVEL-001` | Production render/gameplay record for every wrapper | P1 flow, P3 visuals/outcomes, P4 terrain/resources/package |

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
cmake --build build-person2-baseline-ninja --target SuperMario Person2RuntimeContracts
ctest --test-dir build-person2-baseline-ninja --output-on-failure
.\build-person2-baseline-ninja\Person2RuntimeContracts.exe
```

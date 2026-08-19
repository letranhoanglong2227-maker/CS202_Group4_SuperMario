# Group4 Super Mario — Usage and Progress Workflow

This workflow is for both human implementers and future AI agents. It keeps task identity, ownership, contracts, evidence, and status stable across sessions.

## 1. Start every work session

1. Read the row for the task in `01_MASTER_TASK_BOARD.md`.
2. Read the full card in the owner's `P*_EXECUTION_PLAN.md`.
3. Read every consumed/provided `CON-*` card in `02_INTEGRATION_CONTRACTS.md`.
4. Read every named `DEC-*` card in `03_HUMAN_DECISIONS.md`.
5. Check the live Git status and source diff only for files the task may touch. Preserve user changes.
6. Confirm that task ID, owner, priority, status, dependencies, allowed files, and do-not-implement list still match.
7. If source behavior changed after the audit, annotate only the affected task `STATUS_CHANGED_SINCE_AUDIT`, cite the exact commit/diff evidence, and revise that card. Do not reopen the entire audit.

An AI prompt should name one stable task ID and say whether the agent is authorized to implement or only to review. A bare component name is insufficient when multiple tasks touch that component.

## 2. Choosing work

Pick in this order:

1. `READY` P0 work on the first vertical slice.
2. A provider task that unblocks multiple consumers.
3. `READY` P1 work in files outside current merge-risk hotspots.
4. Contract tests and integration checks for implemented provider/consumer pairs.
5. `LATE_INTEGRATION` and priority-P2 work only after their listed prerequisites.

Do not start a `BLOCKED` task merely because code can be guessed. Resolve the named decision, contract provider, or predecessor first. Work may be split into a clearly labeled unblocked checklist only if the parent card remains truthful and the split does not invent policy.

## 3. Status transitions

The only task statuses are:

| From | To | Required evidence |
|---|---|---|
| `READY` | `IN_PROGRESS` | Owner assigned, allowed files confirmed, dependencies/decisions clear, working branch/change scope recorded |
| `IN_PROGRESS` | `BLOCKED` | Specific blocker ID and last useful result recorded; partial source is not called complete |
| `BLOCKED` | `READY` | Named decision approved or dependency/contract actually available |
| `IN_PROGRESS` | `TESTING` | Source checklist complete, compile command passes, integration handoff available |
| `TESTING` | `IN_PROGRESS` | Failed check requires source correction; record the failing evidence |
| `TESTING` | `DONE` | All applicable completion dimensions and card DoD satisfied |
| Any non-done | `DEFERRED` | Explicit human scope/schedule approval and consequence recorded |
| Any | `EXCLUDED` | Explicit scope authority; never use this to hide unfinished required work |

`DONE` is not reached from `READY`, and a commit is not proof of `DONE`.

## 4. Five independent completion dimensions

Update each dimension on the detailed task card and mirror a compact `S/I/R/V/G` result on the master board:

| Dimension | Mark `YES` only when |
|---|---|
| `SOURCE_DONE` | All missing-behavior checklist items are implemented in allowed files; compile/static checks pass; no prohibited scope entered |
| `INTEGRATION_DONE` | Every provider and consumer contract used by the task works together in the production-shaped path |
| `RUNTIME_TESTED` | The smallest runnable automated/assert-based check passes for success, edge, and required failure behavior |
| `VISUALLY_VERIFIED` | A human/recorded visual run confirms scale, position, animation, view, readability, or other applicable rendering criteria |
| `GAMEPLAY_VERIFIED` | The production flow demonstrates the intended player-visible behavior and transition without a test-only shortcut |

Use `NOT_APPLICABLE` only when the task card explains why and a reviewer agrees. For example, a pure value serialization task may not need a visual gate, while a camera, sprite, menu, or HUD task always does.

## 5. Required evidence record

Keep evidence in the task card's progress section or the implementing change description; do not create ad hoc planning files. Record:

- task ID and exact Group4 commit/diff tested;
- decision revisions and contract revisions used;
- compiler/build command and result;
- runtime test executable/command and exact pass/fail count;
- integration scenario and involved owner tasks;
- visual/gameplay tester, date, stage/player count, and result;
- known failures classified as engine task, cross-owner contract, decision, or user-owned `KNOWN_MAP_EDIT_ITEM`.

Never claim visual or gameplay verification from syntax checks, source inspection, or a headless unit test.

## 6. Cross-owner edit request workflow

Implementation ownership does not move because another person found the defect or needs the interface.

When consumer owner A needs a change in provider owner B's file:

1. A creates a request reference `XREQ-<CONTRACT-ID>-<sequence>` in A's task progress notes.
2. A states the consuming task ID, provider task ID, exact provider file, current interface, smallest requested change, reason, failure behavior, and acceptance check.
3. A attaches or describes the smallest failing compile/contract check without editing B's production file.
4. B reviews the request against YAGNI and the existing API. B either:
   - implements the smallest provider change in B's allowed files and updates the provider task/contract evidence; or
   - rejects/adjusts it with a concrete reason and an alternative that satisfies the contract.
5. B lands or hands off the provider-first commit. A then updates only consumer-owned files.
6. Both owners run the contract integration check. Each updates `INTEGRATION_DONE` independently.

If emergency co-editing is explicitly approved, record the approval, file, primary editor, reviewer, and merge order before editing. Directory location, recent authorship, runtime ownership, or test authorship never grants implementation ownership.

## 7. Provider-first contract workflow

For each `CON-*` card:

1. Provider and consumer confirm input/output, lifetime, call timing, and failure behavior.
2. Provider adds the minimum stable public surface. Avoid a second abstraction if the current API can be extended safely.
3. Provider leaves one runnable check for non-trivial logic.
4. Consumer integrates without reaching into provider internals.
5. Run a production-shaped check that crosses the boundary.
6. Mark the contract implemented only when both sides agree and failure behavior is tested.

Do not use a global raw pointer, shared mutable singleton, or Group5 mediator ownership to bypass a missing contract.

## 8. Decision workflow

1. Open `03_HUMAN_DECISIONS.md` and review the options/consequences.
2. The named approver records selected option, approver, date, and rationale in `Final approval`.
3. Change the decision status from `OPEN` to `APPROVED` only after that record exists.
4. Update affected contract wording, then move genuinely unblocked tasks from `BLOCKED` to `READY`.
5. If the decision changes later, increment the decision revision, list affected completed/in-progress tasks, and do an impact review before new edits.

A recommendation, source assumption, verbal guess, test fixture, or implementation commit is not approval.

## 9. Merge-risk prevention

Before parallel work, compare each task's allowed files against the conflict watch list in the master board.

- One primary editor at a time for `LevelManager.*`, `PhysicsEngine.cpp`, `MapManager.cpp`, `PlayerManager.*`, `Block.*`, `GameState.*`, `src/main.cpp`, and `CMakeLists.txt`.
- Land provider interface/header changes before consumer implementation changes.
- Keep behavior, integration, and formatting changes in separate commits where practical.
- Do not combine a broad rename or formatter pass with gameplay integration.
- Rebase/merge only with user authorization and preserve unrelated working-tree changes.
- Re-run the relevant provider unit check and consumer integration check after conflict resolution.
- Never resolve a conflict by choosing an entire file side without inspecting both task IDs' intended changes.

Suggested commit format is already listed on each task card. Keep one task ID in the subject whenever possible.

## 10. Verified-baseline protection

The seven `BASE-*` records are frozen scopes. Work near them must:

- reuse the current unique-owner/view/participation model;
- preserve immediate spawn adoption and deferred deletion;
- keep same-frame inactive Block exclusion;
- preserve Rocket lost-target safety;
- preserve normal Brick one-hit/four-fragment behavior;
- preserve BrickFragment ownership/lifecycle;
- preserve corrected collision constants, public lifecycle access, and current Coin/Mushroom spawn corrections;
- run the existing 27-check runtime suite or its traceable replacement.

Reopen a baseline only with new reproducible evidence. Record the evidence on that baseline card and create/retarget the smallest owning task; do not relabel a stale report as current.

## 11. Nine-level validation workflow

There is one shared engine task, `P2-NINE-LEVEL-001`, and nine validation IDs. Do not duplicate physics, camera, parser, or state logic per level.

For each `LV-*` entry:

1. Confirm the user has declared that stage's map data ready; do not inspect/edit pixels without separate authorization.
2. Record build/commit, stage ID, 1P/2P decision profile, and active decision revisions.
3. Verify explicit load success and visible safe failure behavior separately.
4. Verify spawn, extent, camera clamp, left/right bounds, pit handling, representative block/enemy/item/hazard, completion, and death/win transition.
5. Record visual alignment and gameplay result.
6. Classify a failure:
   - reusable engine defect → existing owner task;
   - missing cross-owner boundary → existing `CON-*` card/request;
   - product choice → existing `DEC-*` card;
   - pixel/data defect → user-owned `KNOWN_MAP_EDIT_ITEM`.

Never fix a map-data failure by adding a one-level hard-coded engine exception.

## 12. Handoff template for humans or AI

Use this compact handoff in a task comment/change description:

```text
Task: <stable ID>
Status: <allowed status>
Source baseline: <commit>
Allowed files changed: <list>
Decisions used: <IDs + revisions>
Contracts used: <IDs>
SOURCE_DONE: YES/NO — <evidence>
INTEGRATION_DONE: YES/NO — <evidence>
RUNTIME_TESTED: YES/NO/NA — <command/result>
VISUALLY_VERIFIED: YES/NO/NA — <who/date/scenario>
GAMEPLAY_VERIFIED: YES/NO/NA — <who/date/scenario>
Remaining blocker or next task: <ID>
Known map item: <ID/none; no map content changed>
```

## 13. Final review protocol

Before declaring the project complete:

1. Recalculate board counts and ensure no required task is `READY`, `IN_PROGRESS`, `BLOCKED`, or `TESTING`.
2. Confirm all eight decisions have recorded approvals or an explicit approved deferral that does not violate required scope.
3. Confirm all 25 contract cards pass their acceptance rule.
4. Confirm all nine `LV-*` records pass independently.
5. Confirm the whole-game DoD in `00_IMPLEMENTATION_OVERVIEW.md`.
6. Run the clean C++20/SFML 3.1 build, registered tests, package smoke launch, and saved-data recovery checks.
7. Confirm no Group5 chatbot/LLM code, raw-pointer architecture, map-specific engine fork, or unauthorized cross-owner change entered Group4.

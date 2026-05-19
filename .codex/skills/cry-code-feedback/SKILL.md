---
name: cry-code-feedback
description: Apply CRY project-specific coding feedback and review habits when editing or reviewing code in this repository, especially C++ engine code, CRMath usage, refactoring cleanup, magic-number removal, or user feedback about repeated code style issues.
---

# CRY Code Feedback

Use this skill before and during edits in the CRY repository. Treat the user's review comments as standing project preferences, not one-off fixes.

## Editing Habits

- Prefer CRY abstractions over direct `std` or third-party calls in engine-facing code when replacement is plausible. Route math utilities through `CRMath`, containers through CR containers, and project aliases through `CRTypes`/CR headers.
- Do not store graphics API types such as `DXGI_FORMAT` in asset/cooked data that must survive backend changes. Store an engine/RHI-neutral type there and convert to the concrete API type at the backend boundary.
- Keep graphics API format conversion functions centralized in backend-specific files instead of duplicating switch statements at call sites.
- When changing asset metadata or binary file layout, migrate existing files under `Asset/` or provide compatibility loading in the same change.
- After introducing or finding a project abstraction, remove redundant local wrappers and update nearby equivalent call sites in the touched module.
- Do not apply only the literal line the user points at. Scan the surrounding function/file for the same pattern, the inverse condition, and follow-up cleanup made obvious by the change.
- Avoid magic numbers in conditions and algorithms. Promote them to named `static constexpr` values near the narrowest useful scope, or to a shared CRMath/project constant when the value is broadly meaningful.
- Name constants by intent, not by value. Example: `BasisReferenceAxisParallelDotThreshold`, not `DotThreshold999`.
- Preserve behavior while consolidating checks. Do not replace a finite-only check with a "usable vector" check if rejecting zero vectors would change when fallback logic runs.
- When a ternary condition becomes long, split the condition into a meaningfully named local `bool` first. Make the variable name and comparison operator match the actual meaning, even if that means inverting the comparison.
- Re-check readability after edits: avoid unnecessary references to temporaries, overlong condition lines, stale comments, and helper names that no longer add meaning.
- When editing XAML, format for human scanning instead of applying a mechanical "one attribute per line" rule. Keep short, related layout/display attributes on the same line when it improves readability, and break lines by semantic groups such as identity, grid placement, sizing, binding, events, and long resource references.
- In XAML, preserve visual structure with blank lines between major groups, rows, columns, panels, and repeated controls. Align rows and columns so the layout can be understood and modified without reformatting the file first.
- In XAML resource tables, preserve useful column alignment. For compact resources such as `Color` and `SolidColorBrush`, align key/value or key/attribute columns when that makes scanning easier.
- In multi-line XAML elements where attributes are one-per-line, preserve or restore the aligned `=` column, e.g. `BorderBrush         ="{...}"`.

## Current CRY Preferences Learned

- Use `CRMath::IsFinite(...)` instead of direct `std::isfinite(...)` in CRY engine code.
- Prefer `CRMath::IsFinite( CRVector )` over local helpers such as `_IsFiniteVector`.
- Use semantic helpers such as `_IsUsableBasisVector` when the condition is exactly "finite and large enough to normalize."
- Keep direct `CRMath::IsFinite` checks when only finiteness is intended and length validity would change behavior.
- Replace numeric thresholds like `0.999f` with named constants.
- For XAML readability, avoid habitual attribute-per-line formatting. Prefer meaning-based line breaks and grouping that make the layout easier to read, compare, and edit.
- Preserve useful XAML column alignment, especially in resource dictionaries where keys and values form a table.
- Preserve useful XAML multi-line attribute alignment, including padded attribute names before `=`.
- In C# code, format generic type arguments with spaces inside angle brackets, such as `IReadOnlyList< TextureImportOption >`, `List< T >`, and `Func< int, IntPtr >`.
- In C# `switch` blocks, keep compact single-action cases on one line with aligned case bodies, e.g. `case Foo: ++fooCount; break;`.

## Verification

- Search the touched module for the old symbol/pattern after refactoring.
- Build at least the nearest affected project when practical. For this repository, `Engine/Engine.vcxproj` with `DebugEditor|x64` is the usual quick C++ verification target.

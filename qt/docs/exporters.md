<!-- Copyright (c) 2026 Jason Self <j@jxself.org>. GPLv3-or-later; see qt/COPYING. -->

# Trizbort Export Subsystem — C++ Reimplementation Specification

This document specifies the behavior of Trizbort's code-export subsystem (`Export/`), derived by reading the C#/.NET source. It is intended as the reference against which a C++ port is validated. All file:line references are to the original C# tree at the repository root.

The subsystem converts a Trizbort map (rooms, connections, regions, and per-room "objects" text) into source code for eight interactive-fiction authoring systems. A shared base class (`CodeExporter`) builds an *intermediate model* from the live map, then each concrete exporter serializes that model into its target syntax.

---

## Export pipeline

### Entry points and orchestration

`CodeExporter` (`Export/CodeExporter.cs:40`) is an abstract class. It is instantiated and driven from `UI/MainForm.cs`:

- **Copy-to-clipboard export** (`MainForm.cs:258` `exportCode<T>()`): calls `exporter.Export()` (no-arg overload, `CodeExporter.cs:84`), which returns a `string` written into a `StringWriter`, then puts it on the clipboard.
- **Export-to-file** (`MainForm.cs:265` `exportCode<T>(ref string)`): builds a `SaveFileDialog` whose filter string is composed from `FileDialogFilters` as `"{Key}|*{Value}"` joined by `|`, with `All Files|*.*||` appended (`MainForm.cs:271-278`); the dialog title is `FileDialogTitle`. On OK, calls `exporter.Export(fileName)` (`CodeExporter.cs:108`).
- **Command-line export** (`MainForm.cs:307` `exportCodeCl<T>(fileName)`): calls `exporter.Export(fileName)` directly.

Both `Export` overloads perform the same sequence (`CodeExporter.cs:84-124`):

1. Compute `title`: `Project.Current.Title`; if empty, the project filename without extension (`PathHelper.SafeGetFilenameWithoutExtension`); if still empty, `"A Trizbort Map"`.
2. Compute `author`: `Project.Current.Author`; if empty, `"A Trizbort User"`.
3. `history = Project.Current.History` (may be empty).
4. `prepareContent()` — builds the intermediate model (below).
5. `ExportHeader(writer, title, author, Project.Current.Description ?? "", history)` — abstract.
6. `ExportContent(writer)` — abstract.

Header always precedes content. The description passed to `ExportHeader` is never null (coalesced to empty).

### The file writer

`Create(fileName)` (`CodeExporter.cs:126`) returns:

```csharp
new StreamWriter(fileName, false, Encoding.ASCII, 2 ^ 16)
```

Two important facts for a port:
- **Encoding is ASCII**, non-appending (overwrite). Non-ASCII characters are lost/substituted by the writer unless the exporter strips or transliterates them first (several do — see `Deaccent`).
- `2 ^ 16` is **bitwise XOR in C#, evaluating to 18** (not 65536). So the buffer size is 18 bytes. This is almost certainly an unintended bug but has no functional effect on output; a port need not reproduce it.

`Inform7Exporter` and `AlanExporter` override `Create` (`Inform7Exporter.cs:50`, `AlanExporter.cs:48`): if the chosen filename ends in `.inform`, they treat it as an Inform project *bundle* — they create a `Source` subdirectory under that path and redirect output to `Source/Story.ni`, then delegate to the base `Create`.

### `prepareContent()` — model-building order

`prepareContent()` (`CodeExporter.cs:280`) runs five passes in this exact order:

```
findRegions();
findRooms();
findExits();
pickBestExits();
findThings();
```

Order matters: regions are named first so rooms can avoid colliding with region names; rooms are named before exits (exits reference `Location`s); exits are picked before things.

#### `findRegions()` (`CodeExporter.cs:165`)

- Builds a case-insensitive `Dictionary<string, Region>` (`InvariantCultureIgnoreCase`) of used export names.
- Seeds it with every reserved word (`ReservedWords` -> `null`) so region names cannot collide with reserved words.
- Iterates `Settings.Regions` (`Setup/Settings.cs:288`, a `List<Region>`) **excluding** the default region (`Region.DefaultRegion` == the literal string `"NoRegion"`, `Domain/Misc/Region.cs:15`).
- For each region: `exportName = GetExportName(region.RegionName, null)`; if empty -> `"region"`. De-duplicate by re-calling `GetExportName(region.RegionName, index++)` starting at `index = 2` until unique.
- Appends `new ExportRegion(region, exportName)` to `RegionsInExportOrder`. **Region export order = `Settings.Regions` list order.**

#### `findRooms()` (`CodeExporter.cs:185`)

- Case-insensitive dictionary seeded with reserved words -> `null` **and** every region's `ExportName` -> `null`.
- Iterates `Project.Current.Elements.OfType<Room>()` **in element (creation/document) order**.
- For each room: `exportName = GetExportName(room, null)`; if empty -> `"object"`; de-duplicate via `GetExportName(room, index++)` from `index = 2`.
- Creates `Location(room, exportName)`, appends to `LocationsInExportOrder`, and records `mMapRoomToLocation[room] = location`. **Location export order = project element order.**

#### `findExits()` (`CodeExporter.cs:140`)

- Iterates `Project.Current.Elements.OfType<Connection>()`.
- For each connection: `sourceRoom = connection.GetSourceRoom(out sourceCompassPoint)` and `targetRoom = connection.GetTargetRoom(out targetCompassPoint)` (`Connection.cs:328`, `:344` — the compass point is the *dock port* on each room).
- **Skips** the connection if either endpoint room is null (a dangling/floating connection), or if it is a self-loop docked to the same compass point (`sourceRoom == targetRoom && sourceCompassPoint == targetCompassPoint`).
- If both rooms map to `Location`s: adds `new Exit(sourceLocation, targetLocation, sourceCompassPoint, connection.StartText, connection)` to the source location.
- **Only if `connection.Flow == ConnectionFlow.TwoWay`**: also adds the reverse `Exit(targetLocation, sourceLocation, targetCompassPoint, connection.EndText, connection)` to the target location. One-way connections (`ConnectionFlow.OneWay`) produce a single exit from source->target only.

`ConnectionFlow` is `{TwoWay, OneWay}` with default `TwoWay` (`Connection.cs:34,860`). `StartText`/`EndText` carry the direction-label text (e.g. `"up"`, `"down"`, `"in"`, `"out"`), used to detect secondary directions.

#### `pickBestExits()` (`CodeExporter.cs:274`)

Calls `Location.PickBestExits()` on each location (below).

#### `findThings()` (`CodeExporter.cs:210`)

- Case-insensitive dictionary seeded with reserved words, **all** location export names, and **all** region export names -> `null`. This single shared dictionary makes every thing's export name unique across the *entire* map (not just within its room).
- For each location, reads `location.Room.Objects` (`Room.cs:229`, the multi-line "objects" text of a room). If empty, skip.
- Splits the text on `\n` after stripping `\r`, with `RemoveEmptyEntries`.
- For each line:
  - `displayName = line.Trim()`.
  - **Property-string extraction**: a regex `\[[^\]\[]*\]` (`CodeExporter.cs:232`) looks for a bracketed group. If found:
    - `propString` is set to the whole line, then the bracket group(s) are removed from `displayName` (`rgx.Replace(displayName, "")`).
    - `propString` is reduced to the *contents inside the brackets*: first `.*\[` -> `""` (drop everything through the last `[`), then `\].*` -> `""` (drop the first `]` onward). So `lantern [ms]` -> `displayName = "lantern"`, `propString = "ms"`.
  - If `displayName` is empty after stripping, skip the line.
  - `exportName = GetExportName(displayName, null)`, de-duplicated from `index = 2`.
  - **Indentation -> containment**: `indent` = number of leading spaces on the *original* line. The container is found by scanning `location.Things` backwards for the first prior thing whose `Indent` is strictly less than this line's `indent` (`CodeExporter.cs:259-265`). That thing becomes `container`.
  - Constructs `new Thing(displayName, exportName, location, container, indent, propString)` and appends to both the dictionary and `location.Things`.

### `GetExportName` and name sanitization

Two abstract overloads (`CodeExporter.cs:137-138`) must be implemented per exporter:

```csharp
protected abstract string GetExportName(Room room, int? suffix);
protected abstract string GetExportName(string displayName, int? suffix);
```

They convert a human name into a valid identifier for the target language, optionally appending a numeric `suffix` for de-duplication. The base class's de-dup loops always pass suffixes starting at `2`. Sanitization rules differ per exporter (documented in each section). The base provides one shared helper:

**`Deaccent(string)`** (`CodeExporter.cs:62`): transliterates accented Latin letters to ASCII (a-grave..a-ring->a, etc.; c-cedilla->c; n-tilde->n; and so on). All other characters pass through unchanged. Note: the source's uppercase-O and lowercase-o accent ranges are swapped in case in the mapping — a quirk that should be reproduced verbatim for byte-identical output.

### Reserved-word handling

Each exporter exposes `protected abstract IEnumerable<string> ReservedWords` (`CodeExporter.cs:56`). Reserved words are seeded as keys into the de-dup dictionaries for regions, rooms, and things, so any generated name equal to a reserved word will "collide" and be forced to take a numeric suffix. Additionally, three exporters (Alan, Inform7, Zil) *also* test for reserved words inside their `GetExportName` and pre-apply a suffix; the others rely solely on the collision mechanism.

### Abstract surface a concrete exporter must implement

| Member | Purpose |
|---|---|
| `List<KeyValuePair<string,string>> FileDialogFilters` | Ordered (label, extension) pairs for the Save dialog. |
| `string FileDialogTitle` | Save dialog title. |
| `IEnumerable<string> ReservedWords` | Identifiers that generated names must avoid. |
| `void ExportHeader(writer, title, author, description, history)` | Emit file header/preamble. |
| `void ExportContent(writer)` | Emit rooms, connections, objects, footer. |
| `string GetExportName(Room, int?)` | Room name -> identifier. |
| `string GetExportName(string, int?)` | Object/region name -> identifier. |
| *(optional)* `StreamWriter Create(fileName)` | Override output stream creation. |
| *(optional)* `void Dispose(bool)` | Cleanup. |

### Writer-call sequence for a full export

```
Export(file):
  Create(file)                        // ASCII StreamWriter (or .inform bundle redirect)
  prepareContent()                    // findRegions -> findRooms -> findExits -> pickBestExits -> findThings
  ExportHeader(writer, ...)           // preamble/includes/metadata
  ExportContent(writer)               // regions? -> per-location(room, exits, doors, things) -> player/init -> footer
  dispose writer                      // flush + close
```

---

## Intermediate model

The intermediate model lives in `Export/Domain/`. It decouples the exporters from the live editor model (`Room`, `Connection`, `Region`, `Door`).

### `Location` (`Export/Domain/Location.cs`)

Wraps one `Room` plus its generated `ExportName`.

- `Room Room` (`:44`), `string ExportName` (`:42`).
- `List<Thing> Things` (`:46`) — objects in this room (flat list including nested contents; nesting is expressed via each `Thing.Container`).
- private `List<Exit> mExits` — all exits leading out (added by `findExits`).
- private `Dictionary<MappableDirection, Exit> mMapDirectionToBestExit` — the chosen exit per direction.
- `AddExit(Exit)` (`:48`).
- `PickBestExits()` (`:56`): clears the map, then for **each direction in `Directions.AllDirections`** calls `pickBestExit(direction)` and stores non-null results.
- `pickBestExit(direction)` (`:64`): sorts `mExits` by descending `GetPriority(direction)`, then takes the top exit **only if** its `PrimaryDirection == direction` or `SecondaryDirection == direction`; otherwise no exit exists in that direction.
- `GetBestExit(direction)` (`:52`): dictionary lookup, returns `null` if absent.

### `Exit` (`Export/Domain/Exit.cs`)

Represents **one direction** of a connection (a two-way connection yields two `Exit`s, one per room).

Constructed from `(Location source, Location target, CompassPoint visualCompassPoint, string connectionText, Connection connection)` (`:45`):

- `Source`, `Target` (`Location`s).
- `VisualCompassPoint` — the compass port on the source room where the connection docks (16-point `CompassPoint`, `Domain/Misc/CompassPoint.cs:32`).
- `Door Door = connection.Door` (may be null).
- `ConnectionName = connection.Name`, `ConnectionDescription = connection.Description`.
- `Conditional = (connection.Style == ConnectionStyle.Dashed)` (`:52`) — a dashed connection is a "conditional" exit.
- `Exported` — a mutable flag exporters set to avoid emitting a connection twice.
- `PrimaryDirection` (a `MappableDirection`: N/S/E/W/NE/SE/SW/NW) and `SecondaryDirection` (nullable: Up/Down/In/Out).

**Direction assignment** (constructor, `:54-59`):
1. `assignPrimaryPriority()` (see quirk below).
2. `assignSecondaryDirection(connectionText)` (`:177`): maps the label text `Connection.Up`/`Down`/`In`/`Out` (the strings `"up"`,`"down"`,`"in"`,`"out"`) to `MappableDirection.Up/Down/In/Out`; anything else -> `null`.
3. If `SecondaryDirection != null`, `PrimaryDirection = (MappableDirection)SecondaryDirection` (i.e. the exit *is* an up/down/in/out exit). Otherwise `assignPrimaryDirection()` (`:112`) reduces the 16-point `VisualCompassPoint` to one of the 8 cardinals/ordinals (NNW/N/NNE->North; NE->NorthEast; ENE/E/ESE->East; SE->SouthEast; SSE/S/SSW->South; SW->SouthWest; WSW/W/WNW->West; NW->NorthWest).

**Priority** (`assignPrimaryPriority`, `:151`; `GetPriority`, `:92`):
- Priority starts at 0. If `VisualCompassPoint` is exactly a cardinal/ordinal (N,S,E,W,NE,SE,SW,NW): `+4`. Otherwise (an intermediate point like NNE): `+3`.
- **Quirk to reproduce exactly:** `assignPrimaryPriority()` runs *before* `assignSecondaryDirection()`, so it reads `SecondaryDirection` while it is still `null`. Therefore the `else`-branches that would subtract (-2 / -1) are **dead code** and never execute. Net effect: priority is always `+4` (docked at a cardinal/ordinal point) or `+3` (docked at an intermediate point).
- `GetPriority(direction)`: returns the primary priority (3 or 4) if `direction == PrimaryDirection`; returns `1` if `direction == SecondaryDirection`; otherwise `-1`.

So when two exits leave a room the same visual way, the one docked precisely on a cardinal/ordinal wins, and an explicit up/down/in/out (secondary) exit scores 1 in its secondary direction.

**`IsReciprocated(source, direction, target)`** (`:99`): true iff `target.GetBestExit(GetOpposite(direction))` exists and leads back to `source`. Used by exporters that emit each two-way connection once.

`MappableDirection` enum order (`Domain/Enums/MappableDirection.cs`): `North, South, East, West, NorthEast, SouthEast, SouthWest, NorthWest, Up, Down, In, Out, None`. `Directions.AllDirections` (`Domain/Directions.cs`) yields them in this enum order — **this is the iteration order for emitting exits in every exporter**, and it determines which of several competing best-exits gets emitted first. `None` never matches an exit.

`CompassPointHelper.GetOpposite(MappableDirection)` (`Domain/Misc/CompassPoint.cs:287`): N<->S, E<->W, NE<->SW, NW<->SE, Up<->Down, In<->Out.

### `Thing` (`Export/Domain/Thing.cs`)

Represents one object parsed from a room's `Objects` text.

Fields: `DisplayName`, `ExportName`, `Location`, `Container` (parent `Thing` or null), `Contents` (child `Thing`s), `Indent`, `PropString`, `WarningText`, and derived boolean/enum flags.

Constructor (`:62`) registers itself into `container.Contents` when a container exists, then parses `propString` character-by-character. Enums: `Amounts {Noforce, Singular, Plural}` and `ThingGender {Neuter, Male, Female}`.

**Property-string codes** (valid set is `[fmp12csuwh!]`; any other character is appended to `WarningText`):

| Code | Effect |
|---|---|
| `f` | `IsPerson = true`, `Gender = Female` |
| `m` | `IsPerson = true`, `Gender = Male` (warns if a gender already set) |
| `p` | `IsPerson = true`, `Gender = Neuter` (warns if a gender already set) |
| `w` | `Worn = true` |
| `h` | `PartOf = true` |
| `1` | `Forceplural = Singular` |
| `2` | `Forceplural = Plural` (warns if singular already forced) |
| `c` | `IsContainer = true` (unless person -> warning, not set) |
| `s` | `IsScenery = true` (warns if person) |
| `u` | `IsSupporter = true` (unless person -> warning, not set) |
| `!` | `ProperNamed = true` |

### `ExportRegion` (`Export/Domain/ExportRegion.cs`)

Trivial wrapper of `Region Region` + `string ExportName`.

### Relationship to the domain model

- `Location` <-> `Room` (1:1); `mMapRoomToLocation` links them.
- `Exit` <-> `Connection` (a two-way `Connection` -> two `Exit`s; a one-way -> one). `Exit` reads `Connection.Door/Name/Description/Style` and the per-end dock `CompassPoint`.
- `Thing` is derived purely from `Room.Objects` text; no domain object corresponds to it.
- `ExportRegion` <-> `Region` (1:1, excluding the default `"NoRegion"`). A room's region membership is the string `Room.Region` (`Room.cs:281`); the default/absent value is `Region.DefaultRegion` == `"NoRegion"`.

`Door` (`Domain/Elements/Door.cs`): four bools — `Lockable, Locked, Open, Openable`.

---

## Per-exporter reference

Common note on direction iteration: unless stated otherwise, each exporter loops `Directions.AllDirections` (N, S, E, W, NE, SE, SW, NW, Up, Down, In, Out) and emits `location.GetBestExit(direction)` when non-null.

---

### 1. Adventuron (`Export/Languages/AdventuronExporter.cs`)

- **Target:** Adventuron classic.
- **Extensions / filters:** `.adv` ("Adventuron Source Files"), `.txt` ("Text Files"). **Title:** `"Adventuron Source Code (rooms only)"`.
- **Reserved words:** `ether, objects, inventory, root, player`.
- **Header:** `ExportHeader` is a **no-op** (`:188`); all metadata is discarded.

**File structure** — `ExportContent` (`:27`) builds three blocks in memory then writes them in this order after a blank line:

```
start_at = <startRoom>

locations {
   <id> : location "<desc>" header = "<name>";
   ...
}

connections {
   from, direction, to = [
      <id>, <dir>_oneway, <target>,
      ...
   ]
}
```

- **Start room:** the *last* location that is a start room, or the first location if none is flagged (`:49`).
- **Room -> location line** (`:67`): `   {id padded to maxLen} : location "{escaped primaryDescription}" header = "{escaped name}";`. `maxLen` is the longest escaped id (used for column alignment; note the width loop at `:43` compares *raw* `ExportName.Length` but stores *escaped* length — a minor quirk).
- **Room name** is truncated to **25 characters** (`MaximumHeaderLength`, `:15,59`) *before* escaping.
- **Connections** (`:74`): every best exit becomes one line `      {id}, {dir}_oneway, {targetId},`. **All exits are emitted as one-way**; a two-way connection therefore appears as two `_oneway` rows (one from each room, produced by `findExits`).
- **Direction names** (`toAdventuronDirectionName`, `:210`): `north_oneway, south_oneway, east_oneway, west_oneway, northeast_oneway, northwest_oneway, southeast_oneway, southwest_oneway, up_oneway, down_oneway, enter_oneway` (In), `leave_oneway` (Out).
- **Regions:** not emitted.
- **Objects/things:** not emitted (rooms only).
- **`GetExportName`** (`:200`): lowercase; replace `' '`, `'.'`, `'$'` -> `'_'`; empty -> `"location"`; suffix -> `{name}_{suffix}` (underscore-joined).
- **`escapeAdventuronId`** (`:105`): space/`_` collapse into a single `_` (no leading or doubled underscores); `'` dropped; letters/digits and specific Unicode ranges kept; everything else dropped.
- **`escapeAdventuronText`** (`:139`): `\n`->`\n` (literal backslash-n), `\r` dropped, `$`->`$$`, `\`->`\\`, `[`->`[[`, `]`->`]]`, `{`->`{{`, `}`->`}}`, `"`->`\"`, `~`->`~~`, `<`->`<<`, `>`->`>>`.

---

### 2. Alan (`Export/Languages/AlanExporter.cs`)

- **Target:** ALAN 3.
- **Extensions / filters:** `.i` ("Alan Source Files"), `.txt`. **Title:** `"Export Alan Source Code"`.
- **Reserved words:** `object, objects, thing, things, door, doors, is, are, in, on, and, outside, inside`.
- **`Create` override** (`:48`): contains copy-pasted Inform7 logic that redirects a `.inform` filename to a `Source/Story.ni` bundle — effectively dead for Alan since its filter is `.i`, but present.

**Header** (`ExportHeader`, `:60`) — all comments (`--`): metadata as comments; `-- "<title>" by <author>`; optional `-- description: <desc>.`; if history present, a `Verb about ... End Verb about.` block that emits the history (with `\r\n` turned into a newline + 4 spaces + reopened quote).

**Content** (`ExportContent` -> `printThisLoc`, `:91`), per location:
```
The <exportName> isa location Name '<Room.Name>'
  Description
  "<PrimaryDescription>"                (or:  Description ""  when empty)
  Exit <Dir> to <target>.               (per real exit)
    Check                               (only if Conditional)
      "This was marked as a conditional exit in Trizbort, so you'll want to change it."
  End exit.

  Exit <dirs...> to nowhere             (aggregated "blocked" directions)
    Check
      "You can't go that way."
  End exit.
  Is Not lit.                           (only if Room.IsDark)
end The <exportName>.

The hero Isa actor at <exportName>      (only if IsStartRoom)
End The Hero.

The <thing> isa {actor|thing} at <exportName>.   (per thing)
  IsDisplayedAs <DisplayName>.
End The <thing>.
```

- **Exits:** each best exit is emitted **once** — the exit's `Exported` flag is set on first emission (`:115`). An exit that is `null` **or already `Exported`** (e.g. the reciprocal half of a two-way connection) is instead added to the `nowhere` blocked-directions list (`:126-131`) — a notable quirk: reciprocated two-way exits get written as "to nowhere" from the second room's perspective.
- **Direction names** (`getAlanName`, `:231`): `North, South, East, West, Northeast, Southeast, Northwest, Southwest, Up, Down, Inside` (In), `Outside` (Out).
- **Objects/things:** emitted flat (no containment); `actor` if `IsPerson` else `thing`. Warning text output is commented out in source.
- **Regions:** not emitted.
- **`GetExportName`** (`:191`): if the name `containsOddCharacters` (anything besides space, `-`, letter, digit), `stripOddCharacters` keeps only space/`-`/alphanumerics (empty -> `"object"`). If the name contains a reserved word and no suffix given, force `suffix = 1`. Suffix appended directly (`{name}{suffix}`). **Names may retain spaces and hyphens** (e.g. `Living Room` stays `Living Room`).

---

### 3. Hugo (`Export/Languages/HugoExporter.cs`)

- **Target:** Hugo.
- **Extensions / filters:** `.hug` ("Hugo Source Files"), `.txt`. **Title:** `"Export Hugo Source Code"`.
- **Reserved words:** `Room, Actor, Thing, Object, Door, Chair, Heavy, Fixture, OpenableContainer, Food, GameMainDef, if, else, me`.

**Header** (`:89`):
```
#include "verblib.g" ! grammar must come first

#include "hugolib.h"

routine init
{
	cls
	verbosity = 2
	counter = -1
	STATUSTYPE = 1 !1 = score / turns, 2 = time, 3 = moves: score:
	player = you
	Font(BOLD_ON)            (title block, if title)
	"<title>"
	Font(BOLD_OFF)
	"<description>, by <author>"    (variants: desc only / "by author" only)
	location = <startRoom>          (extra start rooms noted with "! ... is an extra StartRoom.")
	                                 (fallback: location = <firstRoom>, or "! location = ...")
	MovePlayer(location)
}
```
- If `history` is present, an `showAbout()` block is written **inside** `init` before `MovePlayer` (`exportHistory`, `:152`) — likely misplaced, but reproduce as-is.

**Content** (`:50`):
```
property region              (only if any regions)

room <exportName>
{
	region <Room.Region>       (raw region display name, only if not default)
	long_desc
	"
		<PrimaryDescription>
	"

	<dir_to> <target>          (per best exit)

	is not light               (only if IsDark)
}

object <thing>               (per thing, recursively)
{
	in <Room.exportName>       (NOTE: always the room, not the container)
}
...
player_character you "you"
{
}
```
- **Direction property names** (`toHugoPropertyName`, `:188`): `n_to, s_to, e_to, w_to, ne_to, nw_to, se_to, sw_to, u_to, d_to, in_to, out_to`.
- **Exits:** no `Exported` tracking; both halves of a two-way are emitted (each room lists its own `*_to`).
- **Regions:** only the marker `property region` and per-room `region <name>` (raw `Room.Region` string, not the sanitized export name).
- **Objects/things:** recursive via `exportThings` (`:160`), but each object's `in` clause points at `thing.Location.ExportName` (the room), **not** its container — so containment is effectively flattened to the room (quirk).
- **`GetExportName`** (`:137`): `stripOddCharacters` keeps only `A-Za-z0-9_`; room empty -> `"room"`, object empty -> `"item"`; suffix appended directly.
- Helper `toHugoString` (quote-escaping) and `repeat` exist but are unused on the active path (descriptions are emitted raw/unescaped).

---

### 4. Inform 6 (`Export/Languages/Inform6Exporter.cs`)

- **Target:** Inform 6.
- **Extensions / filters:** `.inf` ("Inform 6 Source Files"), `.txt`. **Title:** `"Export Inform 6 Source Code"`.
- **Reserved words:** `Constant, Story, Headline, Include, Object, with, has, hasnt, not, and, or, n_to, s_to, e_to, w_to, nw_to, ne_to, sw_to, se_to, u_to, d_to, in_to, out_to, before, after, if, else, print, player, location, description`.

**Header** (`:116`):
```
Constant Story "<title>";
Constant Headline "^By <author>^<description>^^";

Include "Parser";
Include "VerbLib";
```

**Content** (`:57`):
```
Class <regionExportName>;        (one per region, if any)

<Object|regionExportName>  <exportName> "<Room.Name>"    (class = region export name if room has a region)
  with  description
            "<PrimaryDescription>",
        <dir_to> <target-or-doorName>,     (per best exit; door name if the exit has a door)
   has  light;                             (or "~light" if IsDark)

Object <doorName> <ConnectionDescription>  (per door, once)
  with  name '<vocab words>',
        description "<ConnectionDescription>",
        found_in <loc> <target>,
        door_to [; if (self in <loc>) return <target>; return <loc>;],
        door_dir [; if (self in <loc>) return <dir_to>; return <opp_to>; ],
  has   door openable open lockable locked ;

Object -> <thing> "<DisplayName>"          (per thing; "-> " repeated per nesting level)
  with  name '<vocab>',
        description "<DisplayName>"
  has <attributes>;

[ Initialise;
    location = <startRoom>;      (extra starts -> "! ... is a second start-room"; fallback firstRoom, or "! location = ...;")
    ! "^^Your opening paragraph here...^^";
];

Include "Grammar";

Verb meta 'about' * -> About;    (only if history)
[ AboutSub ;
  print("<history>");
];
```

- **Room class** (`writeLocation`, `:237`): `Object` if the room's region is default, else `GetExportName(Room.Region, null)` (computed on the fly — may differ from the `Class` name emitted at top if de-dup added a suffix; minor quirk).
- **Exits:** non-door exits are plain `<dir_to> <target>` properties in each room; a two-way connection lists `n_to`/`s_to` in the respective rooms (no `Exported` skip for non-doors). **Doors** are emitted once (guarded by `Exported`, `:70`) as full `Object` definitions with `found_in`, `door_to`, `door_dir`, and a `has door ...` attribute line whose tokens are `openable`/`""`, `open`/`~open`, `lockable`/`""`, `locked`/`~locked` (`:228`). When an exit has a door, the room's directional property points at the door's export name instead of the target.
- **Conditional (dashed) exits:** not handled specially by Inform 6.
- **Direction property names** (`toI6PropertyName`, `:165`): `n_to, s_to, e_to, w_to, ne_to, nw_to, se_to, sw_to, u_to, d_to, in_to, out_to`.
- **Things -> attributes** (`setAttributes`, `:278`): contents present -> `transparent` (if any content is `PartOf`) else `open container`; `proper` (ProperNamed); `animate` + `female`/`male`/`neuter` (IsPerson + gender); `scenery`; `supporter`; `container` (IsContainer, if not already); `pluralname` (Forceplural == Plural); `clothing` (Worn — I6 has no per-object `worn`).
- **`GetExportName`** (`:126`): `Deaccent(stripUnaccentedCharacters(name))` then remove all spaces and hyphens; room empty -> `"room"`, object empty -> `"item"`; suffix appended directly. `stripUnaccentedCharacters` keeps `A-Za-z0-9_`, the listed accented letters, spaces and hyphens; `Deaccent` then folds accents to ASCII.
- **String escaping** (`toI6String`, `:197`): `"` -> `~`, remove `\r`, `\n` -> `^`, wrap in the chosen quote. Vocabulary words (`toI6Words`, `:202`): space-split, each word `Deaccent`ed and single-quoted; empty -> `'thing'`.

---

### 5. Inform 7 (`Export/Languages/Inform7Exporter.cs`)

- **Target:** Inform 7.
- **Extensions / filters:** `.inform` ("Inform 7 Projects"), `.ni` ("Inform 7 Source Files"), `.txt`. **Title:** `"Export Inform 7 Source Code"`.
- **Reserved words:** `object, objects, thing, things, door, doors, is, are, in, on, and, outside, inside`.
- **`Create` override** (`:50`): `.inform` filename -> `Source/Story.ni` bundle.

**Header** (`:96`):
```
"<title>" by "<author>"

The story description is "<description>".   (if present)

Volume Trizbort generated map

chapter about                               (if history)

abouting is an action out of world.
understand the command "about" as something new.
understand "about" as abouting.
carry out abouting: say "<history, \r\n -> \r\n[line break]>".
```

**Content** (`:60`), organized into Inform "books": `book Regionless Rooms` (regionless rooms), then per region `book <regionExportName>` with `There is a region called <regionExportName>.` and its rooms, then (if any conditional exits were emitted) a `book conditional exit warning` with a `To block conditional exits:` rule.

Per location (`printThisLoc`, `:217`):
```
part <exportName>

There is a room called <exportName>. The printed name of it is "<Room.Name>". <description>. It is dark. It is in <regionExportName>.

The player is in <exportName>.        (if IsStartRoom)

<article><exportName> <whatItIs> in <location>.    (per thing; placement varies)
 It is privately-named. The printed name of it is "<DisplayName>". Understand <words> as <exportName>.
[Note: there were errors with your bracketed definitions. ...]   (if WarningText)

<Dir> of <location> is <target>.       (per exit; one-way adds "<Opp> of <target> is nowhere.")

Instead of going <dir> from <location>, block conditional exits.   (per conditional exit)
```

- **Region assignment for a room:** ` It is in <ExportRegion.ExportName>` where the region is found by `RegionsInExportOrder.Find(RegionName == Room.Region)` (`:235`). Only rooms whose `Room.Region == "NoRegion"` appear in the *Regionless* book; a room whose region is the empty string `""` (counted by `NumberOfRoomsWithoutRegion` but `!= "NoRegion"`) is **dropped entirely** (quirk).
- **`printedname`:** emitted only when `ExportName != Room.Name` (avoids I7 duplicate-name errors).
- **Things** (`whatItIs`, `:366`; `getArticle`, `:147`): person -> `<name> is a <Gender> person`; scenery/container/supporter variants; otherwise `is a [plural-named ][proper-named ]thing`. Article: `""` (proper or capitalized or plural), `"Some "` (plural/forced-plural lowercase), `"An "` (vowel), `"A "`. Placement: `in <room>` (no container); `worn by`/`carried by` (container is a person); `part of`/`in` (container is an object with/without `PartOf`).
- **Normal exit** (`writeNormalExit`, `:399`): `<Dir> of <location> is <target>.` If reciprocated, the reverse exit's `Exported` flag is set so it is not re-emitted (I7 infers two-way). If the target has *no* opposite exit at all, appends `<Opp> of <target> is nowhere.` to make the one-way explicit.
- **Door** (`writeDoor`, `:384`): four sentences declaring the door, its direction between the two rooms, open/openable and locked/lockable states, and its description; marks the reciprocal `Exported`.
- **Direction names** (`getInform7Name`, `:177`): `North, South, East, West, Northeast, Southeast, Northwest, Southwest, Up, Down, Inside` (In), `Outside` (Out). Door sentences lowercase the raw enum name.
- **`GetExportName`** (`:165`): odd chars stripped (keep space/`-`/alphanumerics; empty -> `"object"`); if it contains a reserved word and no suffix, force `suffix = 1`; suffix appended directly.
- **String escaping** (`toInform7PrintableString`, `:331`): `'` -> `[']`, `"` -> `'`, then wrap in double quotes. Understand-words (`:339`): space-split, each `stripOddCharacters(word, ')` quoted, joined with ` and `.

---

### 6. Quest (`Export/Languages/QuestExporter.cs`)

Two classes: `QuestRoomsExporter` (base, rooms-only) and `QuestExporter` (adds game wrapper).

- **Target:** Quest 5 (ASLX / XML).
- **Extensions / filters:** `.aslx` ("Quest Source Files"), `.txt`. **Titles:** `"Export Quest Source Code (rooms only)"` (base) / `"Export Quest Source Code"` (`QuestExporter`, `:151`).
- **Reserved words:** `object, game, turnscript`.

**`QuestRoomsExporter`** emits **no header** and, per location (`:24`):
```
  <object name="<exportName>">
    <inherit name="editor_room" />
    <alias><Room.Name></alias>
    <dark />                                             (if IsDark)
    <attr name="grid_width" type="int"><Width/32></attr>
    <attr name="grid_length" type="int"><Height/32></attr>
    <attr name="grid_fill"><#RRGGBB fill></attr>
    <attr name="grid_border"><#RRGGBB border></attr>
    <attr name="implementation_notes"><Room.GetToolTipText()></attr>
    <description><PrimaryDescription></description>      (if present)
    <exit alias="<dir>" to="<target>">
      <inherit name="<dir>direction" />
    </exit>
    <object name="player">                               (if IsStartRoom)
      <inherit name="editor_object" />
      <inherit name="editor_player" />
    </object>
    <object name="<thing>">                              (per thing)
      <inherit name="editor_object" />
      <scenery />                                         (if IsScenery)
      <feature_container />                               (if IsContainer)
      <inherit name="container_closed" />
      <inherit name="plural" />                           (if Forceplural==Plural)
      <inherit name="{named,}female|male" />             (person + gender + proper)
      <alias><DisplayName></alias>
    </object>
  </object>
```
- `Width`/`Height` are divided by 32 (grid cells). Colors via `ColorTranslator.ToHtml(...)` -> `#RRGGBB`.
- **Direction aliases** (`toQuestPropertyName`, `:116`): `north, south, east, west, northeast, northwest, southeast, southwest, up, down, in, out`. The `<inherit>` name is `<dir>direction` (e.g. `northdirection`).
- **Exits:** all emitted (no `Exported` skip). **Things** are listed flat under the room (containment not nested).
- **No XML escaping** of names/aliases/descriptions/notes — raw text is interpolated into XML (a real risk for `&`, `<`, `>`; reproduce or, for a robust port, note this as a bug to fix).

**`QuestExporter`** adds:
- **Header** (`:160`): `<asl version="550">`, `<include ref="English.aslx"/>`, `<include ref="Core.aslx"/>`, then a `<game>` block with `<gameid>` (a fresh `Guid`), `<version>1.0</version>`, `<firstpublished>{current year}</firstpublished>`, `<author>`, `<description>`.
- **Footer** (`:153`): after the base content, emits `</asl>`.

- **`GetExportName`** (`:96`): `stripOddCharacters` keeps `A-Za-z0-9_`; empty -> `"item"`; suffix appended directly.

---

### 7. TADS (`Export/Languages/TadsExporter.cs`)

- **Target:** TADS 3 — either **adv3Lite** or **adv3**, controlled by `ApplicationSettingsController.AppSettings.SaveTadstoAdv3Lite` (default `true`, `Domain/AppSettings/ApplicationSettingsController.cs:51`).
- **Extensions / filters:** `.t` ("TADS Source Files"), `.txt`. **Title:** `"Export TADS Source Code"`.
- **Reserved words:** `Room, Actor, Thing, Object, Door, Chair, Heavy, Fixture, OpenableContainer, Food, GameMainDef, if, else, me`.

**Header** (`:109`):
```
#charset "us-ascii"

#include <tads.h>          (adv3Lite)          | #include <adv3.h>   (adv3)
#include "advlite.h"                            | #include <en_us.h>

versionInfo : GameID
    name = '<title>'
    byline = 'By <author>'
    version = '1'
    desc = '<description>'
    showAbout()            (if history)
    {
    "<history>";
    }
;
```

**Content** (`:52`):
```
<regionExportName>: Region     (adv3Lite only, per region)
;

<exportName>: {DarkRoom|Room} '<Room.Name>'
    "<PrimaryDescription>"                      (if present)
    regions = [<Room.Region>]                   (adv3Lite + non-default region)

    <dir> = <target>                            (per best exit; preceded by a blank line)
;

<+...> <thing>: {Container|Thing} '<vocab>' '<vocab>'   (per thing, recursively)
;

me: Actor
    location = <startRoom>       (extra start -> "/( ... is an extra StartRoom. /*"; fallback firstRoom; or "/* location = ... */")
;

gameMain: GameMainDef
    initialPlayerChar = me
;
```
- **Direction property names** (`toTadsPropertyName`, `:180`): `north, south, east, west, northeast, northwest, southeast, southwest, up, down, in, out`.
- **Room type:** `DarkRoom` if `IsDark` else `Room`.
- **Regions** (adv3Lite): each region declared as `<name>: Region ;`; a room's `regions = [<Room.Region>]` uses the raw region display name.
- **Exits:** all emitted (no `Exported` skip). Both halves of two-way appear in their rooms.
- **Things** (`exportThings`, `:154`): `+` repeated per nesting level; class `Container` if the thing has contents else `Thing`; the sanitized display name appears **twice** (format `"{0} {1}: {3} {2} {2}"` -> `+ book: Thing 'book' 'book'`) — the first as vocabulary, the second as name.
- **`GetExportName`** (`:131`): `stripOddCharacters` keeps `A-Za-z0-9_`; room empty -> `"room"`, object empty -> `"item"`; suffix appended directly.
- **String escaping** (`toTadsString`, `:212`): the active quote char is backslash-escaped (`'` -> `\'` or `"` -> `\"`), wrapped in that quote.

---

### 8. ZIL (`Export/Languages/ZilExporter.cs`)

- **Target:** ZIL (Infocom / ZILF).
- **Extensions / filters:** `.zil` ("ZIL Source File"), `.txt`. **Title:** `"Export ZIL Source Code"`.
- **Reserved words:** `object, objects`.

**Header** (`:83`) — determines the start room first (`IsStartRoom` room by ID, else first location): emits a `"<title> main file"` comment, `<VERSION ZIP>`, `<CONSTANT RELEASEID 1>`, a `GAME-BANNER` constant, a full `<ROUTINE GO () ...>` main loop that sets `HERE` to the start room and calls the parser loop, `<INSERT-FILE "parser">`, an optional `<SYNTAX ABOUT = V-ABOUT>` + `<ROUTINE V-ABOUT ...>` block (if history), then the `"Objects"` comment.

**Content** (`:31`), per location:
```
<ROOM <exportName>
    (DESC "<Room.Name>")
    (IN ROOMS)
    (LDESC "<PrimaryDescription>")                    (if present)
    (<DIR> TO <target>)                               (per normal exit)
    (<DIR> PER TRIZBORT-CONDITIONAL-EXIT)             (per conditional/dashed exit)
    (FLAGS LIGHTBIT)                                  (only if NOT IsDark)
>

<ROUTINE TRIZBORT-CONDITIONAL-EXIT ()                 (emitted once, after first room needing it)
    <TELL "An export nymph appears on your keyboard. ...'Bye!'" CR>
    <RFALSE>>

<OBJECT <thing>                                       (per thing, recursively)
    (IN <room-or-container>)
    (DESC "<DisplayName>")
    (SYNONYM <LASTWORD>)                              (if any words)
    (ADJECTIVE <OTHER WORDS>)                         (if >1 word)
    (FLAGS TAKEBIT[ VOWELBIT][ CONTBIT])>
```
- **Directions** (`toZILPropertyName`, `:211`): `NORTH, SOUTH, EAST, WEST, NE, SE, SW, NW, UP, DOWN, IN, OUT`.
- **Light:** `(FLAGS LIGHTBIT)` is written when the room is **not** dark.
- **Conditional exits:** dashed connections emit `(<DIR> PER TRIZBORT-CONDITIONAL-EXIT)` and the shared routine is emitted inline the first time one is needed.
- **Exits:** although `IsReciprocated` is checked and the reciprocal's `Exported` flag is set (`:55-57`), **`ExportContent` never checks `Exported`** — so both halves of a two-way connection are emitted in their respective rooms. (The `Exported` bookkeeping is effectively inert here.)
- **Things** (`exportThings`, `:160`): `(IN <Location.ExportName>)` when top-level, else `(IN <Container.ExportName>)` (containment *is* represented here, unlike Hugo). Vocabulary from `getObjectWords` (`:194`): split display name on spaces, each `stripOddCharacters` + uppercased; last word -> `SYNONYM`, remaining -> `ADJECTIVE`. Flags: always `TAKEBIT`, plus `VOWELBIT` if the display name starts with a vowel, plus `CONTBIT` if it has contents.
- **`GetExportName`** (`:120`): **rooms** — `Name.ToUpper()` with spaces -> `-`; if a suffix is requested, or the name contains a reserved word, or contains odd characters, then `stripOddCharacters(name.Replace(" ", "-"))` (keeps space/`-`/alphanumerics); suffix -> `{name}-{suffix}` (**hyphen**-joined). **objects** — `stripOddCharacters(name)` then `ToUpper()` with spaces -> `-`; empty -> `"item"`; suffix -> `{name}{suffix}` (**no hyphen** — inconsistent with rooms).
- **String escaping** (`toZILString`, `:242`): `\n` -> `|`, `"` -> `\"`, wrapped in double quotes.

---

## Cross-cutting notes for the port

1. **Direction ordering is load-bearing.** All exits are emitted by iterating `Directions.AllDirections` in the `MappableDirection` enum order (N, S, E, W, NE, SE, SW, NW, Up, Down, In, Out). Reproduce this order exactly; it affects which of multiple competing exits is chosen/emitted first.
2. **Best-exit selection** must reproduce the priority rule (cardinal/ordinal dock = 4, intermediate dock = 3, secondary up/down/in/out match = 1, non-match = -1), including the constructor-ordering quirk that makes the negative-priority branches dead code.
3. **Two-way vs one-way** is decided in `findExits`: two `Exit`s vs one. Whether both halves ultimately appear in the output then depends on each exporter's use (or non-use) of the `Exit.Exported` flag: I7, Alan, and I6-doors dedupe; Hugo, Quest, TADS, Zil, and Adventuron effectively emit both halves; Adventuron emits everything as one-way.
4. **Region default** is the literal string `"NoRegion"`; region export order follows `Settings.Regions`.
5. **Object/thing parsing** (indentation -> containment, `[...]` property strings, cross-map-unique naming) is shared and must be replicated before language-specific serialization.
6. **De-duplication** is by case-insensitive comparison, seeded with reserved words plus already-assigned region/room/thing names, using numeric suffixes from 2.
7. **Encoding** is ASCII throughout; several exporters transliterate accents (`Deaccent`) or strip non-ASCII before writing.
8. **Escaping differs per target** and is summarized per section; note that **Quest performs no XML escaping** and **Hugo emits descriptions unescaped**, which a faithful port should preserve (or knowingly fix).

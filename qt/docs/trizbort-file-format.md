<!-- Copyright (c) 2026 Jason Self <j@jxself.org>. GPLv3-or-later; see qt/COPYING. -->

# The `.trizbort` file format

Derived from sample maps and the C# reference implementation
(`Domain/Application/LegacyMapFileEngine.cs`, `Util/XmlScribe.cs`,
`Util/XmlElementReader.cs`, and the `BeginLoad`/`Save` methods of
`Domain/Elements/Room.cs`, `Connection.cs`, and `Domain/Misc/Region.cs`).
It is a data-format description, not copied code.

A `.trizbort` file is UTF-8 XML (with a BOM, as written by the C# app). Top
level:

```xml
<?xml version="1.0" encoding="utf-8"?>
<trizbort version="1.5.8.7">
  <info> ... </info>          <!-- optional map metadata; may be <info /> -->
  <map> ... </map>            <!-- rooms and connections -->
  <settings> ... </settings>  <!-- appearance + per-map preferences -->
</trizbort>
```

- `trizbort/@version` — the Trizbort version that wrote the file. Informational;
  readers should not reject unknown versions. Newer attributes (e.g. room shape
  flags) are simply absent in files written by older versions — **treat every
  attribute below as optional with a default** unless noted.

## `<info>` — map metadata

Child elements, any of which may be absent:

| Element | Meaning |
|---|---|
| `<title>` | Map/game title |
| `<author>` | Author |
| `<description>` | Description |
| `<history>` | "About"/history text |

## `<map>` — rooms and connections

An ordered sequence of `<room>` and `<line>` elements. Order corresponds to
element id / creation order.

### `<room>`

```xml
<room id="1" name="Foggy Room" subtitle="" x="0" y="0" w="96" h="64"
      region="NoRegion" handDrawn="no" allcornersequal="no" ellipse="no"
      roundedCorners="no" octagonal="no" cornerTopLeft="0" cornerTopRight="0"
      cornerBottomLeft="0" cornerBottomRight="0" borderstyle="Solid"
      description="" roomFill="" secondFill="" secondFillLocation="Bottom"
      roomBorder="" roomLargeText="" roomSmallText="#FF0000">
  <objects>Ruddy Bell</objects>
</room>
```

| Attribute | Type | Default | Notes |
|---|---|---|---|
| `id` | int | — | Unique element id, referenced by `<dock>` |
| `name` | string | "" | Room name |
| `subtitle` | string | "" | |
| `x`, `y` | float | — | Room's top-left corner, in map units (confirm against `Room.cs`) |
| `w`, `h` | float | 96, 64 | Room size in map units |
| `region` | string | `NoRegion` | Region name (see `<settings>/<regions>`) |
| `borderstyle` | enum | `Solid` | `Solid` \| `Dashed` \| `None` |
| `description` | string | "" | |
| `handDrawn`, `allcornersequal`, `ellipse`, `roundedCorners`, `octagonal` | yes/no | no | Shape flags; absent in older files |
| `cornerTopLeft`, `cornerTopRight`, `cornerBottomLeft`, `cornerBottomRight` | float | 0 | Per-corner rounding radius |
| `roomFill`, `secondFill` | color | "" | "" = inherit map default; two-tone fill |
| `secondFillLocation` | enum | `Bottom` | `Bottom` \| `Top` \| `Left` \| `Right` (position of the second fill) |
| `roomBorder`, `roomLargeText`, `roomSmallText` | color | "" | Per-room overrides; "" = inherit |

Child `<objects>`: the room's object list as text; multiple objects are
separated by newlines (the C# app also recognizes a leading marker for object
placement, preserved verbatim).

### `<line>` (connection)

```xml
<line id="19" style="dashed" flow="oneWay" name="" description=""
      startText="down" midText="" endText="up" color="#FF0000">
  <dock index="0" id="1" port="n" />
  <dock index="1" id="2" port="s" />
</line>
```

| Attribute | Type | Default | Notes |
|---|---|---|---|
| `id` | int | — | Unique element id |
| `style` | enum | `solid` | `solid` \| `dashed` |
| `flow` | enum | `twoWay` | `twoWay` \| `oneWay` (direction goes vertex[0] → vertex[last]) |
| `name`, `description` | string | "" | |
| `startText`, `midText`, `endText` | string | "" | Labels at the start / middle / end of the connection (e.g. `up`/`down`) |
| `color` | color | "" | "" = inherit map line color |

Children are the connection's ordered vertices — **two or more**. A vertex is
either:

- `<dock index="N" id="R" port="P" />` — anchored to room `R` at compass port
  `P`. Ports observed: `n s e w ne nw se sw` (Trizbort's full compass set also
  includes the 16-point intermediates such as `nne`, `ene`; readers should
  accept any and fall back to the nearest edge point).
- `<point index="N" x="X" y="Y" />` — a free vertex at map coordinates.

`index` gives vertex order. A connection whose resolved vertex list does not
have at least two endpoints is invalid and should be dropped (matches the C#
fix for orphaned connections, #575).

## `<settings>` — appearance and per-map preferences

```xml
<settings>
  <colors>
    <canvas>White</canvas> <fill>White</fill> <border>MidnightBlue</border>
    <line>MidnightBlue</line> <selectedLine>Gold</selectedLine>
    <hoverLine>DarkOrange</hoverLine> <largeText>MidnightBlue</largeText>
    <smallText>MidnightBlue</smallText> <lineText>MidnightBlue</lineText>
    <grid>#F5F5F5</grid>
  </colors>
  <regions>
    <Green Name="Green" TextColor="MidnightBlue">Lime</Green>
    <NoRegion Name="NoRegion" TextColor="Black">White</NoRegion>
  </regions>
  <fonts>
    <room size="13">Comic Sans MS</room>
    <object size="11">Comic Sans MS</object>
    <line size="9">Comic Sans MS</line>
  </fonts>
  <grid>   <snapTo>yes</snapTo> <visible>yes</visible>
           <showOrigin>yes</showOrigin> <size>32</size> </grid>
  <lines>  <width>2</width> <handDrawn>yes</handDrawn>
           <arrowSize>12</arrowSize> <textOffset>4</textOffset> </lines>
  <rooms>  <darknessStripeSize>24</darknessStripeSize> <objectListOffset>4</objectListOffset>
           <connectionStalkLength>32</connectionStalkLength>
           <preferredDistanceBetweenRooms>64</preferredDistanceBetweenRooms> </rooms>
  <ui>     <handleSize>12</handleSize> <snapToElementSize>16</snapToElementSize> </ui>
  <keypadNavigation> <creationModifier>control</creationModifier>
                     <unexploredModifier>alt</unexploredModifier> </keypadNavigation>
</settings>
```

Each `<regions>` child is named by the region name (element tag == region name),
with `@Name`, `@TextColor`, and the element text being the region's fill color.

## Color values

Either a .NET named color (`White`, `MidnightBlue`, `Gold`, `DarkOrange`, …) or
`#RRGGBB` hex (`#F5F5F5`). An empty string means "inherit the map default."
A C++ reader needs a small name→RGB table for the common named colors; unknown
names should fall back gracefully (e.g. to the inherited default).

## Coordinate system

Map units; the on-disk grid `size` is 32 by default. `(x, y)` is a room's
top-left corner and `(w, h)` its size. y increases downward (screen-style).
Compass ports map to points on the room's bounding rectangle (n = top-center,
ne = top-right corner, e = right-center, etc.).

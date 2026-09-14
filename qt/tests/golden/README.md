<!-- Copyright (c) 2026 Jason Self <j@jxself.org>. GPLv3-or-later; see qt/COPYING. -->

# Golden exporter corpus

Reference output produced by the **legacy C# Trizbort exporters**, captured once
as fixtures so the C++/Qt port can be validated to produce byte-identical
output. These files are *generated*, not hand-written.

## Layout

```
qt/tests/golden/<map-name>/<map-name>.<ext>
```

One subdirectory per sample map in `samples/`, containing one file per export
format:

| Format | Option | Extension |
|---|---|---|
| Inform 6 | `--inform6` | `.inf` |
| Inform 7 | `--inform7` | `.ni` |
| TADS 3 | `--tads` | `.t` |
| ALAN 3 | `--alan` | `.i` |
| Hugo | `--hugo` | `.hug` |
| ZIL | `--zil` | `.zil` |
| Quest | `--quest` | `.aslx` |
| Adventuron | `--adventuron` | `.adv` |

## How it is regenerated

By the `Golden Corpus` GitHub Actions workflow
(`.github/workflows/golden-corpus.yml`), run manually. It builds the C# app on
a Windows runner and drives its command-line export mode over every sample and
format, then commits the results here. It requires no local Windows machine and
no .NET on a developer's machine.

## How the C++ port uses it

For each fixture, run the corresponding C++ exporter on the same sample and
compare output. Any difference is a port bug (or a deliberate, documented
divergence — see `qt/docs/exporters.md`, which records the C# quirks worth
reproducing versus the ones worth fixing).

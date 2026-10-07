# Asset credits

## Decoration props (CC0 / public domain)

- `props/` (barrel, candelabra, skeleton, gore, ammo_box) come from
  "Oldschool FPS Decoration Sprites":
  https://opengameart.org/content/oldschool-fps-decoration-sprites
- License: CC0 1.0 (public domain) — no attribution required.
- The full pack (39 sprites: keys, treasure, tables, lamps, more gore)
  is worth revisiting for future items.

## Monster sprites (CC0 / public domain)

- `enemy.png` (Evil Oogie), `boss.png` (Demonario) and the full sprite
  sheets in `sprites/` come from "FPS Monster Enemies":
  https://opengameart.org/content/fps-monster-enemies
- License: CC0 1.0 (public domain) — no attribution required.
- The sheets contain walk, attack and death animation frames
  (Demonario also has a fireball attack) usable for future animation.
- `runner.png` (Smorficus, blue demon) is built from `sprites/smorficus.png`:
  4 walk + 3 attack frames from the sheet, death frames synthesized (squash).

## Sounds

- `sounds/night-ambience.wav` (wind + dark pad loop) and
  `day-ambience.wav` (wind only) are synthesized, no license needed.
- `sounds/gun-fire.wav` is an AK-47 shot from "The Free Firearm
  Sound Library": https://opengameart.org/content/the-free-firearm-sound-library
  — CC0 (first shot of take C_28P, cut to 1s, mono 44.1kHz).
- `sounds/gun-reload.wav` is "assaultriflereload1" from "Gun reload
  sounds" by springyspringo:
  https://opengameart.org/content/gun-reload-sounds — CC0
  (time-compressed to 1.1s, mono 44.1kHz WAV).
- `sounds/explosion.wav` is "Chunky Explosion" by Joth:
  https://opengameart.org/content/chunky-explosion — CC0
  (trimmed to 2.5s, normalized, converted to mono 44.1kHz WAV).
- `sounds/footstep.wav`, `footstep2.wav`, `footstep3.wav`,
  `door-open.wav`, `pickup.wav` come from Kenney's "RPG Audio" pack:
  https://kenney.nl/assets/rpg-audio — CC0.
- `sounds/growl.wav` (monster_03), `boss-roar.wav` (roar_02),
  `hurt.wav` (hurt_01), `bite.wav` (eat_03), `die-grunt.wav` (scream_01),
  `die-runner.wav` (scream_02), `die-boss.wav` (monster_04),
  `night-breath.wav` (breath), `night-weird.wav` (weird_05) and
  `night-creature.wav` (howl) come from
  "80 CC0 creature SFX" by rubberduck:
  https://opengameart.org/content/80-cc0-creature-sfx — CC0.
- `sounds/fireball-shot.wav` (spell_fire_02) comes from
  "80 CC0 RPG SFX" by rubberduck:
  https://opengameart.org/content/80-cc0-rpg-sfx — CC0.
- `sounds/howl.wav` is a 7s cut of "Wolf howls.ogg", a U.S. Fish and
  Wildlife Service recording (public domain, PD-US-FWS):
  https://commons.wikimedia.org/wiki/File:Wolf_howls.ogg
- All converted to mono 44.1 kHz WAV and peak-normalized.

## Menu UI

- `button_bg.png` / `button_bg_hover.png` (main/pause/settings/map-select
  button background) are composited from two real photographed CC0
  textures: "Black Metal" (scratched black metal diffuse) and
  "Lava 001" (color + emission maps, real crack/glow pattern), both
  from cc0-textures.com — https://cc0-textures.com/t/st-black-metal and
  https://cc0-textures.com/t/cc0t-lava-001 — CC0, no attribution
  required. The bevel/rounded-corner shape comes from Kenney's "UI Pack"
  (`button_square_depth_gradient`, CC0, https://kenney.nl/assets/ui-pack).
  Ties into the game's existing fire/hell palette (title, background
  gradient, boss fireballs) instead of an unrelated theme; hover/selected
  state brightens the ember cracks and adds an orange rim matching
  COL_TITLE. COL_BTN*/COL_BTN_BORDER* in macros.h retuned to match.
- `fond_game-menu.jpg` (main menu background) replaces a Battlefield 4
  promotional image that had no license for this project and didn't
  match the game's theme (modern soldiers vs. demons/hell); the unused
  second copy `fond_game-menu2.jpg` (same source, same problem) was
  deleted. Went through several failed attempts: a top-down mosaic of
  level5.wolf ("Roblox bricks" — small repeated tiles have no
  perspective), a flat metal+lava material (no longer looked like the
  game once the map idea was on the table), then a per-column DDA
  raycast render (same math as src/raycast.c) that turned out to look
  like plain vertical stripes instead of stone — at close range each
  screen column only samples a near-single texture pixel wide, so the
  real brick/moss detail in `texture_wall_wolf.png` never showed up.
  Dropped the per-column raycasting for a PIL perspective-warp of the
  real wall texture (crisp, correct brick/moss detail) — technically
  fixed but the full-body corridor shot still read as flat/static, not
  worth clicking on. Went movie-poster (bust crop of the boss upscaled
  ~6.7x) but came out blurry: `sfTexture_setSmooth` is applied to the
  whole menu background in src/game-menu/menu.c, so bilinear filtering
  softens any composited element, and the boss sprite is only 180x192
  per frame — stretched that far it turns to mush regardless of resize
  filter. Capped the boss upscale at 4x (crisp again) but the user's
  verdict on all of the above was the same: none of it looked like
  something you'd want to click on, however technically correct.
  Final version drops compositing from game assets entirely and uses a
  real photo instead: "Man in black and brown camouflage uniform
  holding red smoke" by Jakob Owens — https://unsplash.com/photos/Sa04XETPPx0
  — Unsplash License, color-graded toward the game's orange palette.
  Swapped again on request for a second real photo: "Man in black and
  green camouflage suit holding rifle" by Alexander Jawfox —
  https://unsplash.com/photos/R_6kw7NUTLY — Unsplash License, near-black
  studio portrait (helmet lit, face in shadow, rifle in hand) that
  needed no real color grading since it's already dark/neutral; cropped
  from the 2400x3000 portrait original to a proportional 1920x1080
  slice (no stretching).
- `end_bg_win.jpg` / `end_bg_lose.jpg` (victory/defeat screen background)
  are composited the same way from the same two cc0-textures.com sources
  (black scratched metal + real lava crack/emission), embers pushed to
  the screen edges via a vignette so the center stays dark/readable for
  the title, stats and buttons. Win uses an orange ember tint, lose a
  blood-red tint. Replaces the previous flat two-color gradient.

## Generated art

- `texture_sky_night.png` (starry night sky with moon) is generated
  for this project (script), no license needed.
- `texture_wall_secret.png` is `texture_wall_wolf.png` with a
  generated crack (script), same license as the base texture.
- `health_bar.png` (pixel heart + 5-segment bar, 6 frames), `key.png`,
  `texture_wall_brick.png`, `texture_wall_cold.png`, `texture_door.png`
  and `texture_door_locked.png` are generated for this project (scripts),
  no license needed. The previous health bar was a watermarked stock
  preview and was replaced.
- `texture_floor.png` is `texture_wall_wolf.png` (stone) downscaled to
  a power-of-two 256x256 (LANCZOS), same license as the base texture.
- `fireball.png` and `medkit.png` are generated for this project
  (scripts), no license needed.

## Fonts

- `fonts/MetalMania.ttf` ("Metal Mania" by Caroline Hadilaksono) is
  distributed under the SIL Open Font License 1.1, free for
  commercial use: https://fonts.google.com/specimen/Metal+Mania

## Unresolved provenance (TODO)

These assets predate this credits file and their original source
was never recorded. They're real/in-game, not generated — if the
source is ever remembered, replace this entry with a proper credit;
otherwise consider swapping them for a documented CC0 replacement
before relying on this repo publicly.

- `explosion.png` (barrel explosion frames, added 2026-07-07).
- `weapon_idle.png` / `weapon_fire.png` (first-person rifle render).
- `flashlight.png` (HUD flashlight icon).
- `texture_sky.png` (base daytime sky texture).
- `fonts/wolf3d.ttf` (HUD font).
- `sounds/song_game-menu.wav` (main menu music).

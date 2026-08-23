# Neon Drift

A pseudo-3D arcade racer that runs in the browser, on desktop or phone. One
file, no build step, no dependencies, no network requests — open it and drive.

Pick a car and set the grid in the lobby, then race AI rivals around a
procedurally-built closed circuit with curves, crests and dips, drawn in a
sunset-neon palette. Keyboard, touch and gamepad all work.

## Playing it

```bash
open /Users/mubarakazeez/NeonDrift/index.html
```

Or serve it, which is what you want if you're editing:

```bash
python3 -m http.server 8777 --directory /Users/mubarakazeez/NeonDrift
```

Then visit <http://127.0.0.1:8777>.

## The lobby

The start screen sets up the race:

| Setting  | Options                    |
|----------|----------------------------|
| Car      | PULSE · VECTOR · KITE      |
| Rivals   | 3 · 6 · 9                  |
| Laps     | 1 · 3 · 5                  |
| Class    | ROOKIE · PRO · ACE         |

Navigate with **↑↓** to change row and **←→** to change value, d-pad on a
gamepad, or just tap/click. **Enter** or **A** starts the race from anywhere in
the lobby.

The three cars differ in handling, not just paint:

| Car    | Character    | Top speed | Acceleration | Grip |
|--------|--------------|-----------|--------------|------|
| PULSE  | Balanced     | 0.95      | 1.00         | 1.00 |
| VECTOR | Top speed    | 1.00      | 0.88         | 0.86 |
| KITE   | Corner speed | 0.92      | 1.20         | 1.30 |

Grip scales steering authority *and* resists the centrifugal push, so KITE holds
a line VECTOR washes out of. KITE also carries a larger nitro reserve that
drains slower, which is how it makes back the top-speed deficit. Driven well the
three land within a few seconds of each other over a lap.

Class sets rival pace and how hard they rubber-band: ROOKIE is a comfortable
win, ACE will beat you if you make mistakes.

## Controls

### Gamepad

Any standard-mapping controller, hot-plugged — the lobby shows "Controller
connected" when one is seen.

| Action     | Control                        |
|------------|--------------------------------|
| Steer      | Left stick (analog) or d-pad   |
| Throttle   | RT (analog) or A               |
| Brake      | LT (analog) or B               |
| Nitro      | X or RB                        |
| Drift      | LB or Y                        |
| Pause      | Start                          |
| Menus      | D-pad to move, A confirm, B back |

Stick and triggers are genuinely analog — half throttle gives roughly half
acceleration, and a half-deflected stick steers about half as hard.

### Phone

Hold it in landscape — a portrait prompt asks you to rotate. On-screen pads
appear automatically on any touchscreen: **◀ ▶** under your left thumb,
**NITRO**, **DRIFT** and **BRAKE** under your right, pause up top.

**Throttle is automatic on touch** — you accelerate unless you're braking, so
your right thumb stays free for nitro and drift. Since the right thumb handles
one button at a time, the natural rhythm is drift the corner to charge, then
boost the straight.

### Keyboard

| Action      | Key           |
|-------------|---------------|
| Throttle    | `W` / `↑`     |
| Brake       | `S` / `↓`     |
| Steer       | `A` `D` / `←` `→` |
| Nitro       | `Space`       |
| Drift       | `Shift`       |
| Pause       | `Esc`         |
| Mute        | `M`           |
| Start / restart | `Enter`   |

Drifting sharpens your steering and *charges* nitro, so the fast line is to
slide through the corners and cash it in on the straights. Nitro also trickles
back on its own. Best lap persists between sessions.

## How it works

Classic OutRun-style projection — the road is an array of ~2,000 segments, each
with a curve and a height, projected to screen space from a camera that sits a
fixed distance behind the player. Segments are drawn near-to-far with a running
`maxy` clip so crests occlude what's behind them; cars and roadside props are
then drawn far-to-near against each segment's stored clip line. There is no
polygonal 3D and no WebGL — it's all 2D canvas.

- **Track** — `buildTrack()` assembles the circuit from `straight` / `curve` /
  `hill` / `sCurves` / `bumps` primitives, then eases the final section back to
  `y = 0` so the loop joins itself seamlessly.
- **Driving** — fixed 60 Hz integration. Centrifugal force pushes you toward the
  outside of a corner in proportion to speed and curvature; leaving the tarmac
  applies a hard drag term.
- **Rivals** — each looks ~20 segments ahead, steers around slower traffic and
  the player, brakes for curvature, and holds a preferred lane otherwise. A mild
  rubber band (±8%) keeps the pack together without making it obvious.
- **Laps** — a wrap past the start line only counts once at least half the track
  length has actually been covered, so a collision nudge across the line can't
  double-count a lap.
- **Fitting the screen** — the canvas is a fixed 1280×720 letterboxed with an
  explicit `translate(...) scale(...)`. Centring an oversized box with grid or
  flex does *not* work: browsers start-align an item larger than its container
  rather than clipping both edges. The touch overlay deliberately sits outside
  the scaled wrapper, since a transformed ancestor becomes the containing block
  for `position: fixed` and would shrink the buttons along with the canvas.
- **Mobile budget** — phones drop to 220 drawn segments and skip the `shadowBlur`
  glow passes, which are the expensive part on mobile fill rates.
- **Input funnel** — keyboard, touch and gamepad all resolve into one analog
  `{steer, throttle, brake, nitro, drift}` shape in `readInput()`, so the physics
  never branches on device. A connected pad overrides the keyboard when its
  stick leaves the dead zone.
- **What scales and what doesn't** — the HUD lives inside the scaled canvas
  wrapper because it belongs to the game view; the menus and touch pads sit
  outside it, at real pixel size. A menu that scales with the canvas is
  unreadable on a phone, and a `position: fixed` button inside a transformed
  ancestor is sized by that ancestor, not the viewport.
- **Collision sweep** — at boost a rival can be crossed within one frame, so
  collisions test every segment the player passed through, not just the one they
  landed in.

## Debug handle

The page exposes `window.__ND` for stepping the simulation without
`requestAnimationFrame` — handy for tuning, and for testing in a headless or
backgrounded tab where rAF is suspended:

```js
__ND.start();            // begin a race with the current lobby setup
__ND.step(4);            // advance 4 simulated seconds
__ND.key('up', true);    // hold throttle
__ND.info();             // { state, lap, place, kmh, x, lapTime, best, order, ... }

__ND.lobby();            // back to the lobby
__ND.setup.car = 2;      // indices into CARS / RIVAL_OPTS / LAP_OPTS / CLASSES
__ND.move(1);            // move the focused lobby row
__ND.adjust(-1);         // change the focused row's value
__ND.ui();               // poll the gamepad once (menu edges included)
__ND.config();           // { car, cls, laps, rivals, row, state }
```

## Known limitations

- Gamepad support assumes the browser's **standard mapping**. A pad that doesn't
  report `mapping: "standard"` will have its buttons in the wrong places, and
  there's no remapping UI.
- Only the first connected controller is read — there's no local multiplayer.
- Touch detection keys off `pointer: coarse` + `hover: none`. A touchscreen
  laptop reports both and will get the on-screen pads; append `?touch=1` to
  force them on, which is also how you preview the mobile layout on a desktop.
- All seven cars share one procedurally-drawn body; they differ by colour and
  physics stats, not shape.
- Engine audio is a two-oscillator synth, not a sample.
- One track. The layout is fixed in `buildTrack()` rather than seeded/random.

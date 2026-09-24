# 🎬 Custom Movie Poster Marquee Controller Manual

This is a a quick-reference guide for the custom **96-LED Movie Poster Marquee System**. It includes hardware specifications, user interface shortcuts, and a complete indexed directory of the **35 integrated display patterns**.

---

## 🎛️ Encoder Control Operations

The rotary encoder utilizes a unified single-button state machine to switch configurations cleanly.

| Physical Action | Mode / State | Operational Result |
| :--- | :--- | :--- |
| **Standard Turn** | *Context Dependent* | Adjusts active parameters or navigates menus (see below). |
| **Single Click** | **Cycle System Modes** | Steps through the 3 operational adjustments sequentially:<br>📦 **State 0:** Speed Tempo Control<br>🔆 **State 1:** Brightness Control<br>🗂️ **State 2:** Display Selector Menu |
| **Double Click** | **Direction Reverse** | Instantly flips the marquee spin direction (Forward / Backward). *(Only during pattern playback)* |
| **Long Press (1.5s)** | **Master Power Toggle** | Safely sleeps or awakens the system. Turns all 96 LEDs completely off. |

### Standard Turning Actions per Mode:
* **In Speed Adjustment Mode (State 0):** **Clockwise** speeds up animations (decreases delay); **Counter-Clockwise** slows them down (increases delay).
* **In Brightness Adjustment Mode (State 1):** Dynamically scales the sign's output. Built-in fine tuning automatically dials by **15-step increments** above 20 brightness, and switches to safe **2-step micro-adjustments** below 20.
* **In Selection Mode (State 2):** **Clockwise** scrolls forward up your pattern list; **Counter-Clockwise** scrolls backward down the list. Use the light pattern to show which item is sselected. Number of light lit shows the current display choice by number.

---

## 🗂️ Display Pattern Reference Guide

When you enter **Selection Mode (State 2)**, white tracking dots will light-up cumulatively. Use the index mapping below to find the number of lights showing the current selected display pattern.

### 🏛️ Classic Cinematic Marquees
* **Index 01: `marqueeTheaterGold`**
  Classic warm cinema look. Isolated moving bulbs with wide dark spaces.
* **Index 02: `marqueeChristmas`**
  Alternating festive Christmas red and green blocks dancing around the frame.
* **Index 03: `marqueeRainbow`**
  Vibrant spectrum marquee shifting rainbow colors smoothly.
  
### 🎓 Regional University Marquees
* **Index 04: `marqueeMazeBlue`**
  Maize and blue University of Michigan rotating colors. Adjust brightness to adjust to the correct hue. 
* **Index 05: `marqueeGreenWhite`**
  Michigan State green and white bulb layout rotating colors.  Adjust brightness to adjust to the correct hue. 
* **Index 06: `marqueePolice`**
  High-intensity alternating red and blue emergency-style sequence.
* **Index 07: `marqueeWayneState`**
  Traditional theater layout tuned to Wayne State University colors - Deep Forest Jade and Gold. Adjust brightness to adjust to correct the hue.
* **Index 08: `marqueeCMU`**
  Central Michigan layout using crisp Maroon and Gold. Adjust brightness to adjust to correct the hue.
* **Index 09: `marqueeOakland`**
  Oakland University layout using Golden Grizzlies Gold and dark, near black or turned-off alternating leds. Adjust brightness to adjust to correct the hue.

### 🏃 Legacy & Kinetic Animations
* **Index 10: `chase`**
  Inline pixel chaser tracking around the perimeter.
* **Index 11: `chaseit`**
  High-speed variant variant of the standard layout chaser sequence.
* **Index 12: `chaseone`**
  A single, sharp trailing pixel tracking around the rectangle frame.
* **Index 13: `acceleratingCylon`**
  Scanner eye that builds momentum as it bounces back and forth across the perimeter.
* **Index 14: `rainbow`**
  Full fluid spectrum cycling across all 96 LEDs uniformly.
* **Index 15: `rainbowWithGlitter`**
  Standard rainbow framework layered with spontaneous high-intensity white sparkles.
* **Index 16: `confetti`**
  Random pops of fading colors appearing organically up and down the channels.
* **Index 17: `sinelon`**
  A smooth, sweeping neon point that leaves a tapering trail behind it.
* **Index 18: `juggle`**
  Multiple independent color nodes rhythmically pulsing in step with each other.
* **Index 19: `bpm`**
  Saturated pulsing light bars synchronized to a rhythmic bpm tempo grid.
* **Index 20: `pacifica_loop`**
  Advanced multi-layered simulation mimicking rich, rolling deep-ocean waves.
* **Index 21: `pride`**
  Slow-moving, layered spectrum animation displaying a rich color layout.
* **Index 22: `cylon`**
  Traditional single-node scanner bar shifting side-to-side (around) the display.
* **Index 23: `multiCylon`**
  Multiple intersecting scanning points passing over each other across the frame.

### 🍿 Specialized Theater Layouts
* **Index 24: `marqueeHollywood`**
  Baseline warm glow punctuated by random high-speed white flashbulbs.
* **Index 25: `MaizeAndBlueChase`**
  Solid chasing blocks of deep University of Michigan Maize and Blue. Adjust brightness to adjust to the correct hue. 
* **Index 26: `GreenAndWhiteChase`**
  Saturated alternating block chase using Spartan Green and White. Adjust brightness to adjust to the correct hue. 
* **Index 27: `WayneChase`**
  Optimized solid block chase utilizing Wayne State Forest Emerald and Canary Gold. Adjust brightness to adjust to the correct hue. 
* **Index 28: `CMUChase`**
  High-contrast Central Michigan block chase using pure Crimson Burgundy and Amber. Adjust brightness to adjust to the correct hue. 
* **Index 29: `OaklandChase`**
  Solid Golden Grizzlies block chase using deep Amber and simulated Midnight Obsidian Blue. Adjust brightness to adjust to the correct hue. 
* **Index 30: `ticketChase`**
  Premium vintage marquee layout featuring moving dashes separated by long blank blocks.
* **Index 31: `carnivalChase`**
  High-energy three-color sequence featuring blocks of Red, White, and Gold chasing sequentially.
* **Index 32: `curtainsOpen`**
  Symmetrical horizon pattern starting simultaneously at all 4 corners and meeting in the center.

### 🌌 Organic Sky Animations
* **Index 33: `twinkleStar`**
  Random individual stars gently fading up to unique peak brightness limits and melting away.
* **Index 34: `shootingStar`**
  Quiet star twinkle sky interrupted by blazing-fast, anti-aliased meteors with liquid decay trails.
* **Index 35: `solid`**
  Solid static backdrop fill. Great for steady ambient illumination or checking strip integrity.

---

## 🔒 Master System Safety Bounds
These constants are locked into your code architecture to protect your power supply, keep your controller running cool, and prevent color clipping.

* **Hardware Amperage Limit:** `3,000 mA (3.0 Amps)` (Preserves a safe 600mA buffer on a minimum required 3.6A wall brick)
* **Maximum Brightness Ceiling:** `220 / 255` (Prevents high-power washing and holds deep color tinting)
* **Minimum Brightness Floor:** `1 / 255` (Ensures the display stays visible and active during extreme dimming). Use long press to turn all LEDs off or back on. 
* **Hardware Protection:** `470 Ω Resistor` (Pin 3 data line protector) & `1000 µF Capacitor` (Barrel jack voltage cushion)

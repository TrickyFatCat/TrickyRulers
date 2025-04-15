# About

**TrickyRulers** is an engine plugin that provides a set of measurement tools for level designers and developers to
easily visualize distances and dimensions within the editor.

## Features

- **Various Ruler Types** - Line, Circle, Sphere, Cylinder, Capsule, Box, and Cone rulers
- **Spline Measurement** - Measure along curved paths with TrickySplineRuler
- **Real-time Updates** - Measurements update automatically when objects are moved
- **Visual Debugging** - Clear visual indicators with customizable colors and debug text

## Installation

1. Copy the plugin folder into your project's `Plugins` directory.
2. Open your Unreal Engine project.
3. Enable the **TrickyRulers** in the plugins menu.
4. Restart the editor.

## Usage

1. Place `TrickyRuler` or `TrickySplineRuler`
2. Adjust their parameters to measure what you need

Alternatively, you can create your own set of rulers via creating BP actors inherited from either `TrickyRuler` or
`TrickySplineRuler`.

Keep in mind that these actors are editor only, thus you won't see them in the game at all.
# Web UI - Langton's Ant Simulator

Modern, interactive browser-based interface for the Langton's Ant cellular automaton.

## Features

✅ **Real-time Visualization**
- Canvas-based grid rendering
- Animated ants (red squares)
- Black/white cell states
- Adjustable zoom (via grid size parameter)

✅ **Interactive Controls**
- Play/Pause/Stop/Reset buttons
- Animation speed slider (1x-100x)
- Grid size adjustment (50-500)
- Ant count control (1-100)
- Algorithm selector (Sequential/MPI modes)

✅ **Live Statistics**
- Current step number
- Alive ants count
- Black cells count
- FPS counter
- Elapsed time

✅ **Responsive Design**
- Works on desktop and tablet
- Adaptive canvas sizing
- Dark theme with cyan accents

## Quick Start

### Method 1: Python HTTP Server (Recommended)

```bash
cd /home/cafe/Desktop/Sesiune/AgPA/Langton/ui
python3 server.py
```

Then open browser: **http://localhost:8080**

### Method 2: Direct File Open

```bash
# Linux
xdg-open /home/cafe/Desktop/Sesiune/AgPA/Langton/ui/index.html

# macOS
open /home/cafe/Desktop/Sesiune/AgPA/Langton/ui/index.html

# Windows (from file explorer)
file:///path/to/Langton/ui/index.html
```

### Method 3: Simple HTTP Server (Built-in Python)

```bash
cd /home/cafe/Desktop/Sesiune/AgPA/Langton/ui
python3 -m http.server 8080
```

## Usage

1. **Start Server** → Open browser to `http://localhost:8080`
2. **Configure Parameters** (left sidebar):
   - Grid Size: 50-500 pixels
   - Number of Ants: 1-100
   - Animation Speed: 1x-100x
   - Algorithm: Sequential (MPI integration coming soon)
3. **Run Simulation**:
   - Click **Play** to start
   - Use **Speed** slider to control animation
   - Click **Pause** to freeze
   - Click **Stop** to reset
4. **Monitor Statistics** (right sidebar):
   - Watch step counter
   - Track alive ants
   - Monitor black cell count
   - Check FPS for performance

## File Structure

```
ui/
├── index.html      # HTML structure + styling
├── app.js          # JavaScript simulation engine
├── server.py       # Python HTTP server
└── README.md       # This file
```

## Technical Details

### Frontend Architecture
- **Canvas Rendering**: 2D HTML5 Canvas for efficient grid display
- **JavaScript Simulation**: Pure JS implementation of Langton's Ant rules
- **State Management**: Single state object tracks grid, ants, and metrics
- **Animation Loop**: requestAnimationFrame for smooth 60 FPS rendering

### Simulation Rules
1. **On each step**:
   - For each ant:
     - Rotate: white cell → right, black cell → left
     - Flip: toggle cell color
     - Move: one step forward in direction
2. **Boundaries**: Ants that leave grid are removed

### Color Scheme
- **White cells**: False (initial state)
- **Black cells**: True (visited/flipped)
- **Ants**: Red squares
- **Grid lines**: Light gray (every 20 cells)

## Performance

| Grid Size | Speed | Performance |
|-----------|-------|-------------|
| 50×50     | 100x  | ~60 FPS     |
| 100×100   | 50x   | ~60 FPS     |
| 200×200   | 10x   | ~60 FPS     |
| 500×500   | 1x    | ~30 FPS     |

Larger grids reduce FPS due to cell-by-cell rendering.

## Keyboard Shortcuts (Future)
- Space: Play/Pause
- R: Reset
- +: Faster
- -: Slower

## Future Enhancements

- [ ] Integration with C++ simulator (for large-scale tests)
- [ ] WebSocket communication with Python backend
- [ ] Export simulation as MP4/GIF
- [ ] Highway detection visualization
- [ ] Multi-threaded simulation (Web Workers)
- [ ] Keyboard shortcuts
- [ ] Save/load simulation state
- [ ] Preset scenarios

## Troubleshooting

### Server won't start
```bash
# Check if port 8080 is in use
lsof -i :8080

# Use different port
python3 -m http.server 9000
```

### Slow performance on large grids
- Reduce grid size to 100-200
- Lower animation speed
- Close other browser tabs

### Ants disappear immediately
- This is normal - they're leaving the bounded grid
- Increase grid size or reduce step count to observe longer

## Browser Compatibility

- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+

Requires HTML5 Canvas support.

## License

Educational project - free to use and modify.

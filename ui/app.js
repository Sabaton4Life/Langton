// Simulation state
let state = {
    grid: [],
    ants: [],
    step: 0,
    running: false,
    gridSize: 200,
    numAnts: 5,
    speed: 10,
    algorithm: 'sequential',
    startTime: 0,
    fps: 0,
    frameCount: 0,
    lastFpsTime: 0
};

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

// Initialize simulation
function init() {
    state.gridSize = parseInt(document.getElementById('gridSize').value);
    state.numAnts = parseInt(document.getElementById('numAnts').value);
    state.grid = Array(state.gridSize).fill(null).map(() => Array(state.gridSize).fill(false));
    state.ants = [];
    state.step = 0;
    state.startTime = Date.now();
    state.frameCount = 0;

    const centerX = Math.floor(state.gridSize / 2);
    const centerY = Math.floor(state.gridSize / 2);

    for (let i = 0; i < state.numAnts; i++) {
        state.ants.push({
            x: centerX + (i % 3) - 1,
            y: centerY + Math.floor(i / 3) - 1,
            dir: 0 // 0=N, 1=E, 2=S, 3=W
        });
    }

    resizeCanvas();
    draw();
    updateStats();
}

// Resize canvas to fit container
function resizeCanvas() {
    const container = canvas.parentElement;
    const size = Math.min(container.clientWidth - 20, container.clientHeight - 20);
    canvas.width = size;
    canvas.height = size;
}

// Draw grid and ants
function draw() {
    const pixelSize = canvas.width / state.gridSize;

    // Clear
    ctx.fillStyle = 'white';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Draw black cells
    ctx.fillStyle = 'black';
    for (let r = 0; r < state.gridSize; r++) {
        for (let c = 0; c < state.gridSize; c++) {
            if (state.grid[r][c]) {
                ctx.fillRect(c * pixelSize, r * pixelSize, pixelSize, pixelSize);
            }
        }
    }

    // Draw ants
    ctx.fillStyle = 'red';
    for (const ant of state.ants) {
        if (ant.x >= 0 && ant.x < state.gridSize && ant.y >= 0 && ant.y < state.gridSize) {
            ctx.fillRect(ant.x * pixelSize + 1, ant.y * pixelSize + 1, pixelSize - 2, pixelSize - 2);
        }
    }

    // Draw grid lines (optional, thin)
    ctx.strokeStyle = '#eee';
    ctx.lineWidth = 0.5;
    for (let i = 0; i <= state.gridSize; i += Math.ceil(state.gridSize / 20)) {
        ctx.beginPath();
        ctx.moveTo(i * pixelSize, 0);
        ctx.lineTo(i * pixelSize, canvas.height);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(0, i * pixelSize);
        ctx.lineTo(canvas.width, i * pixelSize);
        ctx.stroke();
    }
}

// Simulate one step
function step() {
    const newAnts = [];

    for (const ant of state.ants) {
        if (ant.x < 0 || ant.x >= state.gridSize || ant.y < 0 || ant.y >= state.gridSize) {
            continue; // Ant left grid
        }

        const isBlack = state.grid[ant.y][ant.x];

        if (isBlack) {
            ant.dir = (ant.dir + 3) % 4; // Rotate left
        } else {
            ant.dir = (ant.dir + 1) % 4; // Rotate right
        }

        state.grid[ant.y][ant.x] = !state.grid[ant.y][ant.x]; // Flip

        // Move forward
        const dirs = [[0, -1], [1, 0], [0, 1], [-1, 0]]; // N, E, S, W
        const [dx, dy] = dirs[ant.dir];
        ant.x += dx;
        ant.y += dy;

        // Keep ant if still in bounds (or barely out)
        newAnts.push(ant);
    }

    state.ants = newAnts;
    state.step++;
}

// Update statistics
function updateStats() {
    document.getElementById('stepStat').textContent = state.step;
    document.getElementById('antsStat').textContent = state.ants.filter(a =>
        a.x >= 0 && a.x < state.gridSize && a.y >= 0 && a.y < state.gridSize
    ).length;

    let blackCount = 0;
    for (let r = 0; r < state.gridSize; r++) {
        for (let c = 0; c < state.gridSize; c++) {
            if (state.grid[r][c]) blackCount++;
        }
    }
    document.getElementById('blackStat').textContent = blackCount;

    const elapsed = (Date.now() - state.startTime) / 1000;
    document.getElementById('timeStat').textContent = elapsed.toFixed(1) + 's';

    // FPS
    state.frameCount++;
    if (Date.now() - state.lastFpsTime > 500) {
        state.fps = Math.round((state.frameCount * 1000) / (Date.now() - state.lastFpsTime));
        document.getElementById('fpsStat').textContent = state.fps;
        state.frameCount = 0;
        state.lastFpsTime = Date.now();
    }
}

// Animation loop
let animationId = null;

function animate() {
    if (!state.running) {
        animationId = null;
        return;
    }

    // Run multiple steps per frame based on speed
    for (let i = 0; i < state.speed; i++) {
        step();
    }

    draw();
    updateStats();

    animationId = requestAnimationFrame(animate);
}

// Controls
function play() {
    if (state.ants.length === 0) init();
    state.running = true;
    document.getElementById('playBtn').disabled = true;
    document.getElementById('pauseBtn').disabled = false;
    document.getElementById('status').textContent = 'Running...';
    document.getElementById('status').classList.add('running');
    if (!animationId) animate();
}

function pause() {
    state.running = false;
    document.getElementById('playBtn').disabled = false;
    document.getElementById('pauseBtn').disabled = true;
    document.getElementById('status').textContent = 'Paused';
    document.getElementById('status').classList.remove('running');
}

function stop() {
    state.running = false;
    init();
    document.getElementById('playBtn').disabled = false;
    document.getElementById('pauseBtn').disabled = true;
    document.getElementById('status').textContent = 'Stopped';
    document.getElementById('status').classList.remove('running');
}

function reset() {
    stop();
    document.getElementById('status').textContent = 'Ready';
}

// Parameter updates
function updateParam(name, value) {
    switch (name) {
        case 'gridSize':
            state.gridSize = parseInt(value);
            document.getElementById('gridSizeLabel').textContent = value + ' × ' + value;
            stop();
            break;
        case 'numAnts':
            state.numAnts = parseInt(value);
            document.getElementById('numAntsLabel').textContent = value + ' ants';
            stop();
            break;
        case 'speed':
            state.speed = parseInt(value);
            document.getElementById('speedLabel').textContent = value + 'x';
            break;
        case 'algorithm':
            state.algorithm = value;
            console.log('Algorithm changed to:', value);
            // Future: implement actual MPI selection
            break;
    }
}

// Initialize on load
window.addEventListener('load', () => {
    init();
    document.getElementById('status').textContent = 'Ready';
});

// Handle window resize
window.addEventListener('resize', () => {
    resizeCanvas();
    draw();
});

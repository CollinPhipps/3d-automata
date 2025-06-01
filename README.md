# 3D Cellular Automata Simulation

A real-time 3D cellular automaton simulation built with OpenGL, featuring customizable rules and interactive controls.

![3D Cellular Automata Demo](Simulation.mp4) <!-- Add a screenshot when available -->

## Features

### Customizable Cellular Automaton Rules
- **Birth Requirement**: Set the number of neighbors needed for a dead cell to become alive
- **Survival Requirement**: Set the number of neighbors needed for a live cell to survive
- **Decay Time**: Configure how long cells take to fully die (creates trailing effects)

### Initial Pattern Generation
Choose from multiple initial cell patterns:
- **Random Sphere**: Randomly populated spherical region
- **Random Cube**: Randomly populated cubic region  
- **Symmetric Noise**: Symmetric noise pattern based on coordinate relationships

### Pattern Customization
- **Radius Control**: Adjust the size of initial patterns
- **Fill Percentage**: Control the density of initially alive cells (0-100%)

### Interactive Controls
- **Play/Pause**: Start and stop the simulation
- **Reset**: Return to initial state with current pattern
- **3D Navigation**: Rotate the view using WASD keys
- **Zoom**: Mouse wheel to zoom in/out
- **Real-time Parameter Adjustment**: Change rules and patterns while running

## Technology Stack

- **OpenGL**: 3D graphics rendering
- **GLFW**: Window management and input handling
- **ImGui**: Immediate mode GUI for controls
- **GLM**: Mathematics library for 3D transformations
- **C++**: Core application logic

## Building

### Prerequisites
- CMake 3.15 or higher
- C++17 compatible compiler
- OpenGL 3.3+ support

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/CollinPhipps/3d-automata.git
cd 3d-automata

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run (from build directory)
./3d-automata  # Linux/Mac
# or
3d-automata.exe  # Windows
# ProjetAvion: The Ultimate OpenGL 3D Flight Simulator

![ProjetAvion Banner](https://img.shields.io/badge/Status-Active-success) ![OpenGL](https://img.shields.io/badge/OpenGL-3D_Graphics-blue) ![C++](https://img.shields.io/badge/C++-14%2F17-orange) ![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey)

Welcome to **ProjetAvion**, an immersive, high-performance 3D flight simulator built from scratch using C++ and OpenGL. Originally conceived as a basic graphics rendering project, it has evolved into a fully-fledged infinite flight arcade game featuring a custom physics engine, procedurally generated environments, dynamic lighting, an intelligent autopilot system, and real-time radar mapping.

---

## Massive Feature Set

ProjetAvion boasts an extensive array of features designed to provide a visually stunning and mechanically deep flight experience:

### Procedural Infinite Cityscape
Dive deep below the clouds and fly over a sprawling, procedurally generated metropolis!
- **Endless Horizon:** Thousands of skyscrapers are generated on the fly, seamlessly looping to create an infinite sense of speed and scale.
- **Dynamic Lighting:** Buildings react to the time of day, gleaming under the bright afternoon sun and shimmering beneath the glow of your plane's headlights at night.
- **Safe Ceiling:** The flight ceiling and floor are mathematically clamped, ensuring you skim the rooftops at breathtaking speeds without colliding into the ground.

### Intelligent Autopilot (Pathfinder AI)
Feeling lazy or need help navigating the challenging ring courses? Press a button to hand over control to the onboard AI!
- **Target Acquisition:** The AI automatically scans for the nearest active ring objective in 3D space.
- **Smooth Interpolation:** Calculates the optimal yaw, pitch, and roll to smoothly intercept the target.
- **Real-Time Corrections:** Adjusts dynamically if the plane is thrown off course.

### Nitro Boost & Flight Dynamics
- **Arcade Physics:** A balanced mix of realistic gravity and arcade-style maneuverability.
- **Nitro System:** Engage the afterburners for a massive burst of speed. The nitro system dynamically increases your Field of View (FOV) for an intense warp-speed effect, accompanied by custom particle/fire exhaust trails.
- **Throttle Control:** Micro-manage your speed with responsive throttle up/down mechanics, perfect for tight maneuvers.

### Tactical Minimap (HUD Radar)
A fully functional 2D overlay radar maps the 3D world into a top-down tactical view.
- **Spatial Awareness:** Displays your plane in the center, tracking relative positions of upcoming rings and objectives.
- **Altitude Indicators:** HUD markers dynamically change shape (triangles pointing up/down or squares) to indicate if objectives are above, below, or at your exact altitude.

### Dynamic Day/Night Cycle & Illumination
- **Time of Day:** Switch between brilliant daylight and deep night modes instantly.
- **Custom Lighting Models:** Features ambient moonlight, blinding sun reflections, and a custom forward-facing spotlight (Headlight) attached to the nose of the plane that cuts through the darkness.
- **Blinking Beacons:** Real-time flashing navigation lights on the wingtips (Red/Blue) powered by the elapsed internal clock.

### Cinematic Camera System
Experience the flight from multiple perspectives:
- **Mode 0:** Third-Person Follow Cam (Classic arcade view)
- **Mode 1:** First-Person Cockpit View (Immersive)
- **Mode 2:** Cinematic Fly-by
- **Mode 3:** Free Look / Orbital Cam

---

## Controls & Keybindings

Master the skies with these intuitive keyboard controls:

| Key | Action | Description |
| :---: | :--- | :--- |
| **W** | Throttle Up | Increase cruising speed |
| **S** | Throttle Down | Decrease cruising speed |
| **X** | **NITRO BOOST** | Activate afterburners (Hold for max speed!) |
| **A** | Toggle Autopilot | Engage/Disengage the Pathfinder AI |
| **C** | Switch Camera | Cycle through the 4 available camera modes |
| **L** | Toggle Lights | Turn the global lighting engine on or off |
| **P** | Toggle Headlight | Turn the plane's front-facing spotlight on/off |
| **N** | Toggle Night Mode | Switch between Day and Night environments |
| **F** | Toggle Wireframe | Switch graphics between Solid Fill and Wireframe |
| **R** | Reset Camera | Instantly reset the camera to the default view |
| **Space** | Pause | Freeze the simulation and time |
| **+ / -** | Adjust Mesh | Dynamically increase or decrease polygon count |

---

## System Requirements & Dependencies

To run and compile ProjetAvion, your machine must meet the following requirements:

- **OS:** Linux (Ubuntu/Debian, Fedora, Arch) or Windows (via MinGW or Visual Studio)
- **Compiler:** GCC/G++ supporting C++11 or higher
- **Graphics API:** OpenGL 3.0+
- **Libraries:**
  - `freeglut3` / `freeglut3-dev`
  - `libglew-dev`
  - `libgl1-mesa-dev`
  - `libglu1-mesa-dev`

### Installing Dependencies (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential freeglut3-dev libglew-dev libgl1-mesa-dev libglu1-mesa-dev
```

---

## Build & Execution Instructions

ProjetAvion is built using standard GNU Make and G++. 

### 1. Compiling the Source
Navigate to the root directory of the repository and run the following G++ command to link all the necessary OpenGL libraries and compile the C++ classes:

```bash
g++ Src/main.cpp Src/Engine/*.cpp Src/Entities/*.cpp -o ProjetAvion_linux -lGL -lGLU -lglut -lGLEW
```

### 2. Running the Simulator
Once compiled successfully, an executable named `ProjetAvion_linux` will be generated. Run it using:

```bash
./ProjetAvion_linux
```

*(Note: Ensure that the `Resources/` folder is present in the same directory, as the game loads external `.bmp` textures for the skybox and plane fuselage from there.)*

---

## Project Architecture

The codebase is cleanly separated into logical directories to separate the physics/game logic from the rendering engine:

```text
ProjetAvion/
├── Bin/                    # Legacy binary/compilation folder
├── Include/                # Header files (if separated)
├── Resources/              # Textures (metal.bmp, sky.bmp) and 3D Models
├── Src/                    # Main C++ Source Code
│   ├── Engine/             # Core OpenGL rendering, Camera, and Lighting
│   ├── Entities/           # Game Objects (Player, Cloud, Obstacle, Rings)
│   ├── Maths/              # Custom Vector/Matrix math libraries
│   └── main.cpp            # Application Entry Point & Game Loop
├── .gitignore              # Git ignore rules for build artifacts
└── ProjetAvion_linux       # Compiled Executable
```

### Key Classes
- **`Player.cpp` / `Player.h`:** Handles the flight model, input interpolation, nitro logic, and drawing the complex hierarchical 3D mesh of the airplane (wings, propellers, thrusters).
- **`Camera.cpp` / `Camera.h`:** Manages the view matrix, FOV interpolation for nitro, and the different tracking modes.
- **`Obstacle.cpp` / `Obstacle.h`:** Defines the floating rings in the sky, their collision detection, and score tracking.

---

## Rendering Techniques Used

ProjetAvion relies on standard legacy OpenGL (Fixed Function Pipeline) augmented with custom mathematical transformations to achieve its aesthetic:
- **Hierarchical Modeling:** The plane is built using `glPushMatrix()` and `glPopMatrix()` to create complex articulated structures like the spinning propeller.
- **Material Properties:** Heavy use of `glMaterialfv` to simulate different surfaces (shiny metal, matte ground, glowing fire).
- **Texture Mapping:** Spherical and linear UV mapping used for the skybox and the plane's fuselage.
- **Frustum Culling & Fog:** (Optional depending on build) to manage the massive procedural city without tanking the framerate.
- **Alpha Blending:** Used for the transparent HUD radar and particle effects.

---

---

## Project Submission Details

This section contains all the information required for the project submission as per the evaluation guidelines.

### 1. Group Members
- **Student Name 1:** [Enter Name] (Email: [Enter Email])
- **Student Name 2:** [Enter Name] (Email: [Enter Email])

### 2. Work Distribution
- **[Student Name 1]:** Responsible for [e.g., Physics Engine, Camera System, HUD Design].
- **[Student Name 2]:** Responsible for [e.g., Procedural City Generation, Texture Mapping, Autopilot AI].
- **Collaborative Work:** Overall architecture design and debugging.

### 3. Project File Manifest (Zip Contents)
Below is a commented list of all files included in this project archive:
- **`Src/main.cpp`:** Main entry point containing the game loop, initialization, and input handling.
- **`Src/Engine/`:** Contains core engine classes.
  - `Camera.cpp / .h`: Implementation of the multi-mode camera system.
- **`Src/Entities/`:** Contains game object classes.
  - `Player.cpp / .h`: Flight dynamics and 3D plane mesh rendering.
  - `Cloud.cpp / .h`: Atmospheric cloud object logic.
  - `Obstacle.cpp / .h`: Scoring rings and collision detection.
- **`Src/Resources/`:** External assets used by the simulator.
  - `metal.bmp`: Fuselage texture for the aircraft.
  - `sky.bmp`: Texture used for the skybox environment.
- **`Include/`:** Header dependencies for OpenGL (GL, GLU, GLUT, GLEW).
- **`Lib/`:** Pre-compiled library files for cross-platform compatibility.
- **`ProjetAvion_linux`:** The compiled executable for Linux systems.
- **`README.md`:** This documentation file.

### 4. Developed Classes Documentation
A detailed look at the custom classes developed for this project:
- **`Player Class`**: Manages the aircraft's state (position, rotation, velocity). Handles hierarchical modeling for the propeller and wing animations.
- **`Camera Class`**: Implements 4 distinct viewing modes and handles smooth Field of View (FOV) transitions during nitro boosts.
- **`Obstacle Class`**: Represents the target rings. Includes logic for distance-based shape changing on the radar and collision verification.
- **`Cloud Class`**: Simple entity for background environmental detail.

### 5. Project Subject & Description
**ProjetAvion** is a high-performance 3D flight simulator developed in C++ and OpenGL. The objective was to create a functional flight engine that supports infinite terrain generation, complex object collision, and a semi-autonomous autopilot system.

### 6. User Documentation (How to Use)
- **Controls**: See the [Controls & Keybindings](#controls--keybindings) section above.
- **Build**: Follow the instructions in [Build & Execution](#build--execution-instructions).
- **Navigation**: Use the Radar HUD to locate objectives (Rings) and fly through them to increase your score.

### 7. Technical Implementation Comments
- **Hierarchical Modeling**: The plane is built using a stack-based approach (`glPushMatrix/glPopMatrix`) to ensure the propeller rotates relative to the fuselage.
- **Physics**: Implements a custom drag and lift model to simulate flight physics, coupled with a nitro system that scales both velocity and FOV.
- **Optimization**: The city generation uses a procedural looping algorithm to maintain high performance without loading thousands of unique assets.

> [!IMPORTANT]
> **Acknowledgment of Receipt:** An acknowledgment of receipt will be sent via email to the addresses provided in Section 1 once the archive has been retrieved and extracted. If any issues are encountered during extraction or execution, you will be notified at these same addresses.

---

## Contributing

Contributions, issues, and feature requests are welcome! If you want to add new planes, more complex city generation, or multiplayer support, feel free to fork this repository.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## License

This project is open-source and free to use for educational and entertainment purposes. Happy flying!

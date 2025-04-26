# MiniRT

**MiniRT** is a C-based mini ray tracer that utilizes a minimal graphics library.

A ray tracer is a computer graphics technique used to create highly realistic images by simulating the behavior of light in a virtual 3D environment. In ray tracing, the core concept revolves around tracing the path of light rays as they interact with objects and surfaces within a scene.

This project focuses purely on **CPU calculations** and runs **only on Linux** systems.

---

## ✨ Features

- **BVH Tree** acceleration (Bounding Volume Hierarchy) for efficient ray-object intersection tests
- **Multithreaded** rendering for faster image computation
- **SIMD** optimizations for faster vector math and BVH navigation
- **Cache** optimization for improved memory access patterns
- **Real-Time** rendering (continuous updates on camera movement)
- **Anti-Aliasing** for smoother edges
- **Trimesh** object support (.obj file loading)
- Switchable **Camera Modes** (orbit or static mode)
- **Mouse & Keyboard** controls to move and rotate the camera
- **Pure CPU-based** calculations, no GPU acceleration
- Supports planes, spheres, cylinders, rectangles, and triangles

---

## 🎮 Controls

### Keyboard

| Key  | Action                      |
|:----:|:-----------------------------|
| `q` or `ESC` | Quit the program |
| `w` | Move camera forward |
| `s` | Move camera backward |
| `a` | Move camera left |
| `d` | Move camera right |
| `z` | Zoom in |
| `x` | Zoom out |
| `i` | Rotate camera up |
| `k` | Rotate camera down |
| `j` | Rotate camera left |
| `l` | Rotate camera right |
| `u` | Roll camera left |
| `o` | Roll camera right |
| `c` | Toggle camera mode (orbit vs. static) |
| `m` | Enable anti-aliasing |

### Mouse

| Button | Action                      |
|:------:|:-----------------------------|
| Left click + drag | Translate camera |
| Right click + drag | Rotate camera |
| Scroll up | Zoom in |
| Scroll down | Zoom out |

---

## ⚙️ Installation & Usage

```bash
# Clone the repository
git clone https://github.com/maricalmer/miniRT.git minirt
cd minirt

# Build the project
make

# Run scene
./miniRT scenes/suzanne.rt

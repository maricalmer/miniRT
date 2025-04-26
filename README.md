# MiniRT

**MiniRT** is a lightweight ray tracer written in C, designed to create realistic 3D images through a custom minimal graphics library.

Ray tracing is a rendering technique that simulates the way light travels and interacts with objects to produce highly detailed visuals. By tracing the paths of light rays through a virtual scene, it generates lifelike reflections, shadows, and materials with great accuracy.

This project is entirely CPU-based and built specifically for Linux environments.

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

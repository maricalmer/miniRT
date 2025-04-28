# MiniRT

**MiniRT** is a 3D rendering engine written in C, using the ray tracing method.

In 3D computer graphics, ray tracing simulates the behavior of light to create realistic digital images. By tracing the paths of light rays through a virtual scene, it produces highly accurate reflections, refractions and shadows.

This project runs entirely on the CPU and is designed specifically for Linux systems.

---

## 🔮 100k spheres

<div align="center">
  <a href="https://github.com/maricalmer/miniRT">
    <img src="assets/renders/100k.gif" max-width="100%" height="100%"/>
  </a>
</div>

---

## 📸 Renders
<table align=center>
	<thead>
		<tr>
			<th colspan=2>click to enlarge</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td><image src="assets/renders/mask_teeth.png"></image></td>
			<td><image src="assets/renders/suzanne.png"></image></td>
		</tr>
		<tr>
			<td><image src="assets/renders/duck.png"></image></td>
			<td><image src="assets/renders/whitted.png"></image></td>
		</tr>
	</tbody>
</table>

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
./miniRT assets/scenes/100k_happy.rt
```

---

## 📚 **Resources**

- [Nvidia intro to Ray Tracing](https://www.youtube.com/watch?v=gBPNO6ruevk)
- [Physics in Ray Tracing](https://physique.cmaisonneuve.qc.ca/svezina/nyc/note_nyc/NYC_CHAP_6_IMPRIMABLE_4.pdf)
- [Whitted algo](https://dl.acm.org/doi/pdf/10.1145/358876.358882)
- [BVH structure](https://www.realtimerendering.com/raytracing/Ray%20Tracing_%20The%20Next%20Week.pdf)

---

## 🏭 **Further development**

- Write more tests
- Add portability for main platforms

---

## 📝 **Authors**

- [Harold Ruiz-Franck](https://github.com/haroldrf) ([**hruiz-fr**](https://profile-v3.intra.42.fr/users/hruiz-fr))
- [David Lemaire](https://github.com/maricalmer) ([**dlemaire**](https://profile-v3.intra.42.fr/users/dlemaire))


# Hexagonal Lattice Boltzmann Simulation

This project implements a parallel and serial Lattice Boltzmann Method (LBM) simulation on a hexagonal lattice. It models fluid particle behavior across a 2D grid using discrete velocity directions. The simulation is parallelized using MPI for distributed computing and supports visualization via MATLAB.

---

## 📁 Project Structure

| File | Description |
|------|-------------|
| `serial.c` | Serial implementation of the LBM model |
| `parallel.c` | Parallel MPI version with inter-process communication |
| `parallel.sh` | SLURM batch script to run `parallel.c` on HPC |
| `hex_visualize.m` | MATLAB script to visualize simulation frames |

---

## ⚙️ Simulation Details

- **Lattice**: 2D hexagonal (6 directions)
- **Grid Size**: 300 × 100 nodes (configurable via constants)
- **Boundary Conditions**:
  - Slip on left/right
  - Dirichlet on top/bottom
- **Processes**: Domain decomposed across `x_nodes`
- **Timesteps**: 1000 (default)
- **Output**: One `.txt` file per timestep, named as `output{t}.txt`

---

## 🚀 Build Instructions

### Serial

```bash
gcc -O2 -o proj_serial serial.c
./proj_serial

Parallel (MPI)
mpicc -O2 -o proj_parallel parallel.c
mpirun -np <N_PROCESSES> ./proj_parallel
You can test locally using fewer processors or run on a cluster with SLURM.

☁️ SLURM Usage
sbatch parallel.sh

Make sure to match --ntasks-per-node and -np in the script for consistent parallel runs.

📄 Output Format
Each output file (output{t}.txt) contains:

Number of x_nodes

Number of y_nodes

x_nodes × y_nodes rows of 6-bit binary strings, representing occupancy in each of 6 hex directions

📊 Visualization (MATLAB)
To visualize the simulation:

Open hex_visualize.m in MATLAB

Make sure abhilash.txt or outputX.txt is available

Run the script to render directional particle movement on the hexagonal grid

Modify the file path in fopen() if you're visualizing a different output.

📈 Performance Note
Each MPI process handles a chunk of x_nodes

Overlap communication + computation for better scaling

Initial domain setup includes dirichlet_in() injection and slip() boundaries

Suitable for benchmarking serial vs. parallel performance

👤 Author
Nahin – 2025
© 2025 All rights reserved

🧠 Future Enhancements
Add support for visual animations (e.g. video exports)

Implement other boundary types (e.g. bounce-back)

GPU acceleration (OpenCL/CUDA)

Interactive web-based result viewer

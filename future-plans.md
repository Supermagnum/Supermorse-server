Future plans:


Add frequency offset simulation (the core of the "Donald Duck" effect)

Implement BFO (Beat Frequency Oscillator) simulation

Add characteristic SSB filtering and bandwidth limitations

Create a tuning interface that lets users experience the off-frequency effects

Add frequences and bands from bandplans, 80,60,30,20,17,15,10, and 6 meters.

Apply frequency shifting to simulate the off-frequency tuning effect

Add realistic background noise and static

Implement a "tuning" control that adjusts the frequency offset in real-time

Modulation simulation: USB (Upper Sideband) and LSB (Lower Sideband) single sideband modulation can be digitally generated using mathematical transformations like the Hilbert transform

Audio filtering: Bandpass filters simulate the narrow bandwidth typical of SSB communications (roughly 2.4-3 kHz)

Support for rotateable antennas, and stationary antennas like Yagi,loop, ground plane 1/4 wave, endfed.


Bandplans and antennas can be found in:
https://github.com/Supermagnum/Supermorse-server/tree/main/Bandplans_and_antennas


4NEC2 Pattern File Support ( these provides antenna gain patterns)
Antennas described in this folder needs to be simulated to get relevant data.


Native formats:

NEC output files - 4NEC2 can export its calculated patterns as text files
Radiation pattern data - Generated from your NEC geometry models
Far-field pattern files - ASCII format with angle/gain data
4NEC2 pattern exports typically include:

Theta/Phi coordinates (spherical coordinates)
Gain values in dBi
Frequency information
Polarization components (vertical/horizontal)


Hardware needed:
PowerEdge R720 2X Intel(R) Xeon(R) CPU E5-2650 0 @ 2.00GHz 
GPU options:
Tesla P40 (~$200-400 used)

Specs: Single GPU, 3840 CUDA cores, 24GB GDDR5
Power: 250W

Tesla P100 (~$400-800 used)

Specs: 3584 CUDA cores, 16GB HBM2
Power: 250W



Tesla P4 (~$50-150 used),- 4X

Specs: 2560 CUDA cores, 8GB GDDR5
Power: 75W (no supplementary power needed)


OS:
Tesla P4 Setup (Ubuntu 20.04 LTS)

Concurrent Users: 100-140
Bottleneck: GPU compute power (only 2560 CUDA cores)
Advantages: Low power, could potentially run multiple P4s
Best for: Basic propagation calculations, cost-effective option

Tesla P40 Setup (Ubuntu 20.04 LTS)

Concurrent Users: 150-220
Bottleneck: CPU coordination with all that GPU power
Advantages: 24GB VRAM handles large propagation matrices, excellent price/performance
Best for: Complex interference modeling, large user populations

Tesla P100 Setup (Ubuntu 22.04 LTS)

Concurrent Users: 180-250
Bottleneck: Your dual E5-2650s become the limiting factor
Advantages: HBM2 memory bandwidth, fastest compute performance
Best for: Real-time complex atmospheric modeling, maximum performance

Key Factors:

Memory: All three have enough VRAM for your user counts
CPU Limitation: Your E5-2650s will likely max out before the P40/P100 do
Network: You'll need substantial bandwidth (50-100 Mbps) for 200+ users


Note:
Mumble requires: CMake 3.15 or higher
* Qt 5.12 or higher
* C++ compiler with C++14 support (GCC, Clang, MSVC)
* SQLite development libraries

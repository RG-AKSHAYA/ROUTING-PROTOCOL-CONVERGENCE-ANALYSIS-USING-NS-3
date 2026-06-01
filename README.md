# Routing Protocol Convergence Analysis using NS-3

**Computer Communications and Networks**

---

## Overview

This project compares three routing protocols — **RIP**, **OSPF**, and **EIGRP** — using the NS-3 network simulator. All three are simulated on the same six-router linear topology under identical conditions, including a deliberate mid-simulation link failure and recovery, to get a fair side-by-side comparison of their convergence behaviour and resource usage.

---

## Repository Structure

```
├── simulation/
│   ├── rip_project.cc      # RIP simulation (Distance Vector)
│   ├── ospf_like.cc        # OSPF simulation (Link State)
│   └── eigrp_like.cc       # EIGRP simulation (Hybrid / DUAL)
│
├── analysis/
│   ├── analyze.py          # Automated PCAP packet count extraction
│   └── generate_graphs.plt # Gnuplot scripts for all metric graphs
│
├── graphs/
│   ├── PIC1.png            # Convergence time graph
│   ├── PIC2.png            # Control packet overhead graph
│   ├── PIC3.png            # Memory usage graph
│   └── graph4.png          # Combined comparison graph
│
├── animation/
│   ├── rip_animation.xml   # NetAnim trace — RIP
│   ├── ospf_animation.xml  # NetAnim trace — OSPF
│   └── eigrp_animation.xml # NetAnim trace — EIGRP
│
└── report/
    └── Routing_Protocol_Convergence_Report.docx
```

---

## Network Topology

```
[ R0 ] ──10.1.1.0/24── [ R1 ] ──10.1.2.0/24── [ R2 ] ──10.1.3.0/24── [ R3 ]
                                                          ↑ FAILURE LINK ↑
[ R3 ] ──10.1.4.0/24── [ R4 ] ──10.1.5.0/24── [ R5 ]
```

| Parameter     | Value          |
|---------------|----------------|
| Routers       | 6 (R0 – R5)    |
| Links         | Point-to-point |
| Data Rate     | 5 Mbps         |
| Delay         | 2 ms           |
| Subnets       | /24            |
| Traffic       | UDP echo       |
| Failure at    | t = 8 s        |
| Recovery at   | t = 12 s       |
| Sim duration  | 20 s           |

---

## How to Run

### 1. Build & Run Simulations

Copy the `.cc` files to your NS-3 scratch directory, then:

```bash
cd ~/ns-3
./ns3 build

./ns3 run "scratch/rip_project"
./ns3 run "scratch/ospf_like"
./ns3 run "scratch/eigrp_like"
```

Each run produces:
- `*_routes.txt` — routing table snapshots at t=5s, t=10s, t=14s
- `*-0-0.pcap` — packet capture on every link
- `*_animation.xml` — NetAnim visualisation file

### 2. Analyse PCAP Output

```bash
# Automated
python3 analysis/analyze.py

# Manual cross-check
tcpdump -nn -r rip_project-0-0.pcap  | wc -l
tcpdump -nn -r ospf_like-0-0.pcap    | wc -l
tcpdump -nn -r eigrp_like-0-0.pcap   | wc -l
```

### 3. Generate Graphs

```bash
gnuplot analysis/generate_graphs.plt
```

### 4. Measure Memory & CPU

```bash
/usr/bin/time -v ./ns3 run "scratch/rip_project"
/usr/bin/time -v ./ns3 run "scratch/ospf_like"
/usr/bin/time -v ./ns3 run "scratch/eigrp_like"
```

### 5. View Animations (NetAnim)

```bash
cd ~/netanim/build
./netanim
# Open: rip_animation.xml / ospf_animation.xml / eigrp_animation.xml
```

---

## Results Summary

| Metric            | RIP      | OSPF     | EIGRP    | Best          |
|-------------------|----------|----------|----------|---------------|
| Convergence Time  | 4.0 s    | 2.0 s    | 1.0 s    | **EIGRP**     |
| Control Packets   | 77       | 24       | 24       | OSPF / EIGRP  |
| Memory Usage      | 5000 KB  | 4200 KB  | 4000 KB  | **EIGRP**     |
| CPU Time          | 1.2 s    | 0.9 s    | 0.7 s    | **EIGRP**     |
| Scalability       | Poor     | Good     | Excellent| **EIGRP**     |
| Open Standard     | ✓        | ✓        | ✗        | RIP / OSPF    |

**EIGRP** outperforms across all resource metrics. **OSPF** is the best open-standard choice. **RIP** is suitable only for small, static networks.

---

## Tools Used

| Tool     | Version | Purpose                          |
|----------|---------|----------------------------------|
| NS-3     | 3.38    | Network simulation               |
| Gnuplot  | 5.4     | Graph generation                 |
| Python 3 | 3.10    | PCAP analysis scripting          |
| NetAnim  | 3.108   | Animated packet flow visualiser  |
| tcpdump  | 4.99    | Packet capture analysis          |

---

## Dependencies

```bash
# NS-3 (Ubuntu/Debian)
sudo apt install g++ python3 cmake ninja-build git
# Follow: https://www.nsnam.org/wiki/Installation

# Other tools
sudo apt install gnuplot tcpdump python3
```

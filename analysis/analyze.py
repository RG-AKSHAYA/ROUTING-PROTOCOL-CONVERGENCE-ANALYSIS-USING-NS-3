
import os
import subprocess

PCAP_FILES = {
    "RIP":   "rip_project-0-0.pcap",
    "OSPF":  "ospf_like-0-0.pcap",
    "EIGRP": "eigrp_like-0-0.pcap",
}

def count_packets(pcap_file: str) -> int:
    """Return total packet count in a PCAP file using tcpdump."""
    result = subprocess.run(
        ["tcpdump", "-r", pcap_file],
        capture_output=True, text=True
    )
    lines = [l for l in result.stdout.splitlines() if l.strip()]
    return len(lines)

def main():
    print("\n" + "="*50)
    print("  PCAP Packet Count Analysis")
    print("="*50)

    results = {}
    for protocol, fname in PCAP_FILES.items():
        if os.path.exists(fname):
            count = count_packets(fname)
            results[protocol] = count
            print(f"  {protocol:<8} | {fname:<30} | {count} packets")
        else:
            print(f"  {protocol:<8} | {fname:<30} | FILE NOT FOUND")

    if results:
        total = sum(results.values())
        print("\n" + "-"*50)
        print(f"  {'Total':<8} | {'All protocols':<30} | {total} packets")
        print("\n  Bandwidth share:")
        for protocol, count in results.items():
            pct = (count / total * 100) if total > 0 else 0
            print(f"    {protocol}: {pct:.1f}%")

    print("="*50 + "\n")

if __name__ == "__main__":
    main()
